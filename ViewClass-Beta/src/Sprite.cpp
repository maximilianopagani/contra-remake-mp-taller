/*
 * Sprite.cpp
 *
 *  Created on: Sep 11, 2018
 *      Author: giova
 */

#include "Sprite.h"

Sprite::Sprite(GameView* _view, std::string imagen, int wCLip, int hClip, int _destinationWidth, int _destinationHeight) {
	view =_view ;
	destinationWidth = _destinationWidth ;
	destinationHeight = _destinationHeight ;
	// TODO obtener maxWidth y maxHeight con sdl_querytexture
	maxWidth = NULL;
	imagenName = imagen;

	texture = view->textureGenerator(imagenName.c_str());

	src.x = 0;
	src.y = 0;
	src.w = wCLip;
	src.h = hClip;
}

Sprite::~Sprite() {}

void Sprite::render(int x, int y){
//	texture = view->textureGenerator(imagenName.c_str());
	view->draw(texture,&src,x,y,destinationWidth,destinationHeight);

	this->update();
}
void Sprite::update(){
	src.x = src.x + src.w;
	if (src.x == maxWidth) src.x = 0;
}

void Sprite::destroy() {
	//TODO
}
