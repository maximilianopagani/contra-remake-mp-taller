/*
 * Platform.hh
 *
 *  Created on: Sep 16, 2018
 *      Author: pola17
 */

#ifndef PLATFORM_HH_
#define PLATFORM_HH_

#include "../Vista/GameView.hh"
#include "../Vista/Sprite.hh"
#include "../Utils/ICollisional.hh"
#include "CameraLogic.hh"
#include "LogicToViewTransporter.hh"

class Platform : public ICollisional
{
	public:

		//enum GroundType {GRASS, WOOD, ROCK, ICE};

		Platform(GameView* _gameView, CameraLogic* _cameraLogic, LogicToViewTransporter* _logicToViewTransporter, string type, int pos_x, int pos_y, int pixels);
		virtual ~Platform();

		void render();
		int getPosX() { return posX; }
		int getPosY() { return posY; }
		void destroy();

		//Collisional
		int getLeftLimit();
		int getRightLimit();
		int getTopLimit();
		int getBottomLimit();

	private:
		GameView* gameView;
		CameraLogic* cameraLogic;
		LogicToViewTransporter* logicToViewTransporter;
		string type; // probablemente no necesario
//		string path;

		int tileAmount, tileWidth, height; // Se podria hacer tod.o con un solo atributo, platformWidth, pero para el metodo render habria que hacer la query de la textura para obtener
		//su ancho, y loopear con el for hasta i<platformWidth/ancho ó hasta i<plataformWidth;i=i+ancho
//		Sprite* tileSprite;

		int posX, posY;
};

#endif /* PLATFORM_HH_ */
