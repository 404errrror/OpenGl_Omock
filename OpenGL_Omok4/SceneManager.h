#pragma once
#include "Start.h"
#include "TwoPlayer.h"
#include "AI_Game.h"
#include "ServerGame.h"

GLint sceneNum = 0;
GLint InitNum = 0;

void SceneManager_DoDisplay()
{
	if (sceneNum == 0) {
		StartScene(&sceneNum);
	}
	else if (sceneNum == 1)
		AI_GameScene(&sceneNum);
	else if (sceneNum == 2)
		TwoPlayerScene(&sceneNum);
	else if (sceneNum == 3 || sceneNum == 4)
		ServerGameScene(&sceneNum);

}
