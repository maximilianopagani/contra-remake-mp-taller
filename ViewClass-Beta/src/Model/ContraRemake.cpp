#include "../Vista/GameView.hh"
#include "../Utils/Logger.hh"
#include "../Utils/GameParser.hh"
#include "../Utils/Utils.hh"
#include "Game.hh"

Game* synergy;

int main(int argc, char* args[])
{

	// se inicia el logger por consola para el parser
	LOGGER_INIT_FILELESS(Logger::DEBUG);
	GameParser* parser = new GameParser();
	if (parser->loadConfiguration()) {
		parser->testDataParserModel();
	}
	// se mata al primer logger
	LOGGER_KILL();

	// a partir del archivo de configuracion se crea el logger deseado
	if (parser->getLevel() == "DEBUG") {
		LOGGER_INIT(Logger::DEBUG);
	}
	else if (parser->getLevel() == "INFO") {
		LOGGER_INIT(Logger::INFO);
	}
	else {
		LOGGER_INIT(Logger::ERROR);
	}

	GameView* view = new GameView();
	if(!view->init())
	{
		LOGGER_ERROR("Error al inicializar GameView. Se aborta ejecución.");
		view->destroy();
	}
	else
	{
		//Manejo del framerate
		const int FPS = 35;
		const int frameDelay = 1000 / FPS ;
		Uint32 timeAtIterationStart;
		int iterationTime;

		synergy = new Game(parser, view);

		synergy->init();

		//CICLO DEL JUEGO
		while(synergy->state())
		{
			timeAtIterationStart = view->getTicks();

			synergy->handleEvents();

			synergy->update();

			synergy->render();

			//Manejo del framerate
			iterationTime = view->getTicks() - timeAtIterationStart;

			if(frameDelay > iterationTime) // Si lo que tardó la iteracion es menor a lo que debe tardar un ciclo para mostrarse a la tasa de frames deseada
			{
				view->delay(frameDelay - iterationTime);
			}
		}
		synergy->destroy();
		LOGGER_KILL();
	}

	return 0;
}