#pragma once
#include "Function.h"
#include "OmokFunction.h"

void TwoPlayer_Init();
void TwoPlayer_MouseClick(int button, int state, int x, int y);
void TwoPlayer_DoMouseMotion(GLint x, GLint y);
void TwoPlayer_GetKeyboard(GLubyte key, GLint x, GLint y);
void TwoPlayer_GetSpecialKeyboard(GLint key, GLint x, GLint y);

void TwoPlayer_Timer(int value);
void TwoPlayer_TimeLimit();
void TwoPlayer_PrintTime();
void TwoPlayer_PersonTrunAnimation();

void TwoPlayer_StartAnimation();
void TwoPlayer_EndAnmation();
void TwoPlayer_MoveSceneAnimation();

void TwoPlayer_TestMode();

// 기초 데이터.
GLint* sceneNumAddr;
Vector2 mousePos;
BoardData boardData;
GLint _isInit = 0;
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

void TwoPlayerScene(GLint* sceneNum)
{
	if (_isInit == 0)
	{
		TwoPlayer_Init();
		sceneNumAddr = sceneNum;
	}

	glClearColor(1, 1, 1, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(0.0117, 0.6627, 0.9568);
	DrawCircle(blueCirRad, 0, 0);

	
	glColor3f(1, 1, 1);

	// 오목판
	DrawBoard(boardData.pos, 1, BOARD_SIZE);
	if(inBoard)
		DrawStone(boardData, mousePos.x, mousePos.y);
	DrawAllStone(boardData);
	
	// 시간제한 출력
	TwoPlayer_PrintTime();

	//사람 캐릭터 출력
	_LoadAlphaImage("GameResources/BlackCharacter.bmp", "GameResources/BlackCharacterMask.bmp", 0.55, 0.5, 1 * 0.4, 1 * 1.78* 0.4) ;
	ClippingMaskCir(blueCirRad, 0, 0);
	_LoadAlphaImage("GameResources/WhiteCharacter.bmp", "GameResources/WhiteCharacterMask.bmp", 0.55, -0.5, 1 * 0.4, 1 * 1.78 * 0.4);
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
				ClippingMaskRectReverse(0.35, -0.141667, 0.75, -0.858333);
			else if (boardData.nowTurn == WHITE)
				ClippingMaskRectReverse(0.35, 0.858333, 0.75, 0.141667);
		}
		// 타임오버로 끝난게 아닐땐 이긴 오목알에 클리핑 마스크 생성.
		else
				WinStencilEffect(boardData, winPos);
		
		

		glColor4f(0, 0, 0, 0.5);
		DrawCircle(endCirRad, 0, 0);
		glDisable(GL_STENCIL_TEST);

		// 클리핑 된 곳이 텍스트에 가리면 텍스트가 잘 안보여서 약간 알파값 조정.
		if (!endTime)
		{
			ClippingMaskCir(endCirRad, 0, 0);
			glColor4f(0, 0, 0, 0.25);
			for (int i = 0;  winPos[i].x != -300; i++)
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


void TwoPlayer_Init()
{
	_isInit = 1;
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);	 // MIN : 축소할때, NEAREST : 포토샵 최단입단 같은 효과, LINEAR : 포토샵 같은 부드럽게 효과.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glutMouseFunc(TwoPlayer_MouseClick);
	glutPassiveMotionFunc(TwoPlayer_DoMouseMotion);
	glutTimerFunc(1, TwoPlayer_Timer, 1);
	glutKeyboardFunc(TwoPlayer_GetKeyboard);
	glutSpecialFunc(TwoPlayer_GetSpecialKeyboard);
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

void TwoPlayer_MouseClick(GLint button, GLint state, GLint x, GLint y)
{


	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && !isWin)
	{
		GLint isPut;
		Vector2 tempVec = CallibratorToArray(boardData, mousePos.x, mousePos.y);
		if (inBoard == 1)
			isPut = PutStone(&boardData, &mousePos.x, &mousePos.y);

		if (Win_Check(boardData, tempVec.x, tempVec.y, winPos) && isPut)
		{
			isWin = 1;
			for (int i = 0; winPos[i].x != -300; i++)
				winPos[i] = CallibratorToBoard(boardData, winPos[i].x, winPos[i].y);

		}
		else if (inBoard == 1)
		{
			timeLimit = clock() + 20000;
		}
	}

	else if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && isWin && _animationNum == 2)
		_animationNum = 3;

	glutPostRedisplay();
}

void TwoPlayer_DoMouseMotion(GLint x, GLint y)
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

//////////////////////////////// 키보드 ////////////////////////////////////////////
void TwoPlayer_GetKeyboard(GLubyte key, GLint x, GLint y)
{
	GLfloat lineSpace = boardData.scale / (boardData.amount - 1);
	GLint isPut;

	// 씬 바꿀 때.
	if (isWin && _animationNum == 2)
		_animationNum = 3;

	

		switch (key)
		{
		case 'a':
			mousePos.x -= lineSpace;
			if (startPoint.x > mousePos.x - lineSpace * 0.5)
				mousePos.x = startPoint.x + 0.01;
			break;
		case 'd':
			mousePos.x += lineSpace;
			if (startPoint.x + boardData.scale < mousePos.x + lineSpace * 0.5)
				mousePos.x = startPoint.x + boardData.scale - 0.01;
			break;
		case 's':
			mousePos.y -= lineSpace * 1.78;
			if (startPoint.y - boardData.scale * 1.78 > mousePos.y - lineSpace * 1.78 * 0.5)
				mousePos.y = startPoint.y - boardData.scale * 1.78 + 0.01;
			break;
		case 'w':
			mousePos.y += lineSpace * 1.78;
			if (startPoint.y < mousePos.y + lineSpace * 1.78 * 0.5)
				mousePos.y = startPoint.y - 0.01;
			break;
		case 32: //스페이스바
		case 'q':

			isPut = PutStone(&boardData, &mousePos.x, &mousePos.y);
			Vector2 tempVec = CallibratorToArray(boardData, mousePos.x, mousePos.y);
			timeLimit = clock() + 20000;

			if (Win_Check(boardData, tempVec.x, tempVec.y, winPos) && isPut)
			{
				isWin = 1;
				for (int i = 0; winPos[i].x != -300; i++)
					winPos[i] = CallibratorToBoard(boardData, winPos[i].x, winPos[i].y);

			}
			break;
		default:
			break;
		}

	glutPostRedisplay();
}

void TwoPlayer_GetSpecialKeyboard(GLint key, GLint x, GLint y)
{
	GLfloat lineSpace = boardData.scale / (boardData.amount - 1);

	// 씬 바꿀 때.
	if (isWin && _animationNum == 2)
		_animationNum = 3;


	if (mousePos.x > startPoint.x && mousePos.x < startPoint.x + boardData.scale
		&& mousePos.y < startPoint.y && mousePos.y > startPoint.y - boardData.scale * 1.78)
	{

		switch (key)
		{
		case GLUT_KEY_LEFT:
			mousePos.x -= lineSpace;
			if (startPoint.x > mousePos.x - lineSpace * 0.5)
				mousePos.x = startPoint.x + 0.01;
			break;
		case GLUT_KEY_RIGHT:
			mousePos.x += lineSpace;
			if (startPoint.x + boardData.scale < mousePos.x + lineSpace * 0.5)
				mousePos.x = startPoint.x + boardData.scale - 0.01;
			break;
		case GLUT_KEY_DOWN:
			mousePos.y -= lineSpace * 1.78;
			if (startPoint.y - boardData.scale * 1.78 > mousePos.y - lineSpace * 1.78 * 0.5)
				mousePos.y = startPoint.y - boardData.scale * 1.78 + 0.01;
			break;
		case GLUT_KEY_UP:
			mousePos.y += lineSpace * 1.78;
			if (startPoint.y < mousePos.y + lineSpace * 1.78 * 0.5)
				mousePos.y = startPoint.y - 0.01;
			break;
		default:
			break;
		}
	}

	glutPostRedisplay();

}


//////////////////////////////////////// 타이머 ////////////////////////////////////////
void TwoPlayer_Timer(int value)
{
	if (_animationNum == 0)
		TwoPlayer_StartAnimation();
	else if (_animationNum == 3)
		TwoPlayer_MoveSceneAnimation();

	if (*sceneNumAddr != 2)
	{
		_isInit = 0;
		glutPostRedisplay();
		return;
	}

	if (isWin == 1 && _animationNum != 3)
		TwoPlayer_EndAnmation();
	else if (endTime != 1)
		TwoPlayer_TimeLimit();

	if (isWin != 1 && blueCirRad >= 1.1)
		TwoPlayer_PersonTrunAnimation();

	glutPostRedisplay();
	glutTimerFunc(1, TwoPlayer_Timer, 1);
}

void TwoPlayer_TimeLimit()
{

	nowtime = clock();
	if (timeLimit - nowtime < 0)
	{
		endTime = 1;	isWin = 1;
	}

}

void TwoPlayer_PrintTime()
{
	GLbyte timeChar[12];
	sprintf(timeChar,"Time : %d", (GLint)(timeLimit - nowtime)/1000);

	int length = (int)strlen(timeChar);

	glColor3f(1, 1, 1);
		glRasterPos2f(0.5, 0);

	for (int i = 0; i < length; i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, timeChar[i]);
	glFlush();



}

void TwoPlayer_StartAnimation()
{
	SmoothEndReshapeCirlceAnimation(&blueCirRad, 1.2, 1);
}

void TwoPlayer_EndAnmation()
{
	SmoothEndReshapeCirlceAnimation(&endCirRad, 1.2, 1);
	if (endCirRad >= 1.15)
		_animationNum = 2;
}

void TwoPlayer_PersonTrunAnimation()
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


void TwoPlayer_MoveSceneAnimation()
{
	SmoothEndReshapeCirlceAnimation(&returnCirRad, 0, 3);
	if (returnCirRad <= 0.001)
		*sceneNumAddr = 0;
}

////////////////////////////////////////////////////////////////////////////////////////

void TwoPlayer_TestMode()
{

	char txt[64];
	sprintf(txt, "mouse Vector : (%f,%f)",
		mousePos.x, mousePos.y);

	glColor3f(1, 1, 1);
	PrintText(txt, 0.3, -0.95);

}