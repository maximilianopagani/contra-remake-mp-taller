#include "GameParser.hh"

GameParser::GameParser() {
	this->tiXmlFileConfig = NULL;
	this->fileConfigLoaded = false;
	this->tiXmlDefaultFileConfig = NULL;
	this->defaultfileConfigLoaded = false;
	this->level = "";
	this->fondo1Nivel1 = "";
	this->fondo2Nivel1 = "";
	this->fondo3Nivel1 = "";
	this->fondo1Nivel2 = "";
	this->fondo2Nivel2 = "";
	this->fondo3Nivel2 = "";
	this->fondo1Nivel3 = "";
	this->fondo2Nivel3 = "";
	this->fondo3Nivel3 = "";
}

void GameParser::testDataParserModel(){
	// PROBANDO ...
	cout << "El nivel de loggueo almacenado en el parser es: ";
	cout << getLevel() << endl;
	std::list<PlataformaParser>::iterator it;
	std::list<PlataformaParser> lista;

	lista = this->getPlataformas();
    for (it=lista.begin(); it != lista.end();it++){
        int dato = (*it).getId();
    	cout << "El id de la plataforma es: ";
    	cout << dato << endl;
    }
}

bool GameParser::evaluateTagDebug(){
	bool sucess = true;
	string strLevel;

	TiXmlHandle tiXmlHandle(this->tiXmlFileConfig);

	TiXmlNode* tagDebugNode = tiXmlHandle.FirstChild(TAG_CONFIGURATION).FirstChild(TAG_DEBUG).ToNode();

	if (!(tagDebugNode)){
		LOGGER_INIT(Logger::ERROR);
		LOGGER_ERROR("TAG_DEBUG no existe");
		LOGGER_KILL();
	    return false;
	}

    //TAG_LEVEL
	TiXmlNode* tagLevelNode = tagDebugNode->FirstChildElement(TAG_LEVEL);
	if ((tagLevelNode)&&(tagDebugNode->FirstChildElement(TAG_LEVEL)->GetText())) {
		strLevel.clear();
		strLevel.append(tagDebugNode->FirstChildElement(TAG_LEVEL)->GetText());
	    if (!strLevel.compare(VALUE_EMPTY)) {
			LOGGER_INIT(Logger::ERROR);
			LOGGER_ERROR("TAG_LEVEL esta vacio");
			LOGGER_KILL();
	    	sucess = false;
	    }

        this->level = strLevel;
        if (!((this->level.compare("ERROR"))||(this->level.compare("DEBUG"))||(this->level.compare("INFO")))) {
			LOGGER_INIT(Logger::ERROR);
			LOGGER_ERROR("TAG_LEVEL es desconocido");
			LOGGER_KILL();
			sucess = false;
		}

	} else {
		LOGGER_INIT(Logger::ERROR);
		LOGGER_ERROR("TAG_LEVEL no existe o tiene valores invalidos");
		LOGGER_KILL();
		sucess = false;
	}

	if (sucess) {
		if (!(this->level.compare("ERROR"))){
			LOGGER_INIT(Logger::ERROR);
			if (this->fileConfigLoaded == true) {
				LOGGER_INFO("Se ha cargado el archivo de configuracion");
			} else {
				if (this->defaultfileConfigLoaded == true) {
					LOGGER_INFO("Se ha cargado el archivo default de configuracion")
				}
			}
		}

		if (!(this->level.compare("DEBUG"))){
			LOGGER_INIT(Logger::DEBUG);
			if (this->fileConfigLoaded == true) {
				LOGGER_INFO("Se ha cargado el archivo de configuracion");
			} else {
				if (this->defaultfileConfigLoaded == true) {
					LOGGER_INFO("Se ha cargado el archivo default de configuracion")
				}
			}
		}

		if (!(this->level.compare("INFO"))){
			LOGGER_INIT(Logger::INFO);
			if (this->fileConfigLoaded == true) {
				LOGGER_INFO("Se ha cargado el archivo de configuracion");
			} else {
				if (this->defaultfileConfigLoaded == true) {
					LOGGER_INFO("Se ha cargado el archivo default de configuracion")
				}
			}
		}
	}

	return sucess;
}

bool GameParser::evaluateTagNivel1(){
	bool sucess = true;
	string strFondo1;
	string strFondo2;
	string strFondo3;

	TiXmlHandle tiXmlHandle(this->tiXmlFileConfig);
	TiXmlNode* tagNivel1Node = tiXmlHandle.FirstChild(TAG_CONFIGURATION).FirstChild(TAG_ESCENARIOS).FirstChild(TAG_NIVEL1).ToNode();

	if (!(tagNivel1Node)){
		LOGGER_DEBUG("TAG_NIVEL1 no existe");
		return false;
	}

    //TAG_FONDO1
	TiXmlNode* tagFondo1Node = tagNivel1Node->FirstChildElement(TAG_FONDO1);
	if ((tagFondo1Node)&&(tagNivel1Node->FirstChildElement(TAG_FONDO1)->GetText())) {
		strFondo1.clear();
		strFondo1.append(tagNivel1Node->FirstChildElement(TAG_FONDO1)->GetText());
	    if (!strFondo1.compare(VALUE_EMPTY)) {
	    	LOGGER_DEBUG("TAG_FONDO1 esta vacio");
	    	sucess = false;
	    }

        this->fondo1Nivel1 = strFondo1;
	} else {
		LOGGER_DEBUG("TAG_FONDO1 no existe o tiene valores invalidos");
		sucess = false;
	}

    //TAG_FONDO2
	TiXmlNode* tagFondo2Node = tagNivel1Node->FirstChildElement(TAG_FONDO2);
	if ((tagFondo2Node)&&(tagNivel1Node->FirstChildElement(TAG_FONDO2)->GetText())) {
		strFondo2.clear();
		strFondo2.append(tagNivel1Node->FirstChildElement(TAG_FONDO2)->GetText());
	    if (!strFondo2.compare(VALUE_EMPTY)) {
	    	LOGGER_DEBUG("TAG_FONDO2 esta vacio");
	    	sucess = false;
	    }

        this->fondo2Nivel1 = strFondo2;
	} else {
		LOGGER_DEBUG("TAG_FONDO2 no existe o tiene valores invalidos");
		sucess = false;
	}

    //TAG_FONDO3
	TiXmlNode* tagFondo3Node = tagNivel1Node->FirstChildElement(TAG_FONDO3);
	if ((tagFondo3Node)&&(tagNivel1Node->FirstChildElement(TAG_FONDO3)->GetText())) {
		strFondo3.clear();
		strFondo3.append(tagNivel1Node->FirstChildElement(TAG_FONDO3)->GetText());
	    if (!strFondo3.compare(VALUE_EMPTY)) {
	    	LOGGER_DEBUG("TAG_FONDO3 esta vacio");
	    	sucess = false;
	    }

        this->fondo3Nivel1 = strFondo3;
	} else {
		LOGGER_DEBUG("TAG_FONDO3 no existe o tiene valores invalidos");
		sucess = false;
	}

	return sucess;
}

bool GameParser::evaluateTagNivel2(){
	bool sucess = true;
	string strFondo1;
	string strFondo2;
	string strFondo3;

	TiXmlHandle tiXmlHandle(this->tiXmlFileConfig);
	TiXmlNode* tagNivel2Node = tiXmlHandle.FirstChild(TAG_CONFIGURATION).FirstChild(TAG_ESCENARIOS).FirstChild(TAG_NIVEL2).ToNode();

	if (!(tagNivel2Node)){
		LOGGER_DEBUG("TAG_NIVEL2 no existe");
    	return false;
    }

    //TAG_FONDO1
	TiXmlNode* tagFondo1Node = tagNivel2Node->FirstChildElement(TAG_FONDO1);
	if ((tagFondo1Node)&&(tagNivel2Node->FirstChildElement(TAG_FONDO1)->GetText())) {
		strFondo1.clear();
		strFondo1.append(tagNivel2Node->FirstChildElement(TAG_FONDO1)->GetText());
	    if (!strFondo1.compare(VALUE_EMPTY)) {
	    	LOGGER_DEBUG("TAG_FONDO1 esta vacio");
	    	sucess = false;
	    }

        this->fondo1Nivel2 = strFondo1;
	} else {
		LOGGER_DEBUG("TAG_FONDO1 no existe o tiene valores invalidos");
		sucess = false;
	}

    //TAG_FONDO2
	TiXmlNode* tagFondo2Node = tagNivel2Node->FirstChildElement(TAG_FONDO2);
	if ((tagFondo2Node)&&(tagNivel2Node->FirstChildElement(TAG_FONDO2)->GetText())) {
		strFondo2.clear();
		strFondo2.append(tagNivel2Node->FirstChildElement(TAG_FONDO2)->GetText());
	    if (!strFondo2.compare(VALUE_EMPTY)) {
	    	LOGGER_DEBUG("TAG_FONDO2 esta vacio");
	    	sucess = false;
	    }

        this->fondo2Nivel2 = strFondo2;
	} else {
		LOGGER_DEBUG("TAG_FONDO2 no existe o tiene valores invalidos");
		sucess = false;
	}

    //TAG_FONDO3
	TiXmlNode* tagFondo3Node = tagNivel2Node->FirstChildElement(TAG_FONDO3);
	if ((tagFondo3Node)&&(tagNivel2Node->FirstChildElement(TAG_FONDO3)->GetText())) {
		strFondo3.clear();
		strFondo3.append(tagNivel2Node->FirstChildElement(TAG_FONDO3)->GetText());
	    if (!strFondo3.compare(VALUE_EMPTY)) {
	    	LOGGER_DEBUG("TAG_FONDO3 esta vacio");
	    	sucess = false;
	    }

        this->fondo3Nivel2 = strFondo3;
	} else {
		LOGGER_DEBUG("TAG_FONDO3 no existe o tiene valores invalidos");
		sucess = false;
	}

	return sucess;
}

bool GameParser::evaluateTagNivel3(){
	bool sucess = true;
	string strFondo1;
	string strFondo2;
	string strFondo3;

	TiXmlHandle tiXmlHandle(this->tiXmlFileConfig);
	TiXmlNode* tagNivel3Node = tiXmlHandle.FirstChild(TAG_CONFIGURATION).FirstChild(TAG_ESCENARIOS).FirstChild(TAG_NIVEL3).ToNode();

	if (!(tagNivel3Node)){
		LOGGER_DEBUG("TAG_NIVEL3 no existe");
    	return false;
    }

    //TAG_FONDO1
	TiXmlNode* tagFondo1Node = tagNivel3Node->FirstChildElement(TAG_FONDO1);
	if ((tagFondo1Node)&&(tagNivel3Node->FirstChildElement(TAG_FONDO1)->GetText())) {
		strFondo1.clear();
		strFondo1.append(tagNivel3Node->FirstChildElement(TAG_FONDO1)->GetText());
	    if (!strFondo1.compare(VALUE_EMPTY)) {
	    	LOGGER_DEBUG("TAG_FONDO1 esta vacio");
	    	sucess = false;
	    }

        this->fondo1Nivel3 = strFondo1;
	} else {
		LOGGER_DEBUG("TAG_FONDO1 no existe o tiene valores invalidos");
		sucess = false;
	}

    //TAG_FONDO2
	TiXmlNode* tagFondo2Node = tagNivel3Node->FirstChildElement(TAG_FONDO2);
	if ((tagFondo2Node)&&(tagNivel3Node->FirstChildElement(TAG_FONDO2)->GetText())) {
		strFondo2.clear();
		strFondo2.append(tagNivel3Node->FirstChildElement(TAG_FONDO2)->GetText());
	    if (!strFondo2.compare(VALUE_EMPTY)) {
	    	LOGGER_DEBUG("TAG_FONDO2 esta vacio");
	    	sucess = false;
	    }

        this->fondo2Nivel3 = strFondo2;
        //cout << this->fondo2Nivel3 << endl;
	} else {
		LOGGER_DEBUG("TAG_FONDO2 no existe o tiene valores invalidos");
		sucess = false;
	}

    //TAG_FONDO3
	TiXmlNode* tagFondo3Node = tagNivel3Node->FirstChildElement(TAG_FONDO3);
	if ((tagFondo3Node)&&(tagNivel3Node->FirstChildElement(TAG_FONDO3)->GetText())) {
		strFondo3.clear();
		strFondo3.append(tagNivel3Node->FirstChildElement(TAG_FONDO3)->GetText());
	    if (!strFondo3.compare(VALUE_EMPTY)) {
	    	LOGGER_DEBUG("TAG_FONDO3 esta vacio");
	    	sucess = false;
	    }

        this->fondo3Nivel3 = strFondo3;
	} else {
		LOGGER_DEBUG("TAG_FONDO3 no existe o tiene valores invalidos");
		sucess = false;
	}

	return sucess;
}


bool GameParser::evaluateTagPlataformas(){
	bool sucess = true;
	TiXmlNode* tagIDNode = NULL;
	TiXmlNode* tagTipoNode = NULL;
	TiXmlNode* tagXInicialNode = NULL;
	TiXmlNode* tagXFinalNode = NULL;
	TiXmlNode* tagXAlturaNode = NULL;
	string strID = ZERO;
	string strTipo = VALUE_EMPTY;
	string strXInicial = ZERO;
	string strXFinal = ZERO;
	string strAltura = ZERO;

	PlataformaParser plataformaParser;

	TiXmlHandle tiXmlHandle(this->tiXmlFileConfig);
	TiXmlElement* tagPlataformaElement = tiXmlHandle.FirstChild(TAG_CONFIGURATION).FirstChild(TAG_ESCENARIOS).FirstChild(TAG_NIVEL1).FirstChild(TAG_PLATAFORMAS).FirstChild(TAG_PLATAFORMA).ToElement();

    int cant=1;
	for (tagPlataformaElement; tagPlataformaElement; tagPlataformaElement = tagPlataformaElement->NextSiblingElement()) {
		cout << "La plataforma a evaluar es la nro: ";
		cout << cant << endl;
		sucess = true;

		//TAG_ID
		tagIDNode = tagPlataformaElement->FirstChildElement(TAG_ID);
		if ((tagIDNode)&&(tagPlataformaElement->FirstChildElement(TAG_ID)->GetText())) {
			strID.clear();
			strID.append(tagPlataformaElement->FirstChildElement(TAG_ID)->GetText());
		    if (!strID.compare(ZERO)) {
		    	LOGGER_DEBUG("TAG_ID es cero");
		    	sucess = false;
		    }
	        plataformaParser.setId(atoi(strID.c_str()));
		} else {
			LOGGER_DEBUG("TAG_ID no existe o tiene valores invalidos");
			sucess = false;
		}

		//TAG_TIPO
		tagTipoNode = tagPlataformaElement->FirstChildElement(TAG_TIPO);
		if ((tagTipoNode)&&(tagPlataformaElement->FirstChildElement(TAG_TIPO)->GetText())) {
			strTipo.clear();
			strTipo.append(tagPlataformaElement->FirstChildElement(TAG_TIPO)->GetText());
		    if (!strTipo.compare(VALUE_EMPTY)) {
		    	LOGGER_DEBUG("TAG_TIPO esta vacio");
		    	sucess = false;
		    }
		    plataformaParser.setTipo(strTipo);
		} else {
			LOGGER_DEBUG("TAG_TIPO no existe o tiene valores invalidos");
			sucess = false;
		}

	    //TAG_XINICIAL
		tagXInicialNode = tagPlataformaElement->FirstChildElement(TAG_XINICIAL);
		if ((tagXInicialNode)&&(tagPlataformaElement->FirstChildElement(TAG_XINICIAL)->GetText())) {
			strXInicial.clear();
			strXInicial.append(tagPlataformaElement->FirstChildElement(TAG_XINICIAL)->GetText());
		    if (!strXInicial.compare(ZERO)) {
		    	LOGGER_DEBUG("TAG_XINICIAL es cero");
		    	sucess = false;
		    }
	        plataformaParser.setXInicial(atoi(strXInicial.c_str()));
		} else {
			LOGGER_DEBUG("TAG_XINICIAL no existe o tiene valores invalidos");
			sucess = false;
		}

	    //TAG_XFINAL
		tagXFinalNode = tagPlataformaElement->FirstChildElement(TAG_XFINAL);
		if ((tagXFinalNode)&&(tagPlataformaElement->FirstChildElement(TAG_XFINAL)->GetText())) {
			strXFinal.clear();
			strXFinal.append(tagPlataformaElement->FirstChildElement(TAG_XFINAL)->GetText());
		    if (!strXFinal.compare(ZERO)) {
		    	LOGGER_DEBUG("TAG_XFINAL es cero");
		    	sucess = false;
		    }

	        plataformaParser.setXFinal(atoi(strXFinal.c_str()));
		} else {
			LOGGER_DEBUG("TAG_XFINAL no existe o tiene valores invalidos");
			sucess = false;
		}

	    //TAG_ALTURA
		tagXAlturaNode = tagPlataformaElement->FirstChildElement(TAG_ALTURA);
		if ((tagXAlturaNode)&&(tagPlataformaElement->FirstChildElement(TAG_ALTURA)->GetText())) {
			strAltura.clear();
			strAltura.append(tagPlataformaElement->FirstChildElement(TAG_ALTURA)->GetText());
		    if (!strAltura.compare(ZERO)) {
		    	LOGGER_DEBUG("TAG_ALTURA es cero");
		    	sucess = false;
		    }
		    plataformaParser.setAltura(atoi(strAltura.c_str()));
		} else {
			LOGGER_DEBUG("TAG_ALTURA no existe o tiene valores invalidos");
			sucess = false;
		}

		if (sucess) {
			this->plataformas.push_back(plataformaParser);
	    	cout << "El tipo de la plataforma es: ";
	    	cout << plataformaParser.getTipo() << endl;
			cant++;
		}
	}

	return sucess;
}

bool GameParser::evaluateDataXML (){
	bool sucess = true;

	TiXmlHandle tiXmlHandle(this->tiXmlFileConfig);

	sucess = this->evaluateTagDebug();
	if (sucess) {
		LOGGER_DEBUG("TAG_DEBUG, evaluacion aprobada");
	}

	sucess = this->evaluateTagNivel1();
	if (sucess) {
		LOGGER_DEBUG("TAG_NIVEL1, evaluacion aprobada");
	}

	TiXmlNode* tagPlataforma = tiXmlHandle.FirstChild(TAG_CONFIGURATION).FirstChild(TAG_ESCENARIOS).FirstChild(TAG_NIVEL1).FirstChild(TAG_PLATAFORMAS).FirstChild(TAG_PLATAFORMA).ToNode();
	if (tagPlataforma) {
		sucess = this->evaluateTagPlataformas();
		if (sucess) {
			LOGGER_DEBUG("TAG_PLATAFORMAS, evaluacion aprobada");
		}
	} else {
		LOGGER_DEBUG("TAG_PLATAFORMAS no existe");
    	return false;
	}

	sucess = this->evaluateTagNivel2();
	if (sucess) {
		LOGGER_DEBUG("TAG_NIVEL2, evaluacion aprobada");
	}

	sucess = this->evaluateTagNivel3();
	if (sucess) {
		LOGGER_DEBUG("TAG_NIVEL3, evaluacion aprobada");
	}

	return sucess;
}

bool GameParser::loadConfiguration() {
	bool sucess = true;

	this->tiXmlFileConfig = new TiXmlDocument (Constants::FILE_CONFIGURATION);
	if (this->tiXmlFileConfig->LoadFile()) {
		this->fileConfigLoaded = true;
		evaluateDataXML();
	} else {
		this->tiXmlDefaultFileConfig = new TiXmlDocument (Constants::DEFAULT_FILE_CONFIGURATION);
		if (this->tiXmlDefaultFileConfig->LoadFile()) {
			this->tiXmlFileConfig = this->tiXmlDefaultFileConfig;
			this->defaultfileConfigLoaded = true;
			evaluateDataXML();
		} else {
			LOGGER_INIT(Logger::ERROR);
			LOGGER_ERROR("No hay archivos de configuracion");
			LOGGER_KILL();
			if (this->tiXmlFileConfig) {
				delete this->tiXmlFileConfig;
				this->fileConfigLoaded = false;
			}
			if (this->tiXmlDefaultFileConfig) {
				delete this->tiXmlFileConfig;
				this->defaultfileConfigLoaded = false;
			}
			sucess = false;
		}
	}
	return sucess;
}

GameParser::~GameParser() {
	// TODO Auto-generated destructor stub
	if (this->tiXmlFileConfig) {
		delete this->tiXmlFileConfig;
		this->fileConfigLoaded = false;
	}
	if (this->tiXmlDefaultFileConfig) {
		delete this->tiXmlFileConfig;
		this->defaultfileConfigLoaded = false;
	}
}

const string& GameParser::getFondo1Nivel1() const {
	return fondo1Nivel1;
}

const string& GameParser::getFondo1Nivel2() const {
	return fondo1Nivel2;
}

const string& GameParser::getFondo1Nivel3() const {
	return fondo1Nivel3;
}

const string& GameParser::getFondo2Nivel1() const {
	return fondo2Nivel1;
}

const string& GameParser::getFondo2Nivel2() const {
	return fondo2Nivel2;
}

const string& GameParser::getFondo2Nivel3() const {
	return fondo2Nivel3;
}

const string& GameParser::getFondo3Nivel1() const {
	return fondo3Nivel1;
}

const string& GameParser::getFondo3Nivel2() const {
	return fondo3Nivel2;
}

const string& GameParser::getFondo3Nivel3() const {
	return fondo3Nivel3;
}

const string& GameParser::getLevel() const {
	return level;
}

const std::list<PlataformaParser>& GameParser::getPlataformas() const {
	return plataformas;
}
