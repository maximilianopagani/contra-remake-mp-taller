/*
 * ServerHandler.cpp
 *
 *  Created on: Oct 12, 2018
 *      Author: maximiliano
 */

#include "ServerHandler.hh"
#include "../../Utils/ServerParser.hh"

ServerHandler::ServerHandler(int _port, int _max_clients, std::list<UserParser> _users) // @suppress("Class members should be properly initialized")
{
	max_clients = _max_clients;
	port = _port;
	listening_socket = -1;
	users = _users;
}

bool ServerHandler::startServer()
{
	// CREO UN SOCKET PARA ESCUCHAR CONEXIONES DEL TIPO IPV4 Y STREAM
	listening_socket = socket(AF_INET, SOCK_STREAM, 0);

	// FALLA LA CREACION DEL SOCKET
	if(listening_socket == -1)
	{
		LOGGER_ERROR("Falla al crear el socket de escucha inicial");
		return false;
	}
	// ESTRUCTURA DE INFORMACION QUE DEFINE COMPORTAMIENTO DEL SOCKET PARA ESCUCHAR CONEXIONES "listening_socket"

	server_address.sin_family = AF_INET; // PROTOCOLO IVP4
	server_address.sin_addr.s_addr = INADDR_ANY; // TOMA LA IP ACTUAL DEL HOST
	server_address.sin_port = htons(port); // TRANSFORMO EL PUERTO A EL ORDEN DE BYTES ESTANDAR ESPERADO EN LAS REDES (BIG ENDIAN)

	// ASOCIO EL SOCKET QUE CREE PARA ESCUCHAR CONEXIONES CON UNA DIRECCION DE IP, PUERTO Y FAMILIA DE PROTOCOLO DEFINIDO ANTERIORMENTE
	if(bind(listening_socket, (struct sockaddr*) &server_address, sizeof(server_address)) == -1)
	{
		LOGGER_ERROR("Falla al asociar el socket de escucha con la estructura deseada");
		return false;
	}

	// LE DIJO AL SOCKET QUE SE QUEDE ESCUCHANDO CONEXIONES ENTRANTES (HASTA max_clients CONEXIONES)
	if(listen(listening_socket, max_clients) == -1)
	{
		LOGGER_ERROR("Falla al setear al socket en modo de espera de conexiones");
		return false;
	}

	return true;
}

void ServerHandler::startListeningThread()
{
	pthread_create(&accept_connections_thread, NULL, &ServerHandler::acceptConnectionsThread, this);
}

// Para invocar un thread no se puede con un metodo común de un objeto, hay que llamar una funcion estática, y pasarle los objetos para luego llamar al método de instancia.
void* ServerHandler::acceptConnectionsThread(void* server)
{
	LOGGER_DEBUG("Iniciando acceptConnectionsThread");
	((ServerHandler*)server)->acceptConnections();
	return nullptr;
}

struct thread_arg_struct {
	ServerHandler* arg_server;
	Client* arg_client;
};

// Metódo infinito, mediante un thread queda a la escucha de nuevas conexiones y las va a aceptar
// si la cantidad de jugadores conectados no alcanzó el máximo para esa partida
void ServerHandler::acceptConnections()
{
	while(true)
	{

		struct sockaddr_in client_address; // Estructura que me va a servir para guardar la informacion de la conexion entrante
		socklen_t address_size = sizeof(client_address);

		LOGGER_DEBUG("Invocando llamada bloqueante accept(). En espera de conexiones...");

		// LLAMADA BLOQUEANTE. NO avanza hasta que no recibe y acepta una nueva conexión. Si salió to.do OK, me devuelve el archivo
		// descriptivo de un NUEVO SOCKET creado exclusivamente para esa conexión (un integer). El socket original de escucha sigue escuchando, no se ve modificado.
		int new_socket = accept(listening_socket, (struct sockaddr*) &client_address, &address_size);
		// Guarda los datos de la conexion en la estructura client_address

		if(new_socket != -1) // Si salió to.do OK
		{
			if(!this->isGameFull() || !this->allClientsOnline()) // Todavía no llegué a la cantidad de jugadores seteados para esa partida
			{
				string sep = ": ";
				// Creo nuevo cliente con los datos que obtuve de client_address. Es necesario hacer esas conversiones con las llamadas "inet_ntoa" y "n_tohs" ya que originalmente los datos
				// vienen en el formato de red estándar (big endian) y en mi maquina los quiero en (little endian)
				LOGGER_INFO("Conexión establecida con IP" + sep + inet_ntoa(client_address.sin_addr) + " - PUERTO: " + std::to_string(ntohs(client_address.sin_port)));
				// Mando un mensaje al cliente recien conectado
				this->sendToSocket(new_socket, new MessageServer(INFO, NONE, "Te conectaste al servidor."));

				this->sendToSocket(new_socket, new MessageServer(REQUEST, LOGIN, "Solicito datos de login."));

				char msg[256];
				if(this->receiveOneMessageFromSocket(new_socket, msg, sizeof(msg)))
				{
					LOGGER_DEBUG("Recibo mensaje del cliente" + sep + msg);

					std::string user;
					std::string passw;

					if(this->extractUserAndPasswFromMsg(new MessageServer(msg), user, passw))
					{
						LOGGER_DEBUG("Extraigo User: " + user + " - Password: " + passw);

						if(this->validateUserAndPassw(user, passw)) // si chequeo validacion user y passw es correcto
						{
							LOGGER_DEBUG("Datos de login validados");

							if(!this->alreadyOnline(user, passw))
							{
								LOGGER_DEBUG("Chequeo si jugador ya está online OK");

								if(!this->allClientsOnline() && this->alreadyLoggedBefore(user, passw))
								{
									LOGGER_DEBUG("Entro en el caso para reconectar");

									Client* reconnectClient = this->searchForClient(user, passw);

									LOGGER_DEBUG("Encuentro cliente anterior para asociar");

									if(reconnectClient != NULL)
									{
										if(this->isGameFull())
										{
											LOGGER_DEBUG("El juego ya reservó todos los lugares para jugadores disponibles. Es una reconexión estandar con el juego ya comenzado");

											MessageServer* reconnect_msg = new MessageServer(INFO, RECONNECT, "Info de reconnect para game.");
											reconnect_msg->setPlayerId(reconnectClient->getClientId());
											reconnect_msg->setUsername(reconnectClient->getUsername());
											pthread_mutex_lock(&server_mutex);
											server_recv_msgs_queue.push(reconnect_msg);
											pthread_mutex_unlock(&server_mutex);
											LOGGER_DEBUG("Mensaje de reconexión enviado a game");
										}
										else
										{
											LOGGER_DEBUG("El juego todavía no reservó todos los lugares para jugadores disponibles. Estoy en etapa de espera de conexiones");
										}

										reconnectClient->setOnline(new_socket);
										LOGGER_DEBUG("Seteando cliente nuevamente online");
										struct thread_arg_struct args;
										args.arg_server = this;
										args.arg_client = reconnectClient;
										LOGGER_DEBUG("Creando nuevo thread de recepción de mensajes para el cliente");
										pthread_create(reconnectClient->getRecieveMessagesThread(), NULL, &ServerHandler::recieveMessagesFromClientThread, (void*) &args);
									}
									else
									{
										this->sendToSocket(new_socket, new MessageServer(ERROR, RECONNECT, "Error raro al reconectar."));
										Utils::setDelay(300);
										shutdown(new_socket, SHUT_RDWR);
									}
								}
								else if(!this->isGameFull())
								{
									LOGGER_DEBUG("El juego todavía no reservó todos los lugares para jugadores disponibles");

									if(!this->alreadyLoggedBefore(user, passw))
									{
										LOGGER_DEBUG("El usuario no se ha logeado antes");

										Client* new_client = new Client(new_socket, inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port), user, passw);

										LOGGER_INFO("Creado cliente con ID: " + std::to_string(new_client->getClientId()));

										// Agrego el cliente a la lista de jugadores conectados
										pthread_mutex_lock(&server_clients_mutex);
										connectedClients.push_back(new_client);
										pthread_mutex_unlock(&server_clients_mutex);

										this->sendToSocket(new_socket, new MessageServer(INFO, WAITINGPLAYERS, "Informo login OK. Esperando."));

										// CREO UN THREAD DEDICADO A ESCUCHAR MENSAJES DE ESE NUEVO CLIENTE (PORQUE RECV ES UNA LLAMADA BLOQUEANTE Y ME BLOQUEARIA TO.DO SI UN CLIENTE NO MANDA NADA)
										struct thread_arg_struct args;
										args.arg_server = this;
										args.arg_client = new_client;

										LOGGER_DEBUG("Creando nuevo thread de recepción");

										pthread_create(new_client->getRecieveMessagesThread(), NULL, &ServerHandler::recieveMessagesFromClientThread, (void*) &args);

										// LOG DE EXITO
										// MANDAR PANTALLA DE ESPERANDO JUGADORES O LO QUE SEA ANTES DE ENTRAR A LA OTRA LLAMADA BLOQUEANTE DE ACCEPT
									}
									else
									{
										this->sendToSocket(new_socket, new MessageServer(ERROR, LOGIN_ALREADY_ON, "Usuario ya logeado."));
										Utils::setDelay(300);
										shutdown(new_socket, SHUT_RDWR);
									}
								}
								else
								{
									this->sendToSocket(new_socket, new MessageServer(ERROR, GAMEFULL, "Juego completo."));
									Utils::setDelay(300);
									shutdown(new_socket, SHUT_RDWR);
								}
							}
							else
							{
								this->sendToSocket(new_socket, new MessageServer(ERROR, LOGIN_ALREADY_ON, "Usuario online."));
								Utils::setDelay(300);
								shutdown(new_socket, SHUT_RDWR);
							}
						}
						else
						{
							this->sendToSocket(new_socket, new MessageServer(ERROR, LOGIN_DATA, "User y/o clave incorrectos."));
							Utils::setDelay(300);
							shutdown(new_socket, SHUT_RDWR);
						}
					}
					else
					{
						this->sendToSocket(new_socket, new MessageServer(ERROR, LOGIN_DATA, "Mensaje de login incorrecto."));
						Utils::setDelay(300);
						shutdown(new_socket, SHUT_RDWR);
					}
				}
				else
				{
					LOGGER_ERROR("Error al leer mensaje de login. Conexión perdida o apagada");
					shutdown(new_socket, SHUT_RDWR);
				}
			}
			else
			{
				this->sendToSocket(new_socket, new MessageServer(ERROR, GAMEFULL, "Juego completo."));
				Utils::setDelay(300);
				shutdown(new_socket, SHUT_RDWR);
			}
		}
		else
		{
			LOGGER_ERROR("Error al tratar de aceptar conexión entrante y crear nuevo socket");
		}
	}
}

bool ServerHandler::readyToStartGame()
{
	if(this->isGameFull() && this->allClientsOnline())
		return true;
	else
		return false;
}

Client* ServerHandler::searchForClient(std::string user, std::string passw)
{
	for(uint i = 0; i < connectedClients.size(); i++)
	{
		if((connectedClients.at(i)->getUsername() == user) && (connectedClients.at(i)->getPassword() == passw))
			return connectedClients.at(i);
	}
	return NULL;
}

int ServerHandler::getConnectedClients()
{
	int clients_amount = 0;

	pthread_mutex_lock(&server_clients_mutex);
	clients_amount = connectedClients.size();
	pthread_mutex_unlock(&server_clients_mutex);

	return clients_amount;
}

bool ServerHandler::isGameFull()
{
	return (this->getConnectedClients() >= max_clients);
}

bool ServerHandler::allClientsOnline()
{
	int clients_size = this->getConnectedClients();

	for(int i = 0; i < clients_size; i++)
	{
		if(connectedClients.at(i)->isOffline())
			return false;
	}
	return true;
}

bool ServerHandler::alreadyLoggedBefore(std::string user, std::string passw)
{
	for(uint i = 0; i < connectedClients.size(); i++)
	{
		if((connectedClients.at(i)->getUsername() == user) && (connectedClients.at(i)->getPassword() == passw))
			return true;
	}
	return false;
}

bool ServerHandler::alreadyOnline(std::string user, std::string passw)
{
	for(uint i = 0; i < connectedClients.size(); i++)
	{
		if((connectedClients.at(i)->getUsername() == user) && (connectedClients.at(i)->getPassword() == passw) && (connectedClients.at(i)->isOnline()))
			return true;
	}
	return false;
}

bool ServerHandler::validateUserAndPassw(std::string user, std::string passw) // HAY QUE METER LO DEL PARSER ACA
{
	std::list<UserParser>::iterator it;
	for (it = users.begin(); it != users.end(); it++) {
		if (user == it->getName() && passw == it->getPassword()) return true;
	}
	return false;
}

bool ServerHandler::extractUserAndPasswFromMsg(MessageServer* message, std::string &user, std::string &passw)
{
	int MSG_HEADER_1, MSG_HEADER_2;
	char param1[32]; char param2[32]; char param3[32]; char param4[32];
	char msg[256];

	message->getContent(msg);
	string sep = ": ";
	LOGGER_DEBUG("handleEvents() - Procesando mensaje" + sep + msg);

	sscanf(msg,"%i,%i,%[^,],%[^,],%[^,],%[^,];", &MSG_HEADER_1, &MSG_HEADER_2, param1, param2, param3, param4);

	if(MSG_HEADER_1 == INFO && MSG_HEADER_2 == LOGIN)
	{
		user = param1;
		passw = param2;
		return true;
	}
	else
		return false;
}

// Necesitaba pasarle por parametro el servidor, para que pueda ejecutar el método de la instancia, y el cliente para
// decirle para cual cliente voy a crear el thread de recepcion de mensajes
void* ServerHandler::recieveMessagesFromClientThread(void* arguments)
{
	struct thread_arg_struct* args = (struct thread_arg_struct*) arguments;;
	LOGGER_DEBUG("Iniciando recieveMessagesThread de cliente con IP: " + (args->arg_client)->getIp() + " y PUERTO: " + std::to_string((args->arg_client)->getPort()));
	(args->arg_server)->recieveMessagesFrom((args->arg_client));
	return nullptr;
}

bool ServerHandler::receiveOneMessageFromSocket(int socket, char* dest_char, int dest_char_size)
{
	int bytes_received = 0;

	bytes_received = recv(socket, dest_char, dest_char_size, 0); // LLAMADA BLOQUEANTE. NO avanza hasta recibir un mensaje

	if(bytes_received > 0)
		return true;
	else
		return false;
}

// Metodo infinito, siempre queda en la espera de recepcion de mensajes de ese cliente determinado
void ServerHandler::recieveMessagesFrom(Client* client)
{
	char buffer[256];
	int bytes_received = 0;

	while(true)
	{
		bytes_received = recv(client->getSocket(), buffer, sizeof(buffer), 0); // LLAMADA BLOQUEANTE. NO avanza hasta recibir un mensaje

		if(bytes_received > 0)
		{
			MessageServer* received_msg = new MessageServer(buffer);
			received_msg->setPlayerId(client->getClientId());
			received_msg->setUsername(client->getUsername());
			pthread_mutex_lock(&server_mutex);
			server_recv_msgs_queue.push(received_msg); // PUSHEO EL MENSAJE A LA COLA COMPARTIDA QUE ME SETEÓ GAME
			pthread_mutex_unlock(&server_mutex);

		}
		else if(bytes_received == -1)
		{
			LOGGER_ERROR("Falla en recepción de mensaje");
		}
		else if(bytes_received == 0) // solo funciona si cierro la cruz (que invoca al shutdown desde el cliente y esto lo detecta)
		{
			client->setOffline();

			if(this->isGameFull())
			{
				LOGGER_INFO("Hubo shutdown desde cliente mientras el juego estaba lleno y corriendo");
				MessageServer* disconnected_msg = new MessageServer(INFO, DISCONNECT, "Info de desconexion para game");
				disconnected_msg->setPlayerId(client->getClientId());
				disconnected_msg->setUsername(client->getUsername());
				pthread_mutex_lock(&server_mutex);
				server_recv_msgs_queue.push(disconnected_msg); // PUSHEO EL MENSAJE A LA COLA COMPARTIDA QUE ME SETEÓ GAME
				pthread_mutex_unlock(&server_mutex);
			}
			else
			{
				LOGGER_INFO("Hubo shutdown desde cliente mientras el juego estaba en etapa de espera de conexiones");
			}
			break;
		}
	}
}

void ServerHandler::getNewReceivedMessages(std::queue<MessageServer*>* store_in_queue)
{
	pthread_mutex_lock(&server_mutex);

	MessageServer* message;

	while(!server_recv_msgs_queue.empty())
	{
		message = server_recv_msgs_queue.front();
		server_recv_msgs_queue.pop();
		char msg[256];
		message->getContent(msg);
		LOGGER_DEBUG("Moviendo mensaje de cola de ServerHandler a cola de Game");
		store_in_queue->push(message);
	}

	pthread_mutex_unlock(&server_mutex);
}

void ServerHandler::sendToAllConnectedClients(MessageServer* message)
{
	for(int i = 0; i < max_clients; i++)
	{
		if(connectedClients.at(i)->isOnline())
		{
			char msg[256];
			message->getContent(msg);
			LOGGER_DEBUG("Mensaje enviado al cliente id " + std::to_string(connectedClients.at(i)->getClientId()) + " : " + msg);
			send(connectedClients.at(i)->getSocket(), msg, sizeof(msg), 0);
		}
	}
	delete message;
}

void ServerHandler::sendToConnectedClient(Client* client, MessageServer* message)
{
	if(client->isOnline())
	{
		char msg[256];
		message->getContent(msg);
		LOGGER_DEBUG("Mensaje enviado al cliente id " + std::to_string(client->getClientId()) + " : " + msg);
		send(client->getSocket(), msg, sizeof(msg), 0);
	}
	delete message;
}

void ServerHandler::sendToConnectedClientId(int client_id, MessageServer* message)
{
	if(connectedClients.at(client_id)->isOnline())
	{
		char msg[256];
		message->getContent(msg);
		LOGGER_DEBUG("Mensaje enviado al cliente id " + std::to_string(client_id) + " : " + msg);
		send(connectedClients.at(client_id)->getSocket(), msg, sizeof(msg), 0);
	}
	delete message;
}

void ServerHandler::sendToSocket(int destination_socket, MessageServer* message)
{
	char msg[256];
	message->getContent(msg);
	string sep = ": ";
	LOGGER_DEBUG("Mensaje enviado al socket" + sep + msg);
	send(destination_socket, msg, sizeof(msg), 0);
	delete message;
}

ServerHandler::~ServerHandler() {}

