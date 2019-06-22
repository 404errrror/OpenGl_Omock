#pragma once
#include "Function.h"
#include "OmokFunction.h"

void AI_Game_Init();
void AI_Game_MouseClick(int button, int state, int x, int y);
void AI_Game_DoMouseMotion(GLint x, GLint y);

void AI_Game_Timer(int value);
void AI_Game_TimeLimit();
void AI_Game_PrintTime();
void AI_Game_PersonTrunAnimation();

void AI_Game_StartAnimation();
void AI_Game_EndAnmation();
void AI_Game_MoveSceneAnimation();

void TwoPlayer_TestMode();
Vector2 OmokAI();

// 기초 데이터.
GLint* sceneNumAddr;
Vector2 mousePos;
BoardData boardData;
GLint __isInit = 0;
GLint _animationNum;
Vector2 startPoint;
GLboolean inBoard;
Vector2 winPos[12];
GLboolean isWin;
GLfloat timeLimit; GLfloat nowtime; GLboolean endTime;
Vector2 whiteTimePos; Vector2 blackTimePos;

// 처음 원 효과.
GLfloat blueCirRad;

// 끝났을 때 원효과.
GLfloat endCirRad;

// 끝나는 애니메이션 끝났는지 체크.
GLboolean endAnimationState;

// 처음 씬으로 다시 돌아가는 효과
GLfloat returnCirRad;

// 사람 알파값 이펙트
GLfloat BlackPersonRad; GLfloat WhitePersonRad;

void AI_GameScene(GLint* sceneNum)
{
	if (__isInit == 0)
	{
		AI_Game_Init();
		sceneNumAddr = sceneNum;
	}

	glClearColor(1, 1, 1, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(0.0117, 0.6627, 0.9568);
	DrawCircle(blueCirRad, 0, 0);


	glColor3f(1, 1, 1);

	// 오목판
	DrawBoard(boardData.pos, 1, BOARD_SIZE);
	if (inBoard)
		DrawStone(boardData, mousePos.x, mousePos.y);
	DrawAllStone(boardData);

	// 시간제한 출력
	AI_Game_PrintTime();

	//사람 캐릭터 출력
	_LoadAlphaImage("GameResources/BlackCharacter.bmp", "GameResources/BlackCharacterMask.bmp", 0.55, 0.5, 1 * 0.4, 1 * 1.78* 0.4);
	ClippingMaskCir(blueCirRad, 0, 0);
	_LoadAlphaImage("GameResources/AI_Game.bmp", "GameResources/AI_GameMask.bmp", 0.55, -0.5, 1 * 0.45, 1 * 1.78 * 0.45);
	glDisable(GL_STENCIL_TEST);

	// 사람 캐릭터 순서 이펙트
	if (isWin == 0) {
		glColor4f(0, 0, 0, 0.1);
		ClippingMaskRect(0.35, 0.858333, 0.75, 0.141667);
		DrawCircle(BlackPersonRad, 0.55, 0.5);
		glDisable(GL_STENCIL_TEST);
		ClippingMaskRect(0.35, -0.141667, 0.75, -0.858333);
		DrawCircle(WhitePersonRad, 0.55, -0.5);
		glDisable(GL_STENCIL_TEST);
	}


	// 이겼을 때
	if (isWin)
	{
		if (endTime)
		{
			ClippingMaskRectReverse(0.5 - 0.01, 0.06, 0.5 + 0.11, -0.03);

			// 이긴 캐릭터 쪽에는 알파값이 안들어가도록.
			if (boardData.nowTurn == BLACK)
				ClippingMaskRectReverse(0.35, -0.101667, 0.75, -0.898333);
			else if (boardData.nowTurn == WHITE)
				ClippingMaskRectReverse(0.35, 0.858333, 0.75, 0.141667);
		}
		// 타임오버로 끝난게 아닐땐 이긴 오목알에 클리핑 마스크 생성.
		else
			AI_WinStencilEffect(boardData, winPos);



		glColor4f(0, 0, 0, 0.5);
		DrawCircle(endCirRad, 0, 0);
		glDisable(GL_STENCIL_TEST);

		// 클리핑 된 곳이 텍스트에 가리면 텍스트가 잘 안보여서 약간 알파값 조정.
		if (!endTime)
		{
			ClippingMaskCir(endCirRad, 0, 0);
			glColor4f(0, 0, 0, 0.25);
			for (int i = 0; winPos[i].x != -300; i++)
				DrawCircle(boardData.scale * 0.5 / (boardData.amount - 1), winPos[i].x, winPos[i].y);
			glDisable(GL_STENCIL_TEST);
		}

		ClippingMaskCir(endCirRad, 0, 0);
		//glColor4f(0, 0, 0, 0.2);
		//glRectf(-0.620313, 0.697222, 0.020313, 0.5);

		//시간으로 이겼든, 오목으로 이겼든 현재 턴인 사람은 진 것.
		if (boardData.nowTurn == BLACK)
			_LoadAlphaImage("GameResources/WhiteStoneWin.bmp", "GameResources/WhiteStoneWinMask.bmp", -0.3, 0.6, 1 * 0.7, 0.20 * 0.7);
		else if (boardData.nowTurn == WHITE)
			_LoadAlphaImage("GameResources/BlackStoneWin.bmp", "GameResources/BlackStoneWinMask.bmp", -0.3, 0.6, 1 * 0.7, 0.20 * 0.7);

		//glColor4f(0, 0, 0, 0.2f);
		//glRectf(-0.620313, 0.113889, 0.020313, -0.113889);
		_LoadAlphaImage("GameResources/PressAnyKey.bmp", "GameResources/PressAnyKeyMask.bmp", -0.3, 0, 1 * 0.7, 0.45 * 0.7);

		glDisable(GL_STENCIL_TEST);
	}

	ClippingMaskCirReverse(returnCirRad, 0, 0);
	glColor3f(1, 1, 1);
	glRectf(-1.5, 1.5, 1.5, -1.5);
	glDisable(GL_STENCIL_TEST);


	//TwoPlayer_TestMode();

	glutSwapBuffers();
}



void AI_Game_Init()
{
	__isInit = 1;
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);	 // MIN : 축소할때, NEAREST : 포토샵 최단입단 같은 효과, LINEAR : 포토샵 같은 부드럽게 효과.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glutMouseFunc(AI_Game_MouseClick);
	glutPassiveMotionFunc(AI_Game_DoMouseMotion);
	glutTimerFunc(1, AI_Game_Timer, 1);

	boardData.amount = BOARD_SIZE;
	boardData.pos.x = -0.3; boardData.pos.y = 0;
	boardData.scale = 1; boardData.nowTurn = BLACK;
	for (int i = 0; i < BOARD_SIZE; i++)
		for (int j = 0; j < BOARD_SIZE; j++)
			boardData.boardArr[i][j] = EMPTY;

	for (int i = 0; i < sizeof(winPos) / sizeof(winPos[1]);i++)
	{
		winPos[i].x = -300;
		winPos[i].y = -300;
	}

	_animationNum = 0;
	startPoint.x = boardData.pos.x - boardData.scale * 0.5;
	startPoint.y = (boardData.pos.y + boardData.scale * 0.5) * 1.78;
	inBoard = 0; isWin = 0;
	timeLimit = clock() + 20000; nowtime = clock(); endTime = 0;
	whiteTimePos.x = 0.5; whiteTimePos.y = 0; blackTimePos.x = 0.5; blackTimePos.y = 0;

	blueCirRad = 0;
	endCirRad = 0;
	returnCirRad = 1.2;

	BlackPersonRad = WhitePersonRad = 0;
}

//////////////////////////////////////// 마우스 /////////////////////////////////////////

void AI_Game_MouseClick(GLint button, GLint state, GLint x, GLint y)
{


	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && !isWin)
	{
		GLint isPut;
		Vector2 tempVec = CallibratorToArray(boardData, mousePos.x, mousePos.y);
		if (inBoard == 1)
			isPut = PutStone(&boardData, &mousePos.x, &mousePos.y);

		if (Win_Check(boardData, tempVec.x, tempVec.y, winPos))
		{
			isWin = 1;
			for (int i = 0;  winPos[i].x != -300; i++)
				winPos[i] = CallibratorToBoard(boardData, winPos[i].x, winPos[i].y);

		}
		else if (inBoard == 1)
		{
			timeLimit = clock() + 20000;
		}

		if (boardData.nowTurn == WHITE)
		{
			Vector2 aiPos;
			aiPos = OmokAI();
			PutStoneNoCali(&boardData,aiPos.x, aiPos.y);
			if (Win_Check(boardData, aiPos.x, aiPos.y, winPos))
			{
				isWin = 1;
				for (int i = 0;  winPos[i].x != -300; i++)
					winPos[i] = CallibratorToBoard(boardData, winPos[i].x, winPos[i].y);

			}
		}
	}

	else if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && isWin && _animationNum == 2)
		_animationNum = 3;

	glutPostRedisplay();
}

void AI_Game_DoMouseMotion(GLint x, GLint y)
{
	mousePos.x = x / (1280 * 0.5) - 1;
	mousePos.y = (y / (720 * 0.5) - 1) * -1;



	if (mousePos.x > startPoint.x && mousePos.x < startPoint.x + boardData.scale
		&& mousePos.y < startPoint.y && mousePos.y > startPoint.y - boardData.scale * 1.78)
		inBoard = 1;
	else
		inBoard = 0;
	glutPostRedisplay();
}




//////////////////////////////////////// 타이머 ////////////////////////////////////////
void AI_Game_Timer(int value)
{
	if (_animationNum == 0)
		AI_Game_StartAnimation();
	else if (_animationNum == 3)
		AI_Game_MoveSceneAnimation();

	if (*sceneNumAddr != 1)
	{
		__isInit = 0;
		glutPostRedisplay();
		return;
	}

	if (isWin == 1 && _animationNum != 3)
		AI_Game_EndAnmation();
	else if (endTime != 1)
		AI_Game_TimeLimit();

	if (isWin != 1 && blueCirRad >= 1.1)
		AI_Game_PersonTrunAnimation();

	glutPostRedisplay();
	glutTimerFunc(1, AI_Game_Timer, 1);
}

void AI_Game_TimeLimit()
{

	nowtime = clock();
	if (timeLimit - nowtime < 0)
	{
		endTime = 1;	isWin = 1;
	}

}

void AI_Game_PrintTime()
{
	char timeChar[12];
	sprintf(timeChar, "Time : %d", (GLint)(timeLimit - nowtime) / 1000);

	int length = (int)strlen(timeChar);

	glColor3f(1, 1, 1);
	glRasterPos2f(0.5, 0);

	for (int i = 0; i < length; i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, timeChar[i]);
	glFlush();



}

void AI_Game_StartAnimation()
{
	SmoothEndReshapeCirlceAnimation(&blueCirRad, 1.2, 1);
}

void AI_Game_EndAnmation()
{
	SmoothEndReshapeCirlceAnimation(&endCirRad, 1.2, 1);
	if (endCirRad >= 1.15)
		_animationNum = 2;
}

void AI_Game_PersonTrunAnimation()
{
	if (boardData.nowTurn == BLACK) {
		SmoothEndReshapeCirlceAnimation(&BlackPersonRad, 0.29, 2);
		SmoothEndReshapeCirlceAnimation(&WhitePersonRad, 0, 1);
	}
	else if (boardData.nowTurn == WHITE)
	{
		SmoothEndReshapeCirlceAnimation(&WhitePersonRad, 0.29, 2);
		SmoothEndReshapeCirlceAnimation(&BlackPersonRad, 0, 1);

	}
}


void AI_Game_MoveSceneAnimation()
{
	SmoothEndReshapeCirlceAnimation(&returnCirRad, 0, 3);
	if (returnCirRad <= 0.001)
		*sceneNumAddr = 0;
}

////////////////////////////////////////////////////////////////////////////////////////
#define data boardData.boardArr


Vector2 OmokAI() {
	int posX = -1;
	int posY = -1;
	int important = 0;

	for (int i = 0; i < 19; i++) {
		for (int j = 0; j < 19; j++) {
			//오목 상황 검색.
			//수비(일반적인 상황)
			//3줄막기
			if (data[i][j] == BLACK &&  data[i][j] == data[i + 1][j] && data[i][j] == data[i + 2][j] && data[i + 3][j] == EMPTY && data[i - 1][j] == EMPTY) {
				if (important < 6) {
					important = 6;
					posX = i + 3;
					posY = j;
				}
			}
			else if (data[i][j] == BLACK &&  data[i][j] == data[i][j + 1] && data[i][j] == data[i][j + 2] && data[i][j + 3] == EMPTY && data[i][j - 1] == EMPTY) {
				if (important < 6) {
					important = 6;
					posX = i;
					posY = j + 3;
				}
			}
			else if (data[i][j] == BLACK &&  data[i][j] == data[i + 1][j - 1] && data[i][j] == data[i + 2][j - 2] && data[i + 3][j - 3] == EMPTY && data[i - 1][j + 1] == EMPTY) {
				if (important < 6) {
					important = 6;
					posX = i + 3;
					posY = j - 3;
				}
			}
			else if (data[i][j] == BLACK &&  data[i][j] == data[i - 1][j - 1] && data[i][j] == data[i - 2][j - 2] && data[i - 3][j - 3] == EMPTY && data[i + 1][j + 1] == EMPTY) {
				if (important < 6) {
					important = 6;
					posX = i - 3;
					posY = j - 3;
				}
			}


			//4줄막기
			if (data[i][j] == BLACK && data[i][j] == data[i + 1][j] && data[i][j] == data[i + 2][j] && data[i][j] == data[i + 3][j] && data[i - 1][j] == EMPTY) {
				if (important < 10) {
					important = 10;
					posX = i - 1;
					posY = j;
				}
			}
			else if (data[i][j] == BLACK && data[i][j] == data[i][j + 1] && data[i][j] == data[i][j + 2] && data[i][j] == data[i][j + 3] && data[i][j - 1] == EMPTY) {
				if (important < 10) {
					important = 10;
					posX = i;
					posY = j - 1;
				}
			}
			else if (data[i][j] == BLACK && data[i][j] == data[i + 1][j - 1] && data[i][j] == data[i + 2][j - 2] && data[i][j] == data[i + 3][j - 3] && data[i - 1][j + 1] == EMPTY) {
				if (important < 10) {
					important = 10;
					posX = i - 1;
					posY = j + 1;
				}
			}
			else if (data[i][j] == BLACK && data[i][j] == data[i - 1][j - 1] && data[i][j] == data[i - 2][j - 2] && data[i][j] == data[i - 3][j - 3] && data[i + 1][j + 1] == EMPTY) {
				if (important < 10) {
					important = 10;
					posX = i + 1;
					posY = j + 1;
				}
			}
			//////////////
			else if (data[i][j] == BLACK && data[i][j] == data[i + 1][j] && data[i][j] == data[i + 2][j] && data[i][j] == data[i + 3][j] && data[i + 4][j] == EMPTY) {
				if (important < 10) {
					important = 10;
					posX = i + 4;
					posY = j;
				}
			}
			else if (data[i][j] == BLACK && data[i][j] == data[i][j + 1] && data[i][j] == data[i][j + 2] && data[i][j] == data[i][j + 3] && data[i][j + 4] == EMPTY) {
				if (important < 10) {
					important = 10;
					posX = i;
					posY = j + 4;
				}
			}
			else if (data[i][j] == BLACK && data[i][j] == data[i + 1][j - 1] && data[i][j] == data[i + 2][j - 2] && data[i][j] == data[i + 3][j - 3] && data[i + 4][j - 4] == EMPTY) {
				if (important < 10) {
					important = 10;
					posX = i + 4;
					posY = j - 4;
				}
			}
			else if (data[i][j] == BLACK && data[i][j] == data[i - 1][j - 1] && data[i][j] == data[i - 2][j - 2] && data[i][j] == data[i - 3][j - 3] && data[i - 4][j - 4] == EMPTY) {
				if (important < 10) {
					important = 10;
					posX = i - 4;
					posY = j - 4;
				}
			}

			//수비(띄움 수 막기)
			else if (data[i][j] == BLACK && data[i][j] == data[i][j + 2] && data[i][j] == data[i][j + 3] && data[i][j + 1] == EMPTY) {
				if (important < 3) {
					important = 3;
					posX = i;
					posY = j + 1;
				}
			}
			else if (data[i][j] == BLACK && data[i][j] == data[i + 2][j] && data[i][j] == data[i + 3][j] && data[i + 1][j] == EMPTY) {
				if (important < 3) {
					important = 3;
					posX = i + 1;
					posY = j;
				}
			}
			else if (data[i][j] == BLACK && data[i][j] == data[i + 2][j - 2] && data[i][j] == data[i + 3][j - 3] && data[i + 1][j - 1] == EMPTY) {
				if (important < 3) {
					important = 3;
					posX = i + 1;
					posY = j - 1;
				}
			}
			else if (data[i][j] == BLACK && data[i][j] == data[i - 2][j - 2] && data[i][j] == data[i - 3][j - 3] && data[i - 1][j - 1] == EMPTY) {
				if (important < 3) {
					important = 3;
					posX = i - 1;
					posY = j - 1;
				}
			}

			//33 43막기
			if (data[i][j] == BLACK && data[i][j] == data[i + 1][j] && data[i - 1][j] == EMPTY) {// ㅡ
				if (data[i][j] == data[i][j - 1] && data[i][j] == data[i + 1][j - 2]// ↗
					|| data[i][j] == data[i - 1][j - 1] && data[i][j] == data[i - 1][j - 2]// ↑
					|| data[i][j] == data[i - 2][j - 1] && data[i][j] == data[i - 3][j - 2] // ↖
					|| data[i][j] == data[i - 1][j] && data[i][j] == data[i - 2][j]   // ←
					|| data[i][j] == data[i - 2][j + 1] && data[i][j] == data[i - 3][j + 2]   // ↙
					|| data[i][j] == data[i - 1][j + 1] && data[i][j] == data[i - 1][j + 2]   // ↓
					|| data[i][j] == data[i][j + 1] && data[i][j] == data[i + 1][j + 2]   // ↘
					|| data[i][j] == data[i][j - 1] && data[i][j] == data[i - 2][j + 1] // /ㅡ
					|| data[i][j] == data[i][j + 1] && data[i][j] == data[i - 2][j - 1] // \ㅡ
					) {
					if (important < 7) {
						important = 7;
						posX = i - 1;
						posY = j;
					}
				}
			}
			if (data[i][j] == BLACK && data[i][j] == data[i + 1][j] && data[i + 2][j] == EMPTY) {
				if (data[i][j] == data[i + 3][j - 1] && data[i][j] == data[i + 4][j - 2]// ↗
					|| data[i][j] == data[i + 2][j - 1] && data[i][j] == data[i + 2][j - 2]// ↑
					|| data[i][j] == data[i + 1][j - 1] && data[i][j] == data[i][j - 2] // ↖
					|| data[i][j] == data[i + 1][j + 1] && data[i][j] == data[i][j + 2]   // ↙
					|| data[i][j] == data[i + 2][j + 1] && data[i][j] == data[i + 2][j + 2]   // ↓
					|| data[i][j] == data[i + 3][j + 1] && data[i][j] == data[i + 4][j + 2]   // ↘
					|| data[i][j] == data[i + 3][j + 1] && data[i][j] == data[i + 1][j - 1] // ㅡ/
					|| data[i][j] == data[i + 1][j + 1] && data[i][j] == data[i + 3][j - 1] // ㅡ\    .
					) {
					if (important < 7) {
						important = 7;
						posX = i + 2;
						posY = j;
					}
				}
			}

			if (data[i][j] == BLACK && data[i][j] == data[i + 1][j + 1] && data[i + 2][j + 2] == EMPTY) {
				if (data[i][j] == data[i + 3][j + 1] && data[i][j] == data[i + 4][j]
					|| data[i][j] == data[i + 3][j + 3] && data[i][j] == data[i + 4][j + 4]
					|| data[i][j] == data[i + 1][j + 3] && data[i][j] == data[i][j + 4]) {
					if (important < 7) {
						important = 7;
						posX = i + 2;
						posY = j + 2;
					}
				}

			}


			if (data[i][j] == BLACK && data[i][j] == data[i][j + 1] && data[i][j - 1] == EMPTY) {//l
				if (data[i][j] == data[i + 1][j - 1] && data[i][j] == data[i - 1][j - 1]// - -
					|| data[i][j] == data[i - 1][j] && data[i][j] == data[i + 1][j - 2]
					|| data[i][j] == data[i + 1][j] && data[i][j] == data[i - 1][j - 2]

					|| data[i][j] == data[i + 1][j - 2] && data[i][j] == data[i + 2][j - 3]
					|| data[i][j] == data[i][j - 2] && data[i][j] == data[i][j - 3]
					|| data[i][j] == data[i - 1][j - 2] && data[i][j] == data[i - 2][j - 3]
					|| data[i][j] == data[i - 1][j - 1] && data[i][j] == data[i - 2][j - 1]
					|| data[i][j] == data[i - 1][j] && data[i][j] == data[i - 2][j + 1]
					|| data[i][j] == data[i + 1][j] && data[i][j] == data[i + 2][j + 1]
					|| data[i][j] == data[i + 1][j - 1] && data[i][j] == data[i + 2][j - 1]
					) {
					if (important < 7) {
						important = 7;
						posX = i;
						posY = j - 1;
					}
				}
			}
			if (data[i][j] == BLACK && data[i][j] == data[i][j - 1] && data[i][j + 1] == EMPTY) {//l
				if (data[i][j] == data[i + 1][j + 1] && data[i][j] == data[i - 1][j + 1]// - -
					|| data[i][j] == data[i - 1][j + 2] && data[i][j] == data[i + 1][j]
					|| data[i][j] == data[i + 1][j + 2] && data[i][j] == data[i - 1][j]

					|| data[i][j] == data[i + 1][j + 2] && data[i][j] == data[i + 2][j + 3]
					|| data[i][j] == data[i][j + 2] && data[i][j] == data[i][j + 3]
					|| data[i][j] == data[i - 1][j + 2] && data[i][j] == data[i - 2][j + 3]
					|| data[i][j] == data[i - 1][j + 1] && data[i][j] == data[i - 2][j + 1]
					|| data[i][j] == data[i - 1][j] && data[i][j] == data[i - 2][j - 1]
					|| data[i][j] == data[i + 1][j] && data[i][j] == data[i + 2][j - 1]
					|| data[i][j] == data[i + 1][j + 1] && data[i][j] == data[i + 2][j + 1]
					) {
					if (important < 7) {
						important = 7;
						posX = i;
						posY = j + 1;
					}
				}
			}




			// 뚫린 33 막아야함

			if (data[i][j] == BLACK && data[i][j] == data[i][j + 2] && data[i][j + 1] == EMPTY) {
				if (data[i][j] == data[i + 1][j] && data[i][j] == data[i - 1][j + 2]
					|| data[i][j] == data[i + 1][j + 1] || data[i][j] == data[i - 1][j + 1]
					|| data[i][j] == data[i - 1][j] && data[i][j] == data[i + 1][j + 2]) {
					if (important < 7) {
						important = 7;
						posX = i;
						posY = j + 1;
					}
				}

			}

			// 31 13막기

			if (data[i][j] == BLACK && data[i][j] == data[i + 1][j] && data[i][j] == data[i + 2][j] && data[i][j] == data[i + 4][j] && data[i + 3][j] == EMPTY) {
				if (important < 9) {
					important = 9;
					posX = i + 3;
					posY = j;
				}
			}

			else if (data[i][j] == BLACK && data[i][j] == data[i - 1][j] && data[i][j] == data[i - 2][j] && data[i][j] == data[i - 4][j] && data[i - 3][j] == EMPTY) {
				if (important < 9) {
					important = 9;
					posX = i - 3;
					posY = j;
				}
			}

			else if (data[i][j] == BLACK && data[i][j] == data[i][j + 1] && data[i][j] == data[i][j + 2] && data[i][j] == data[i][j + 4] && data[i][j + 3] == EMPTY) {
				if (important < 9) {
					important = 9;
					posX = i;
					posY = j + 3;
				}
			}


			// 2 1막기
			if (data[i][j] == BLACK && data[i][j] == data[i + 1][j]) {
				if (data[i][j] == data[i - 2][j] && data[i - 1][j] == EMPTY) {
					if (important < 8) {
						important = 8;
						posX = i - 1;
						posY = j;
					}
				}
				else if (data[i][j] == data[i + 3][j] && data[i + 2][j] == EMPTY) {
					if (important < 8) {
						important = 8;
						posX = i + 2;
						posY = j;
					}
				}
			}

			else if (data[i][j] == BLACK && data[i][j] == data[i][j + 1]) {
				if (data[i][j] == data[i][j - 2] && data[i][j - 1] == EMPTY) {
					if (important < 8) {
						important = 8;
						posX = i;
						posY = j - 1;
					}
				}
				else if (data[i][j] == data[i][j + 3] && data[i][j + 2] == EMPTY) {
					if (important < 8) {
						important = 8;
						posX = i;
						posY = j + 2;
					}
				}
			}



		}
	}

	for (int i = 0; i < 19; i++) {
		for (int j = 0; j < 19; j++) {
			//오목 상황 검색.
			//공격(일반적인 상황)
			//3줄막기

			if (data[i][j] == WHITE &&  data[i][j] == data[i + 1][j] && data[i][j] == data[i + 2][j] && data[i + 3][j] == EMPTY && data[i - 1][j] == EMPTY) {
				if (important < 6) {
					important = 6;
					posX = i + 3;
					posY = j;
				}
			}
			else if (data[i][j] == WHITE &&  data[i][j] == data[i][j + 1] && data[i][j] == data[i][j + 2] && data[i][j + 3] == EMPTY && data[i][j - 1] == EMPTY) {
				if (important < 6) {
					important = 6;
					posX = i;
					posY = j + 3;
				}
			}
			else if (data[i][j] == WHITE &&  data[i][j] == data[i + 1][j - 1] && data[i][j] == data[i + 2][j - 2] && data[i + 3][j - 3] == EMPTY && data[i - 1][j + 1] == EMPTY) {
				if (important < 6) {
					important = 6;
					posX = i + 3;
					posY = j - 3;
				}
			}
			else if (data[i][j] == WHITE &&  data[i][j] == data[i - 1][j - 1] && data[i][j] == data[i - 2][j - 2] && data[i - 3][j - 3] == EMPTY && data[i + 1][j + 1] == EMPTY) {
				if (important < 6) {
					important = 6;
					posX = i - 3;
					posY = j - 3;
				}
			}



			//4줄막기
			if (data[i][j] == WHITE && data[i][j] == data[i + 1][j] && data[i][j] == data[i + 2][j] && data[i][j] == data[i + 3][j] && data[i - 1][j] == EMPTY) {
				if (important < 10) {
					important = 10;
					posX = i - 1;
					posY = j;
				}
			}
			else if (data[i][j] == WHITE && data[i][j] == data[i][j + 1] && data[i][j] == data[i][j + 2] && data[i][j] == data[i][j + 3] && data[i][j - 1] == EMPTY) {
				if (important < 10) {
					important = 10;
					posX = i;
					posY = j - 1;
				}
			}
			else if (data[i][j] == WHITE && data[i][j] == data[i + 1][j - 1] && data[i][j] == data[i + 2][j - 2] && data[i][j] == data[i + 3][j - 3] && data[i - 1][j + 1] == EMPTY) {
				if (important < 10) {
					important = 10;
					posX = i - 1;
					posY = j + 1;
				}
			}
			else if (data[i][j] == WHITE && data[i][j] == data[i - 1][j - 1] && data[i][j] == data[i - 2][j - 2] && data[i][j] == data[i - 3][j - 3] && data[i + 1][j + 1] == EMPTY) {
				if (important < 10) {
					important = 10;
					posX = i + 1;
					posY = j + 1;
				}
			}
			//////////////
			else if (data[i][j] == WHITE && data[i][j] == data[i + 1][j] && data[i][j] == data[i + 2][j] && data[i][j] == data[i + 3][j] && data[i + 4][j] == EMPTY) {
				if (important < 10) {
					important = 10;
					posX = i + 4;
					posY = j;
				}
			}
			else if (data[i][j] == WHITE && data[i][j] == data[i][j + 1] && data[i][j] == data[i][j + 2] && data[i][j] == data[i][j + 3] && data[i][j + 4] == EMPTY) {
				if (important < 10) {
					important = 10;
					posX = i;
					posY = j + 4;
				}
			}
			else if (data[i][j] == WHITE && data[i][j] == data[i + 1][j - 1] && data[i][j] == data[i + 2][j - 2] && data[i][j] == data[i + 3][j - 3] && data[i + 4][j - 4] == EMPTY) {
				if (important < 10) {
					important = 10;
					posX = i + 4;
					posY = j - 4;
				}
			}
			else if (data[i][j] == WHITE && data[i][j] == data[i - 1][j - 1] && data[i][j] == data[i - 2][j - 2] && data[i][j] == data[i - 3][j - 3] && data[i - 4][j - 4] == EMPTY) {
				if (important < 10) {
					important = 10;
					posX = i - 4;
					posY = j - 4;
				}
			}

			//수비(띄움 수 막기)
			else if (data[i][j] == WHITE && data[i][j] == data[i][j + 2] && data[i][j] == data[i][j + 3] && data[i][j + 1] == EMPTY) {
				if (important < 3) {
					important = 3;
					posX = i;
					posY = j + 1;
				}
			}
			else if (data[i][j] == WHITE && data[i][j] == data[i + 2][j] && data[i][j] == data[i + 3][j] && data[i + 1][j] == EMPTY) {
				if (important < 3) {
					important = 3;
					posX = i + 1;
					posY = j;
				}
			}
			else if (data[i][j] == WHITE && data[i][j] == data[i + 2][j - 2] && data[i][j] == data[i + 3][j - 3] && data[i + 1][j - 1] == EMPTY) {
				if (important < 3) {
					important = 3;
					posX = i + 1;
					posY = j - 1;
				}
			}
			else if (data[i][j] == WHITE && data[i][j] == data[i - 2][j - 2] && data[i][j] == data[i - 3][j - 3] && data[i - 1][j - 1] == EMPTY) {
				if (important < 3) {
					important = 3;
					posX = i - 1;
					posY = j - 1;
				}
			}

			//33 43막기
			if (data[i][j] == WHITE && data[i][j] == data[i + 1][j] && data[i - 1][j] == EMPTY) {// ㅡ
				if (data[i][j] == data[i][j - 1] && data[i][j] == data[i + 1][j - 2]// ↗
					|| data[i][j] == data[i - 1][j - 1] && data[i][j] == data[i - 1][j - 2]// ↑
					|| data[i][j] == data[i - 2][j - 1] && data[i][j] == data[i - 3][j - 2] // ↖
					|| data[i][j] == data[i - 1][j] && data[i][j] == data[i - 2][j]   // ←
					|| data[i][j] == data[i - 2][j + 1] && data[i][j] == data[i - 3][j + 2]   // ↙
					|| data[i][j] == data[i - 1][j + 1] && data[i][j] == data[i - 1][j + 2]   // ↓
					|| data[i][j] == data[i][j + 1] && data[i][j] == data[i + 1][j + 2]   // ↘
					|| data[i][j] == data[i][j - 1] && data[i][j] == data[i - 2][j + 1] // /ㅡ
					|| data[i][j] == data[i][j + 1] && data[i][j] == data[i - 2][j - 1] // \ㅡ
					) {
					if (important < 6) {
						important = 6;
						posX = i - 1;
						posY = j;
					}
				}
			}
			if (data[i][j] == WHITE && data[i][j] == data[i + 1][j] && data[i + 2][j] == EMPTY) {
				if (data[i][j] == data[i + 3][j - 1] && data[i][j] == data[i + 4][j - 2]// ↗
					|| data[i][j] == data[i + 2][j - 1] && data[i][j] == data[i + 2][j - 2]// ↑
					|| data[i][j] == data[i + 1][j - 1] && data[i][j] == data[i][j - 2] // ↖
					|| data[i][j] == data[i + 1][j + 1] && data[i][j] == data[i][j + 2]   // ↙
					|| data[i][j] == data[i + 2][j + 1] && data[i][j] == data[i + 2][j + 2]   // ↓
					|| data[i][j] == data[i + 3][j + 1] && data[i][j] == data[i + 4][j + 2]   // ↘
					|| data[i][j] == data[i + 3][j + 1] && data[i][j] == data[i + 1][j - 1] // ㅡ/
					|| data[i][j] == data[i + 1][j + 1] && data[i][j] == data[i + 3][j - 1] // ㅡ\    .
					) {
					if (important < 6) {
						important = 6;
						posX = i + 2;
						posY = j;
					}
				}
			}

			if (data[i][j] == WHITE && data[i][j] == data[i + 1][j + 1] && data[i + 2][j + 2] == EMPTY) {
				if (data[i][j] == data[i + 3][j + 1] && data[i][j] == data[i + 4][j]
					|| data[i][j] == data[i + 3][j + 3] && data[i][j] == data[i + 4][j + 4]
					|| data[i][j] == data[i + 1][j + 3] && data[i][j] == data[i][j + 4]) {
					if (important < 5) {
						important = 5;
						posX = i + 2;
						posY = j + 2;
					}
				}

			}

			if (data[i][j] == WHITE && data[i][j] == data[i][j + 1] && data[i][j - 1] == EMPTY) {//l
				if (data[i][j] == data[i + 1][j - 1] && data[i][j] == data[i - 1][j - 1]// - -
					|| data[i][j] == data[i - 1][j] && data[i][j] == data[i + 1][j - 2]
					|| data[i][j] == data[i + 1][j] && data[i][j] == data[i - 1][j - 2]

					|| data[i][j] == data[i + 1][j - 2] && data[i][j] == data[i + 2][j - 3]
					|| data[i][j] == data[i][j - 2] && data[i][j] == data[i][j - 3]
					|| data[i][j] == data[i - 1][j - 2] && data[i][j] == data[i - 2][j - 3]
					|| data[i][j] == data[i - 1][j - 1] && data[i][j] == data[i - 2][j - 1]
					|| data[i][j] == data[i - 1][j] && data[i][j] == data[i - 2][j + 1]
					|| data[i][j] == data[i + 1][j] && data[i][j] == data[i + 2][j + 1]
					|| data[i][j] == data[i + 1][j - 1] && data[i][j] == data[i + 2][j - 1]
					) {
					if (important < 6) {
						important = 6;
						posX = i;
						posY = j - 1;
					}
				}
			}
			if (data[i][j] == WHITE && data[i][j] == data[i][j - 1] && data[i][j + 1] == EMPTY) {//l
				if (data[i][j] == data[i + 1][j + 1] && data[i][j] == data[i - 1][j + 1]// - -
					|| data[i][j] == data[i - 1][j + 2] && data[i][j] == data[i + 1][j]
					|| data[i][j] == data[i + 1][j + 2] && data[i][j] == data[i - 1][j]

					|| data[i][j] == data[i + 1][j + 2] && data[i][j] == data[i + 2][j + 3]
					|| data[i][j] == data[i][j + 2] && data[i][j] == data[i][j + 3]
					|| data[i][j] == data[i - 1][j + 2] && data[i][j] == data[i - 2][j + 3]
					|| data[i][j] == data[i - 1][j + 1] && data[i][j] == data[i - 2][j + 1]
					|| data[i][j] == data[i - 1][j] && data[i][j] == data[i - 2][j - 1]
					|| data[i][j] == data[i + 1][j] && data[i][j] == data[i + 2][j - 1]
					|| data[i][j] == data[i + 1][j + 1] && data[i][j] == data[i + 2][j + 1]
					) {
					if (important < 6) {
						important = 6;
						posX = i;
						posY = j + 1;
					}
				}
			}


			// 뚫린 33 막아야함

			if (data[i][j] == WHITE && data[i][j] == data[i][j + 2] && data[i][j + 1] == EMPTY) {
				if (data[i][j] == data[i + 1][j] && data[i][j] == data[i - 1][j + 2]
					|| data[i][j] == data[i + 1][j + 1] || data[i][j] == data[i - 1][j + 1]
					|| data[i][j] == data[i - 1][j] && data[i][j] == data[i + 1][j + 2]) {
					if (important < 5) {
						important = 5;
						posX = i;
						posY = j + 1;
					}
				}

			}

			// 31 13막기

			if (data[i][j] == WHITE && data[i][j] == data[i + 1][j] && data[i][j] == data[i + 2][j] && data[i][j] == data[i + 4][j] && data[i + 3][j] == EMPTY) {
				if (important < 9) {
					important = 9;
					posX = i + 3;
					posY = j;
				}
			}

			else if (data[i][j] == WHITE && data[i][j] == data[i - 1][j] && data[i][j] == data[i - 2][j] && data[i][j] == data[i - 4][j] && data[i - 3][j] == EMPTY) {
				if (important < 9) {
					important = 9;
					posX = i - 3;
					posY = j;
				}
			}

			else if (data[i][j] == WHITE && data[i][j] == data[i][j + 1] && data[i][j] == data[i][j + 2] && data[i][j] == data[i][j + 4] && data[i][j + 3] == EMPTY) {
				if (important < 9) {
					important = 9;
					posX = i;
					posY = j + 3;
				}
			}

			// 2 1막기
			if (data[i][j] == WHITE && data[i][j] == data[i + 1][j]) {
				if (data[i][j] == data[i - 2][j] && data[i - 1][j] == EMPTY) {
					if (important < 8) {
						important = 8;
						posX = i - 1;
						posY = j;
					}
				}
				else if (data[i][j] == data[i + 3][j] && data[i + 2][j] == EMPTY) {
					if (important < 8) {
						important = 8;
						posX = i + 2;
						posY = j;
					}
				}
			}

			else if (data[i][j] == WHITE && data[i][j] == data[i][j + 1]) {
				if (data[i][j] == data[i][j - 2] && data[i][j - 1] == EMPTY) {
					if (important < 8) {
						important = 8;
						posX = i;
						posY = j - 1;
					}
				}
				else if (data[i][j] == data[i][j + 3] && data[i][j + 2] == EMPTY) {
					if (important < 8) {
						important = 8;
						posX = i;
						posY = j + 2;
					}
				}
			}

			//외에
			if (data[i][j] == WHITE) {
				int count = 0;
				if (data[i + 1][j] == BLACK)
					count++;
				if (data[i][j + 1] == BLACK)
					count++;
				if (data[i + 1][j + 1] == BLACK)
					count++;
				if (data[i - 1][j] == BLACK)
					count++;
				if (data[i][j - 1] == BLACK)
					count++;
				if (data[i - 1][j - 1] == BLACK)
					count++;
				if (data[i + 1][j - 1] == BLACK)
					count++;
				if (data[i - 1][j + 1] == BLACK)
					count++;
				if (important < 1 && count <5) {
					important = 1;
					if (data[i + 1][j] == EMPTY) {
						posX = i + 1;
						posY = j;
					}
					else if (data[i][j + 1] == EMPTY) {
						posX = i;
						posY = j + 1;
					}
					else if (data[i + 1][j + 1] == EMPTY) {
						posX = i + 1;
						posY = j + 1;
					}
					else if (data[i - 1][j] == EMPTY) {
						posX = i - 1;
						posY = j;
					}
					else if (data[i][j - 1] == EMPTY) {
						posX = i;
						posY = j - 1;
					}
					else if (data[i + 1][j - 1] == EMPTY) {
						posX = i + 1;
						posY = j - 1;
					}
					else if (data[i - 1][j + 1] == EMPTY) {
						posX = i - 1;
						posY = j + 1;
					}
					else if (data[i - 1][j - 1] == EMPTY) {
						posX = i - 1;
						posY = j - 1;
					}
				}
			}

			if (important < 1) {
				int x = 8;
				int y = 8;
				while (!(data[x][y] == EMPTY)) {
					x++;
					y--;
					if (x >19 && y < 0) {
						x = 1;
						y = 18;
					}
				}
				posX = x;
				posY = y;
			}

		}
	}


	Vector2 result;
	result.x = posX; result.y = posY;
	return result;
}



void AI_Game_TestMode()
{

	char txt[64];
	sprintf(txt, "mouse Vector : (%f,%f)",
		mousePos.x, mousePos.y);

	glColor3f(1, 1, 1);
	PrintText(txt, 0.3, -0.95);

}