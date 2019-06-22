#pragma once
#pragma comment(lib, "Ws2_32.lib")

#include "Function.h"
#include "OmokFunction.h"
#include <process.h>

void ServerGame_Init();
void ServerGame_MouseClick(int button, int state, int x, int y);
void ServerGame_DoMouseMotion(GLint x, GLint y);

void ServerGame_InputKeyboard(unsigned char key, int x, int y);

void ServerGame_Timer(int value);
void ServerGame_TimeLimit();
void ServerGame_PrintTime();
void ServerGame_PersonTrunAnimation();
void ServerGame_StartAnimation();
void ServerGame_EndAnmation();
void ServerGame_MoveSceneAnimation();

void ServerGame_NetworkServer();
void ServerGame_NetworkClient();
void ServerGame_CloseSocket();


void ServerGame_TestMode();
int getMyIP(IPv4 * myIP);

unsigned __stdcall ThreadNetwork(void *arg);


// 기초 데이터.
GLint* sceneNumAddr;
Vector2 mousePos;
BoardData boardData;
GLint ___isInit = 0;
GLint _animationNum;
Vector2 startPoint;
GLboolean inBoard;
Vector2 winPos[12];
GLboolean isWin;
GLfloat timeLimit; GLfloat nowtime; GLboolean endTime;
Vector2 whiteTimePos; Vector2 blackTimePos;

GLint myState;
GLint myNetworkState;

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

GLubyte nowNetworkState[32]; GLubyte myIP_char[32]; GLubyte inputIp[32]; GLboolean endInputIp;
 SOCKET serverSocket; SOCKET clientSocket;
 GLubyte message[12];



 unsigned __stdcall ThreadNetwork(void *arg)
 {

	 if (myState == SERVER) 
		 ServerGame_NetworkServer();
	 else if (myState == CLIENT)
		 ServerGame_NetworkClient();
	 while (!isWin)
	 {
		 GLint strLen = 0;
		 GLubyte strTmp[4];	// sprintf는 뒤에 널문자를 자동으로 추가하기때문에 더 길게.
		 GLint resvPosX; GLint resvPosY;
		 sprintf(strTmp, "");

		 if (myNetworkState != NETWORK_NOT_CONNECT)
			 strLen = recv(clientSocket, message, 12, 0);
		 if (strLen == 0 || strLen == -1)
			 continue;

		 // strcmp 하면 문자열 끝까지 검사하기때문에 언제나 틀린게 된다.
		 if (strncmp(message, "END",3) == 0)
		 {
			 isWin = 1;

			 // 이긴측이 END 전송. 이긴측 NetworkState 변경.
			 //진 측이 END 받아옴. 진 측은 소켓을 닫고 NetworkState 변경. 이긴측에게 END 전송. 쓰레드 종료
			 // 이긴측은 END 받아옴. 소켓을 닫고 쓰레드 종료.
			 ServerGame_CloseSocket();
		 }


		 // secv에서 기다리다가 내려오므로 한번더 검사해줘야함.
		 if (myNetworkState != NETWORK_NOT_CONNECT)
		 {

			 timeLimit = clock() + 20000;
			 sprintf(strTmp, "%c%c", message[0], message[1]);
			 resvPosX = atoi(strTmp);
			 sprintf(strTmp, "%c%c", message[2], message[3]);
			 resvPosY = atoi(strTmp);


			 PutStoneNoCali(&boardData, resvPosX, resvPosY);

			 if (Win_Check(boardData, resvPosX, resvPosY, winPos))
			 {
				 isWin = 1;
				 for (int i = 0; winPos[i].x != -300; i++)
					 winPos[i] = CallibratorToBoard(boardData, winPos[i].x, winPos[i].y);


				// free(strTmp);
				 ServerGame_CloseSocket();
			 }
		 }
			 
		 //}
	 }

	 return 0;
 }

 ///////////////////////////////////////////////////////////////////////////

void ServerGameScene(GLint* sceneNum)
{
	if (___isInit == 0)
	{
		sceneNumAddr = sceneNum;
		ServerGame_Init();
		_beginthreadex(NULL, 0, ThreadNetwork, 0, 0, NULL); //Thread Start
		glClearColor(1, 1, 1, 0);
	}

	glClear(GL_COLOR_BUFFER_BIT);

	if (myNetworkState == NETWORK_NOT_CONNECT && isWin == 0)
	{
		
		glColor3f(0.0117, 0.6627, 0.9568);

		if (myState == SERVER)
		{
			PrintText(nowNetworkState, -0.1, 0);
			PrintText(myIP_char, -0.1, -0.1);
		}
		else if (myState == CLIENT)
		{
			PrintText(nowNetworkState, -0.3, 0);
			PrintText(inputIp, -0.1, -0.1);
		}

		glutSwapBuffers();
		return;
	}




	glColor3f(0.0117, 0.6627, 0.9568);
	DrawCircle(blueCirRad, 0, 0);


	glColor3f(1, 1, 1);

	// 오목판
	DrawBoard(boardData.pos, 1, BOARD_SIZE);
	if (inBoard)
		DrawStone(boardData, mousePos.x, mousePos.y);
	DrawAllStone(boardData);

	// 시간제한 출력
	ServerGame_PrintTime();

	//사람 캐릭터 출력
	_LoadAlphaImage("GameResources/BlackCharacter.bmp", "GameResources/BlackCharacterMask.bmp", 0.55, 0.5, 1 * 0.4, 1 * 1.78* 0.4);
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

	///////////////////////체크//////////////////////////////////////////////////
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

			if (myState == SERVER)
				closesocket(serverSocket);
			closesocket(clientSocket);
			WSACleanup();
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


	//ServerGame_TestMode();

	glutSwapBuffers();
}

/////////////////////////////////////////////////////////////////// 초기화/////////////////////////////////////////////////////

void ServerGame_Init()
{
	___isInit = 1;
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);	 // MIN : 축소할때, NEAREST : 포토샵 최단입단 같은 효과, LINEAR : 포토샵 같은 부드럽게 효과.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glutMouseFunc(ServerGame_MouseClick);
	glutPassiveMotionFunc(ServerGame_DoMouseMotion);
	glutTimerFunc(1, ServerGame_Timer, 1);
	glutKeyboardFunc(ServerGame_InputKeyboard);

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

	if (*sceneNumAddr == 3)
		myState = SERVER;
	else if (*sceneNumAddr == 4)
		myState = CLIENT;

	myNetworkState = NETWORK_NOT_CONNECT; sprintf(nowNetworkState, "unknow");sprintf(inputIp, "");
	endInputIp = 0;clientSocket = NULL; serverSocket = NULL; sprintf(message, "");
	closesocket(clientSocket);closesocket(serverSocket); WSACleanup();
}

//////////////////////////////////////// 마우스 /////////////////////////////////////////

void ServerGame_MouseClick(GLint button, GLint state, GLint x, GLint y)
{


	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && !isWin && myNetworkState > NETWORK_NOT_CONNECT)
	{
		if (myState == SERVER && boardData.nowTurn == WHITE)
			return;
		else if (myState == CLIENT && boardData.nowTurn == BLACK)
			return;

		GLint isPut;
		Vector2 tempVec = CallibratorToArray(boardData, mousePos.x, mousePos.y);
		if (inBoard == 1)
		{
			// 추가
			Vector2 caliPos;
			caliPos.x = 0; caliPos.y = 0;
			caliPos = CallibratorToArray(boardData, mousePos.x, mousePos.y);
			if(boardData.boardArr[(GLint)caliPos.x][(GLint)caliPos.y] == EMPTY)
			sprintf(message, "%2d%2d", (GLint)caliPos.x, (GLint)caliPos.y);
			send(clientSocket, message, strlen(message), 0);

			isPut = PutStone(&boardData, &mousePos.x, &mousePos.y);


		}

		if (Win_Check(boardData, tempVec.x, tempVec.y, winPos) && isPut)
		{
			isWin = 1;
			send(clientSocket, "END", strlen("END"), 0);
			//왠진 모르겠지만 네트워크게임이 끝나면 _animationNum 이 0으로 바뀜.
			myNetworkState = NETWORK_NOT_CONNECT;
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

void ServerGame_DoMouseMotion(GLint x, GLint y)
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
void ServerGame_Timer(int value)
{

	if (myNetworkState == NETWORK_NOT_CONNECT && isWin == 0)
	{

		glutPostRedisplay();
		glutTimerFunc(1, ServerGame_Timer, 1);
		nowtime = clock();
		timeLimit = clock() + 20000;
		return;
	}

	if (_animationNum == 0)
		ServerGame_StartAnimation();
	else if (_animationNum == 3)
		ServerGame_MoveSceneAnimation();

	if (*sceneNumAddr != 3 && *sceneNumAddr !=4)
	{
		___isInit = 0;
		glutPostRedisplay();
		return;
	}

	if (isWin == 1 && _animationNum != 3)
		ServerGame_EndAnmation();
	else if (endTime != 1)
		ServerGame_TimeLimit();

	if (isWin != 1 && blueCirRad >= 1.1)
		ServerGame_PersonTrunAnimation();

	glutPostRedisplay();
	glutTimerFunc(1, ServerGame_Timer, 1);
}

void ServerGame_TimeLimit()
{

	nowtime = clock();
	if (timeLimit - nowtime < 0)
	{
		endTime = 1;	isWin = 1;
	}

}

void ServerGame_PrintTime()
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

void ServerGame_StartAnimation()
{
	SmoothEndReshapeCirlceAnimation(&blueCirRad, 1.2, 1);
}

void ServerGame_EndAnmation()
{
	SmoothEndReshapeCirlceAnimation(&endCirRad, 1.2, 1);
	if (endCirRad >= 1.15)
		_animationNum = 2;
}

void ServerGame_PersonTrunAnimation()
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


void ServerGame_MoveSceneAnimation()
{
	SmoothEndReshapeCirlceAnimation(&returnCirRad, 0, 3);
	if (returnCirRad <= 0.001)
		*sceneNumAddr = 0;
}




////////////////////////////////////// 서버 /////////////////////////////////////////////////////////////////




void ServerGame_CloseSocket()
{
	if (myNetworkState != NETWORK_NOT_CONNECT)
		send(clientSocket, "END", strlen("END"), 0);


	closesocket(clientSocket);
	if (myState == SERVER)
		closesocket(serverSocket);
	WSACleanup();
	myNetworkState = NETWORK_NOT_CONNECT;
	_animationNum = 2;

}

void ServerGame_InputKeyboard(unsigned char key, int x, int y)
{
	if (myState == CLIENT && myNetworkState != NETWORK_WAIT)
	{
		if ((key >= '0' && key <= '9') || key == '.') {
			GLubyte temp[3];
			sprintf(temp, "%c", key);
			strcat(inputIp, temp);
		}
		else if (key == 8)
		{
			int i;
			for (i = 0; inputIp[i] != '\0'; i++);
			if (i != 0)
				inputIp[i - 1] = '\0';
		}
		else if (key == 10 || key == 13)
			endInputIp = 1;
	}

}

void ServerGame_NetworkClient()
{


	// 네트워크
	WSADATA wsaData;
	int strLen;
	SOCKADDR_IN servAdr;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		fputs("WSAStartup() error!\n", stderr);

	sprintf(nowNetworkState, "Please enter the access server IP [0.0.0.0]");

	while (!endInputIp);

	clientSocket = socket(PF_INET, SOCK_STREAM, 0);

	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = inet_addr(inputIp);
	servAdr.sin_port = htons(5001);

	if (connect(clientSocket, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR)
		fputs("connect() error!", stderr);
	else
		puts("Connected...........");

	myNetworkState = NETWORK_WAIT;

	// 도대체 뭔 이유인지는 모르겠지만, 서버를 연결하면 클라이언트의 returnCirRad가 0이 되버림.
	returnCirRad = 1.2;

}

////////////////////////////////////////////////////////////////////////////////////////

void ServerGame_NetworkServer()
{

	// 네트워크
	WSADATA wsaData;
	int strLen, i;
	SOCKADDR_IN servAdr, clntAdr;
	IPv4 myIP;
	int clntAdrSize;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		fputs("WSAStartup() error!\n", stderr);

	serverSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (serverSocket == INVALID_SOCKET)
		fputs("socket() error\n", stderr);

	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;		// 주소 전송을 위한 주소 체계, 항상 AF_INET 설정
	servAdr.sin_addr.s_addr = htonl(INADDR_ANY);	// IPv4 전송 주소(32-bit number)
	servAdr.sin_port = htons(5001);		// port 번호(16-bit number)

										// 생성 소켓에 IP주소 부여
	if (bind(serverSocket, (SOCKADDR*)&servAdr,
		sizeof(servAdr)) == SOCKET_ERROR)
		fputs("bind() error\n", stderr);

	getMyIP(&myIP); // 서버 IP 가져오기
	sprintf(myIP_char, "host adress : %d.%d.%d.%d",
		myIP.b1, myIP.b2, myIP.b3, myIP.b4);
	sprintf(nowNetworkState, "Waiting for connection ... ");

	if (listen(serverSocket, 5) == SOCKET_ERROR)
		fputs("listen() error\n", stderr);

	clntAdrSize = sizeof(clntAdr);

	clientSocket = accept(serverSocket,
		(SOCKADDR*)&clntAdr, &clntAdrSize);

	if (clientSocket == -1)
		fputs("accept() error\n", stderr);
	else
	{
		printf("Connected client [kks] \n");
		myNetworkState = NETWORK_WAIT;
	}


}

int getMyIP(IPv4 * myIP)
{
	char szBuffer[1024];

	if (gethostname(szBuffer,
		sizeof(szBuffer)) == SOCKET_ERROR)
	{
		return 0;
	}

	struct hostent *host =
		gethostbyname(szBuffer);
	if (host == NULL)
	{
		return 0;
	}

	//Obtain the computer's IP
	myIP->b1 = ((struct in_addr *)
		(host->h_addr))->S_un.S_un_b.s_b1;
	myIP->b2 = ((struct in_addr *)
		(host->h_addr))->S_un.S_un_b.s_b2;
	myIP->b3 = ((struct in_addr *)
		(host->h_addr))->S_un.S_un_b.s_b3;
	myIP->b4 = ((struct in_addr *)
		(host->h_addr))->S_un.S_un_b.s_b4;


	return 1;
}



void ServerGame_TestMode()
{

	char txt[64];
	sprintf(txt, "mouse Vector : (%f,%f)",
		mousePos.x, mousePos.y);

	glColor3f(1, 1, 1);
	PrintText(txt, 0.3, -0.95);

}