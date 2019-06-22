#pragma once
#include "Function.h"


void Start_Initialize();
void Start_DoDisplay();
void Start_DoMouse(GLint button, GLint state, GLint x, GLint y);
void Start_DoMouseMotion(GLint x, GLint y);
void Start_DoTimer(int value);
void Start_TestMode();

void AppImgAnimation();
void Start_BackgroundAnimation();
void Start_LineAnimation();
void Start_TitleAnimation();
void Start_NameAnimation();
void Start_BackClippingAnimation();
void Start_StartBtnClippingAnimation();
void Start_SingleMultiBtnAnimation();
void Start_WhiteReturnAnimation();
void Start_EndAnimation();


GLint animationNum = 0; GLboolean isInit = 0;

GLint testing = 0;
Vector2 mouseVector;
GLint fps; 	char fpsChar[100]; GLfloat lastTime;

//앱이미지 데이터
Vector2 appImgPos;
AnimationInfo appImgAni;
GLfloat appAcc;

//배경화면.
GLfloat backgroundRad;
GLfloat backgroundEndRad; GLfloat backgroundSpeed;

//라인 데이터
Vector2 linePos;
GLfloat lineSize;
AnimationInfo lineAni;
GLfloat lineAcc;

//타이틀 데이터
Vector2 titlePos;
AnimationInfo titleAni;
GLfloat titleAcc;

//이름 데이터
Vector2 namePos;
AnimationInfo nameAni;
GLfloat nameAcc;

//배경화면 클리핑 마스크.
GLfloat backClippingRad; GLfloat backClippingendRad;
GLfloat backClippingspeed;

//버튼 클리핑 마스크.
GLfloat startBtnClippingRad; GLfloat startBtnClippingEndRad;
GLfloat startBtnClippingSpeed;

// 스타트 버튼
Vector2 startBtnStartPos;Vector2 startBtnEndPos;
Color4 startBtnColor;

// 싱글버튼
Vector2 singleBtnPos;
AnimationInfo singleBtnAni; GLfloat singleBtnAcc;
Color4 singleBtnColor;

// 멀티버튼
Vector2 multiBtnPos;
AnimationInfo multiBtnAni; GLfloat multiBtnAcc;
Color4 multiBtnColor;

// 흰 배경 클리핑 마스크.
Vector2 whiteClippingPos; GLfloat whiteClippingRad;
 GLfloat whiteClippingSpeed;


 //AI, 2player 버튼
 Vector2 aiBtnStartPos; Vector2 aiBtnEndPos; GLfloat aiBtnAlpha; GLboolean aiBtnClick;
 Vector2 twoBtnStartPos; Vector2 twoBtnEndPos; GLfloat twoBtnAlpha;GLboolean twoBtnClick;

 // 뭘 선택했는지.
 GLint selected;

 GLint* nextScene; 

void StartScene(GLint* SceneNum)
{

	if (isInit == 0) {
		isInit = 1;
		nextScene = SceneNum;
		Start_Initialize();
		glutMouseFunc(Start_DoMouse);
		glutPassiveMotionFunc(Start_DoMouseMotion);
		glutTimerFunc(500, Start_DoTimer, 1);	//1000 = 1s
	}

	Start_DoDisplay();
	

}

void Start_Initialize()
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);	 // MIN : 축소할때, NEAREST : 포토샵 최단입단 같은 효과, LINEAR : 포토샵 같은 부드럽게 효과.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND); // <8>


	glClearColor(1, 1, 1, 1);

	//test
	fps = 0;lastTime = clock();

	appImgPos.x = -1; appImgPos.y = -1;
	appImgAni.start = appImgPos;
	appImgAni.end.x = 1;
	appImgAni.end.y = 1;
	appImgAni.speed = 1;
	appAcc = 0;


	animationNum = 0;

	backgroundRad = 0; backgroundEndRad = 1.17;
	backgroundSpeed = 2;

	linePos.x = -1;  linePos.y = 0.5; lineSize = 0.62;
	lineAni.start = linePos;
	lineAni.end.x = 0.01; lineAni.end.y = 0.5;
	lineAni.speed = 1; lineAcc = 0;

	titlePos.x = 0; titlePos.y = 0.25;
	titleAni.start = titlePos;
	titleAni.end.x = 0; titleAni.end.y = 0.67;
	titleAni.speed = 2;
	titleAcc = 0;

	namePos.x = 0; namePos.y = 0.65;
	nameAni.start = namePos;
	nameAni.end.x = 0; nameAni.end.y = 0.4;
	nameAni.speed = 2;
	nameAcc = 0;

	backClippingRad = 0;backClippingendRad = 1.17;
	backClippingspeed = 1.3;

	startBtnClippingRad = 0; startBtnClippingEndRad = 0.145;
	startBtnClippingSpeed = 4;

	startBtnStartPos.x = -0.132813; startBtnStartPos.y = -0.388889;
	startBtnEndPos.x = 0.131250; startBtnEndPos.y = -0.608333;
	startBtnColor.r = startBtnColor.g = startBtnColor.b = startBtnColor.a = 0;

	singleBtnPos.x = 0; singleBtnPos.y = -0.5;
	singleBtnAni.start = singleBtnPos;
	singleBtnAni.end.x = -0.5; singleBtnAni.end.y = singleBtnPos.y;
	singleBtnAni.speed = 5; singleBtnAcc = 0.5;
	singleBtnColor.r = singleBtnColor.g = singleBtnColor.b = singleBtnColor.a = 0;

	multiBtnPos.x = 0; multiBtnPos.y = -0.5;
	multiBtnAni.start = multiBtnPos;
	multiBtnAni.end.x = 0.5; multiBtnAni.end.y = multiBtnPos.y;
	multiBtnAni.speed = 5; multiBtnAcc = 0.5;
	multiBtnColor.r = multiBtnColor.g = multiBtnColor.b = multiBtnColor.a = 0;

	whiteClippingRad = 1.5;
	whiteClippingSpeed = 3;

	aiBtnStartPos.x = -0.525000; aiBtnStartPos.y = 0.441667; aiBtnEndPos.x = -0.073438; aiBtnEndPos.y = -0.441667; aiBtnAlpha = 0;aiBtnClick = 0;
	twoBtnStartPos.x = 0.073438, twoBtnStartPos.y = 0.441667; twoBtnEndPos.x = 0.525000; twoBtnEndPos.y = -0.441667; twoBtnAlpha = 0;twoBtnClick = 0;

	selected = EMPTY;
}



void Start_DoDisplay()
{

	glClear(GL_COLOR_BUFFER_BIT);

	if (animationNum <= 7) {

		glColor3f(0.0117, 0.6627, 0.9568);
		//배경출력.
		DrawCircle(backgroundRad, 0, 0);


		// 라인 출력
		glColor3f(1, 1, 1);
		ClippingMaskRect(-0.3, 0.8, 0.3, 0.1); // 라인 클리핑마스크 생성
		glBegin(GL_LINE_STRIP);
		{
			glVertex2f(linePos.x - lineSize * 0.5, linePos.y);
			glVertex2f(linePos.x + lineSize * 0.5, linePos.y);
		}
		glEnd();
		glDisable(GL_STENCIL_TEST);


		// 배경 출력
		ClippingMaskCir(backClippingRad, 0, 0);
		_LoadAlphaImage("GameResources/MainBackground.bmp", "GameResources/MainBackgroundMask.bmp", 0, 0, 2, 2);
		glDisable(GL_STENCIL_TEST);

		// 타이틀 출력
		ClippingMaskRect(-0.5, 1, 0.5, 0.5);
		_LoadAlphaImage("GameResources/Title.bmp", "GameResources/TitleMask.bmp", titlePos.x, titlePos.y, 0.4, 0.4);
		glDisable(GL_STENCIL_TEST);


		// 이름 출력
		ClippingMaskRect(-0.5, 0.5, 0.5, 0);
		_LoadAlphaImage("GameResources/Name.bmp", "GameResources/NameMask.bmp", namePos.x, namePos.y, 0.4, 0.4);
		glDisable(GL_STENCIL_TEST);



		// 스타트 버튼 출력
		ClippingMaskCir(startBtnClippingRad, 0, -0.5);
		_LoadAlphaImage("GameResources/StartBtn.bmp", "GameResources/StartBtnMask.bmp", 0, -0.5, 0.35, 0.3);

		// 스타트 버튼 효과출력
		glColor4f(startBtnColor.r, startBtnColor.g, startBtnColor.b, startBtnColor.a);
		glRectf(startBtnStartPos.x, startBtnStartPos.y, startBtnEndPos.x, startBtnEndPos.y);
		glDisable(GL_STENCIL_TEST);

		// 싱글버튼 출력
		ClippingMaskRect(-1, -0.388889, -0.132813, -0.608333);
		_LoadAlphaImage("GameResources/SingleBtn.bmp", "GameResources/SingleBtnMask.bmp", singleBtnPos.x, singleBtnPos.y, 0.35, 0.3);
		// 싱글 버튼 효과 출력
		glColor4f(singleBtnColor.r, singleBtnColor.g, singleBtnColor.b, singleBtnColor.a);
		glRectf(singleBtnPos.x - 0.132813, singleBtnPos.y + 0.388889, singleBtnPos.x + 0.132813, singleBtnPos.y - 0.388889);
		glDisable(GL_STENCIL_TEST);

		// 멀티버튼 출력
		ClippingMaskRect(0.131250, -0.388889, 1, -0.608333);
		_LoadAlphaImage("GameResources/MultiBtn.bmp", "GameResources/MultiBtnMask.bmp", multiBtnPos.x, multiBtnPos.y, 0.35, 0.3);
		// 싱글 버튼 효과 출력
		glColor4f(multiBtnColor.r, multiBtnColor.g, multiBtnColor.b, multiBtnColor.a);
		glRectf(multiBtnPos.x - 0.132813, multiBtnPos.y + 0.2, multiBtnPos.x + 0.13, multiBtnPos.y - 0.2);
		glDisable(GL_STENCIL_TEST);
	}

	
	if (animationNum >= 7 && selected == LOCAL) {

		// 흰 배경 출력, 흰색이 안어울려서 바꿈. 변수명 주의
		ClippingMaskCirReverse(whiteClippingRad, whiteClippingPos.x, whiteClippingPos.y);

		glColor3f(0, 0.5882352941, 0.5333333333);
		glRectf(-1, 1, 1, -1);
		_LoadAlphaImage("GameResources/AI_Game.bmp", "GameResources/AI_GameMask.bmp", -0.3, 0, 0.5, 0.5 * 1.78);
		_LoadAlphaImage("GameResources/2Player_Game.bmp", "GameResources/2Player_GameMask.bmp", 0.3, 0, 0.5, 0.5 * 1.78);
		glColor4f(0, 0, 0, aiBtnAlpha);
		glRectf(aiBtnStartPos.x, aiBtnStartPos.y, aiBtnEndPos.x, aiBtnEndPos.y);
		glColor4f(0, 0, 0, twoBtnAlpha);
		glRectf(twoBtnStartPos.x, twoBtnStartPos.y, twoBtnEndPos.x, twoBtnEndPos.y);
		glDisable(GL_STENCIL_TEST);

	}
	if (animationNum >= 7 && (selected == MULTI || selected == CLIENT || selected == SERVER)) {

		// 흰 배경 출력, 흰색이 안어울려서 바꿈. 변수명 주의
		ClippingMaskCirReverse(whiteClippingRad, whiteClippingPos.x, whiteClippingPos.y);

		glColor3f(0, 0.5882352941, 0.5333333333);
		glRectf(-1, 1, 1, -1);
		_LoadAlphaImage("GameResources/ServerIcon.bmp", "GameResources/ServerIconMask.bmp", -0.3, 0, 0.5, 0.5 * 1.78);
		_LoadAlphaImage("GameResources/ClientIcon.bmp", "GameResources/ClientIconMask.bmp", 0.3, 0, 0.5, 0.5 * 1.78);
		glColor4f(0, 0, 0, aiBtnAlpha);
		glRectf(aiBtnStartPos.x, aiBtnStartPos.y, aiBtnEndPos.x, aiBtnEndPos.y);
		glColor4f(0, 0, 0, twoBtnAlpha);
		glRectf(twoBtnStartPos.x, twoBtnStartPos.y, twoBtnEndPos.x, twoBtnEndPos.y);
		glDisable(GL_STENCIL_TEST);

	}

	

	if (animationNum >= 7)
	{

	}
	

	// 테스트 모드
	if (testing) {
		sprintf(fpsChar, "FPS : %d", fps);
		glColor3f(1, 1, 1);
		PrintText(fpsChar, -0.9, -0.95);

		char tempchar[20];
		sprintf(tempchar, "SceneNum : %d", *nextScene);
		glColor3f(0, 0, 0);
		PrintText(tempchar, -0.4, -0.95);
		Start_TestMode();
	}

	glutSwapBuffers();
}

//////////////////////////////////////////// 마우스 /////////////////////////////////////////////

void Start_DoMouse(GLint button, GLint state, GLint x, GLint y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		//테스트
		if (testing)
		{
			mouseVector.x = x;
			mouseVector.y = y;
		}


		if (CheckRectButton(startBtnStartPos.x, startBtnStartPos.y, startBtnEndPos.x, startBtnEndPos.y, x, y) && animationNum >= 3)
			animationNum = 5;

		else if (singleBtnColor.a >= 0.14  && animationNum >= 6)
		{
			whiteClippingPos = singleBtnPos;
			animationNum = 7;
			selected = LOCAL;
		}
		else if (multiBtnColor.a >= 0.14  && animationNum >= 6)
		{
			whiteClippingPos = multiBtnPos;
			animationNum = 7;
			selected = MULTI;
		}
		else if (aiBtnAlpha >= 0.14 && animationNum == 8 && selected == LOCAL)
		{
			whiteClippingPos.x = (aiBtnStartPos.x + aiBtnEndPos.x) * 0.5;
			whiteClippingPos.y = (aiBtnStartPos.y + aiBtnEndPos.y) * 0.5;
			animationNum = 9;
			aiBtnClick = 1;
		}
		else if (twoBtnAlpha >= 0.14 && animationNum == 8 && selected == LOCAL)
		{
			whiteClippingPos.x = (twoBtnStartPos.x + twoBtnEndPos.x) * 0.5;
			whiteClippingPos.y = (twoBtnStartPos.y + twoBtnEndPos.y) * 0.5;
			animationNum = 9;
			twoBtnClick = 1;
		}
		else if (aiBtnAlpha >= 0.14 && animationNum == 8 && selected == MULTI)
		{
			whiteClippingPos.x = (aiBtnStartPos.x + aiBtnEndPos.x) * 0.5;
			whiteClippingPos.y = (aiBtnStartPos.y + aiBtnEndPos.y) * 0.5;
			animationNum = 9;
			selected = SERVER;
		}
		else if (twoBtnAlpha >= 0.14 && animationNum == 8 && selected == MULTI)
		{
			whiteClippingPos.x = (twoBtnStartPos.x + twoBtnEndPos.x) * 0.5;
			whiteClippingPos.y = (twoBtnStartPos.y + twoBtnEndPos.y) * 0.5;
			animationNum = 9;
			selected = CLIENT;
		}

		//////////////////////////////////////////////////////////////////////// 버튼 누르는거.

	}
}


void Start_DoMouseMotion(GLint x, GLint y)
{

	if (CheckRectButton(startBtnStartPos.x, startBtnStartPos.y, startBtnEndPos.x, startBtnEndPos.y, x, y))
		startBtnColor.a = 0.15;
	else
		startBtnColor.a = 0;

	if (CheckRectButton(singleBtnPos.x - 0.13, singleBtnPos.y + 0.12, singleBtnPos.x + 0.13, singleBtnPos.y - 0.12, x, y))
		singleBtnColor.a = 0.15;
	else
		singleBtnColor.a = 0;

	if (CheckRectButton(multiBtnPos.x - 0.13, multiBtnPos.y + 0.12, multiBtnPos.x + 0.13, multiBtnPos.y - 0.12, x, y))
		multiBtnColor.a = 0.15;
	else
		multiBtnColor.a = 0;

	if (CheckRectButton(-0.525000, 0.441667, 0.073438, -0.441667, x, y))
		aiBtnAlpha = 0.15;
	else
		aiBtnAlpha = 0;

	if (CheckRectButton(0.073438, 0.441667, 0.525000, -0.441667, x, y))
		twoBtnAlpha = 0.15;
	else
		twoBtnAlpha = 0;
}


void Start_TestMode()
{

	char txt[64];
	sprintf(txt, "mouse Vector : (%.0f,%.0f) -> (%f,%f)",
		mouseVector.x, mouseVector.y, mouseVector.x / (1280 * 0.5) - 1, (mouseVector.y / (720 * 0.5) - 1) * -1);
	
	glColor3f(1, 1, 1);
	PrintText(txt, 0.3, -0.95);

}





//////////////////////////////////////////////////// 타이머 //////////////////////////////////////////////////
void Start_DoTimer(int value)
{
	if(testing)
		FpsCounter(&fps, &lastTime);


	if (animationNum == 0) {
		Start_LineAnimation();
		Start_BackgroundAnimation();
	}
	else if (animationNum == 1 || animationNum == 2) {
		Start_TitleAnimation();
		Start_NameAnimation();
	}
	else if (animationNum >= 3) {
		Start_BackClippingAnimation();
		Start_StartBtnClippingAnimation();
	}
	if (animationNum == 5)
		Start_SingleMultiBtnAnimation();
	else if (animationNum == 7)
		Start_WhiteReturnAnimation();
	else if (animationNum == 9)
		Start_EndAnimation();

	if (animationNum == 9 && whiteClippingRad >= 1.695)
	{
		if (aiBtnClick == 1)
		{
			*nextScene = 1;
		}
		else if (twoBtnClick == 1)
		{
			*nextScene = 2;
		}
		else if (selected == SERVER)
			*nextScene = 3;
		else if (selected == CLIENT)
			*nextScene = 4;

		isInit = 0;
		glutPostRedisplay();
		return;
	}


	glutPostRedisplay();
	glutTimerFunc(5, Start_DoTimer, 1);		//0.005s => 최대 200FPS
}



void Start_BackgroundAnimation()
{
	if (backgroundRad < backgroundEndRad - 0.005)
		SmoothEndReshapeCirlceAnimation(&backgroundRad, backgroundEndRad, backgroundSpeed);

}

void Start_LineAnimation()
{
	if (linePos.x <= lineAni.end.x - 0.01)
		SmoothMoveAnimation(&linePos, lineAni, &lineAcc);
	else
		animationNum++;		// animationNum = 1
}

void Start_TitleAnimation()
{
	if (titlePos.y <= titleAni.end.y -0.003)
		SmoothMoveAnimation(&titlePos, titleAni, &titleAcc);
	else if(animationNum == 1)
		animationNum ++;		// animationNum = 2
}

void Start_NameAnimation()
{
	if (namePos.y >= nameAni.end.y + 0.003)
		SmoothMoveAnimation(&namePos, nameAni, &nameAcc);
	else if(animationNum == 2)
		animationNum++;		// animationNum = 3
}

void Start_BackClippingAnimation()
{
	if (backClippingRad <= backClippingendRad - 0.01)
		SmoothEndReshapeCirlceAnimation(&backClippingRad, backClippingendRad, backClippingspeed);

}

void Start_StartBtnClippingAnimation()
{
	if (startBtnClippingRad <= startBtnClippingEndRad - 0.001 && backClippingRad > backClippingendRad - 0.5)
		SmoothEndReshapeCirlceAnimation(&startBtnClippingRad, startBtnClippingEndRad, startBtnClippingSpeed);
	else if(!( backClippingRad <= backClippingendRad - 0.01) && animationNum == 3)
		animationNum++;		// animationNum = 4
}

void Start_SingleMultiBtnAnimation()
{
	if (singleBtnPos.x >= singleBtnAni.end.x + 0.001)
	{
		SmoothMoveAnimation(&singleBtnPos, singleBtnAni, &singleBtnAcc);
		SmoothMoveAnimation(&multiBtnPos, multiBtnAni, &multiBtnAcc);
	}
	else
		animationNum++;		// animationNum = 6

}

void Start_WhiteReturnAnimation()
{
	if (whiteClippingRad >= 0.001)
		SmoothEndReshapeCirlceAnimation(&whiteClippingRad, 0, whiteClippingSpeed);
	else
	{
		animationNum++;		// animationNum = 8
		whiteClippingRad = 0;
	}
}

void Start_EndAnimation()
{
	if (whiteClippingRad <= 1.7)
		SmoothEndReshapeCirlceAnimation(&whiteClippingRad, 1.7, whiteClippingSpeed * 0.5);
	else
		animationNum++;		// animationNum = 10
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void AppImgAnimation()
{


	if (appImgPos.x <= 0.999) {		// 1로 하면 무한루프.
		SmoothMoveAnimation(&appImgPos, appImgAni, &appAcc);
	}

}

void Start_Test()
{



	glClearColor(0.0117, 0.6627, 0.9568, 1);

	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(1, 1, 1);
	PrintText("abcd", 0, 0);

	ClippingMaskRect(-0.5, -0.5, 0.5, 0.5);
	_LoadAlphaImage("AppIcon.bmp", "AppIconMask.bmp", appImgPos.x, appImgPos.y, 1, 1.78);
	glDisable(GL_STENCIL_TEST);



	//RenderScene();

	glEnd();

	glutSwapBuffers();

}
