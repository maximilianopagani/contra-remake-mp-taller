/*
 * ClientMessageHandler.hh
 *
 *  Created on: Oct 12, 2018
 *      Author: maximiliano
 */

#ifndef SRC_CLIENTMESSAGEHANDLER_HH_
#define SRC_CLIENTMESSAGEHANDLER_HH_

#include "../../Common/Message.hh"

#include <stdio.h>

#include "View/GameView.hh"
#include "View/PlayerView.hh"
#include "View/LevelView.hh"
#include "View/PlatformView.hh"
#include "View/BulletView.hh"
#include "View/EnemyView.hh"

class ClientMessageHandler
{
	public:

		ClientMessageHandler(GameView* _gameView, PlayerView* _playerView, LevelView* _levelView, PlatformView* _platformView, BulletView* _bulletView, EnemyView* _enemyView);
		virtual ~ClientMessageHandler();
		void redirectRecievedMessage(Message* message);

	private:

		GameView* gameView;
		PlayerView* playerView;
		LevelView* levelView;
		PlatformView* platformView;
		BulletView* bulletView;
		EnemyView* enemyView;
};

#endif /* SRC_CLIENTMESSAGEHANDLER_HH_ */