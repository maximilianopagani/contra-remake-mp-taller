/*
 * Sprite.h
 *
 *  Created on: Sep 11, 2018
 *      Author: giova
 */
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <list>
#include "GameView.h"

using namespace std;

#ifndef SPRITE_H_
#define SPRITE_H_

class Sprite {

public:
	Sprite(GameView* view, std::string imagen, int _destinationWidth, int _destinationHeight, int wCLip, int hClip);
	virtual ~Sprite();
	void render(int x , int y);
	void update();
	void destroy();

private:
	int destinationHeight, destinationWidth, maxWidth;
	GameView* view = NULL;
	SDL_Texture* texture=NULL;
	SDL_Rect src;
	std::string imagenName;
};

#endif /* SPRITE_H_ */