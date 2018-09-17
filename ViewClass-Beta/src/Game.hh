/*
 * Game.h
 *
 *  Created on: 02/09/2018
 *      Author: gio
 */
#ifndef GAME_HH_
#define GAME_HH_

#include "Headers.hh"
#include "Level.hh"
#include "Player.hh"
#include "GameParser.hh"

class Game
{
    public:

        Game(GameParser* gameParser, GameView* gameView);
        virtual ~Game();

        void init();
        void handleEvents();
        void update();
        void render();
        void destroy();

        void restartGame();
        void nextLevel();
        void endGame();

        bool state() { return enEjecucion; }

    private:

        bool enEjecucion;

        int currentLevel;

        GameParser* gameParser;
        GameView* gameView;
        Player* player;
        Level* level;
};

#endif /* SRC_GAME_HH_ */
