#ifndef GAMEPARSER_HH_
#define GAMEPARSER_HH_

#include <string>
#include "../data/tinyxml/tinyxml.h"
#include "../data/tinyxml/tinystr.h"
#include "Headers.hh"
#include <cstring>
#include <cstdio>
#include <stdlib.h>
#include "Constants.hh"
#include "Logger.hh"
#include <list>
#include "PlataformParser.hh"

using namespace std;

class GameParser {
private:
	//Important attributes extracted from the xml files
	string level;
	string fondo1Nivel1;
	string fondo2Nivel1;
	string fondo3Nivel1;
	string fondo1Nivel2;
	string fondo2Nivel2;
	string fondo3Nivel2;
	string fondo1Nivel3;
	string fondo2Nivel3;
	string fondo3Nivel3;

	std::list<PlataformParser> plataformas;
	std::list<PlataformParser> plataforms2;
	std::list<PlataformParser> plataforms3;

	//Others attributes
	bool fileConfigLoaded;
	bool defaultfileConfigLoaded;
	TiXmlDocument* tiXmlFileConfig;
	TiXmlDocument* tiXmlDefaultFileConfig;
public:
	GameParser();
	bool evaluateTagDebug();
	bool evaluateTagNivel1();
	bool evaluateTagPlataformas(const char * tagNivel);
	bool evaluateTagPlataforma1Nivel1();
	bool evaluateTagPlataforma2Nivel1();
	bool evaluateTagNivel2();
	bool evaluateTagNivel3();
	bool evaluateTagConfiguration();
	bool evaluateDataXML();
	bool loadConfiguration();
	void testDataParserModel();

	const string& getFondo1Nivel1() const;
	const string& getFondo1Nivel2() const;
	const string& getFondo1Nivel3() const;
	const string& getFondo2Nivel1() const;
	const string& getFondo2Nivel2() const;
	const string& getFondo2Nivel3() const;
	const string& getFondo3Nivel1() const;
	const string& getFondo3Nivel2() const;
	const string& getFondo3Nivel3() const;
	const string& getLevel() const;
	const std::list<PlataformParser>& getPlataformas() const;

	virtual ~GameParser();
	const std::list<PlataformParser>& getPlataforms2() const;
	const std::list<PlataformParser>& getPlataforms3() const;
};

#endif /* GAMEPARSER_HH_ */