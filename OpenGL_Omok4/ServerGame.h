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


// ���� ������.
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

// ó�� �� ȿ��.
GLfloat blueCirRad;

// ������ �� ��ȿ��.
GLfloat endCirRad;

// ������ �ִϸ��̼� �������� üũ.
GLboolean endAnimationState;

// ó�� ������ �ٽ� ���ư��� ȿ��
GLfloat returnCirRad;

// ��� ���İ� ����Ʈ
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
		 GLubyte strTmp[4];	// sprintf�� �ڿ� �ι��ڸ� �ڵ����� �߰��ϱ⶧���� �� ���.
		 GLint resvPosX; GLint resvPosY;
		 sprintf(strTmp, "");

		 if (myNetworkState != NETWORK_NOT_CONNECT)
			 strLen = recv(clientSocket, message, 12, 0);
		 if (strLen == 0 || strLen == -1)
			 continue;

		 // strcmp �ϸ� ���ڿ� ������ �˻��ϱ⶧���� ������ Ʋ���� �ȴ�.
		 if (strncmp(message, "END",3) == 0)
		 {
			 isWin = 1;

			 // �̱����� END ����. �̱��� NetworkState ����.
			 //�� ���� END �޾ƿ�. �� ���� ������ �ݰ� NetworkState ����. �̱������� END ����. ������ ����
			 // �̱����� END �޾ƿ�. ������ �ݰ� ������ ����.
			 ServerGame_CloseSocket();
		 }


		 // secv���� ��ٸ��ٰ� �������Ƿ� �ѹ��� �˻��������.
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

	// ������
	DrawBoard(boardData.pos, 1, BOARD_SIZE);
	if (inBoard)
		DrawStone(boardData, mousePos.x, mousePos.y);
	DrawAllStone(boardData);

	// �ð����� ���
	ServerGame_PrintTime();

	//��� ĳ���� ���
	_LoadAlphaImage("GameResources/BlackCharacter.bmp", "GameResources/BlackCharacterMask.bmp", 0.55, 0.5, 1 * 0.4, 1 * 1.78* 0.4);
	ClippingMaskCir(blueCirRad, 0, 0);
	_LoadAlphaImage("GameResources/WhiteCharacter.bmp", "GameResources/WhiteCharacterMask.bmp", 0.55, -0.5, 1 * 0.4, 1 * 1.78 * 0.4);
	glDisable(GL_STENCIL_TEST);

	// ��� ĳ���� ���� ����Ʈ
	if (isWin == 0) {
		glColor4f(0, 0, 0, 0.1);
		ClippingMaskRect(0.35, 0.858333, 0.75, 0.141667);
		DrawCircle(BlackPersonRad, 0.55, 0.5);
		glDisable(GL_STENCIL_TEST);
		ClippingMaskRect(0.35, -0.141667, 0.75, -0.858333);
		DrawCircle(WhitePersonRad, 0.55, -0.5);
		glDisable(GL_STENCIL_TEST);
	}

	///////////////////////üũ//////////////////////////////////////////////////
	// �̰��� ��
	if (isWin)
	{
		if (endTime)
		{
			ClippingMaskRectReverse(0.5 - 0.01, 0.06, 0.5 + 0.11, -0.03);

			// �̱� ĳ���� �ʿ��� ���İ��� �ȵ�����.
			if (boardData.nowTurn == BLACK)
				ClippingMaskRectReverse(0.35, -0.141667, 0.75, -0.858333);
			else if (boardData.nowTurn == WHITE)
				ClippingMaskRectReverse(0.35, 0.858333, 0.75, 0.141667);

			if (myState == SERVER)
				closesocket(serverSocket);
			closesocket(clientSocket);
			WSACleanup();
		}
		// Ÿ�ӿ����� ������ �ƴҶ� �̱� ����˿� Ŭ���� ����ũ ����.
		else
			WinStencilEffect(boardData, winPos);



		glColor4f(0, 0, 0, 0.5);
		DrawCircle(endCirRad, 0, 0);
		glDisable(GL_STENCIL_TEST);

		// Ŭ���� �� ���� �ؽ�Ʈ�� ������ �ؽ�Ʈ�� �� �Ⱥ����� �ణ ���İ� ����.
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

		//�ð����� �̰��, �������� �̰�� ���� ���� ����� �� ��.
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

/////////////////////////////////////////////////////////////////// �ʱ�ȭ/////////////////////////////////////////////////////

void ServerGame_Init()
{
	___isInit = 1;
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);	 // MIN : ����Ҷ�, NEAREST : ���伥 �ִ��Դ� ���� ȿ��, LINEAR : ���伥 ���� �ε巴�� ȿ��.
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

//////////////////////////////////////// ���콺 /////////////////////////////////////////

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
			// �߰�
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
			//���� �𸣰����� ��Ʈ��ũ������ ������ _animationNum �� 0���� �ٲ�.
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




//////////////////////////////////////// Ÿ�̸� ////////////////////////////////////////
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




////////////////////////////////////// ���� /////////////////////////////////////////////////////////////////




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


	// ��Ʈ��ũ
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

	// ����ü �� ���������� �𸣰�����, ������ �����ϸ� Ŭ���̾�Ʈ�� returnCirRad�� 0�� �ǹ���.
	returnCirRad = 1.2;

}

////////////////////////////////////////////////////////////////////////////////////////

void ServerGame_NetworkServer()
{

	// ��Ʈ��ũ
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
	servAdr.sin_family = AF_INET;		// �ּ� ������ ���� �ּ� ü��, �׻� AF_INET ����
	servAdr.sin_addr.s_addr = htonl(INADDR_ANY);	// IPv4 ���� �ּ�(32-bit number)
	servAdr.sin_port = htons(5001);		// port ��ȣ(16-bit number)

										// ���� ���Ͽ� IP�ּ� �ο�
	if (bind(serverSocket, (SOCKADDR*)&servAdr,
		sizeof(servAdr)) == SOCKET_ERROR)
		fputs("bind() error\n", stderr);

	getMyIP(&myIP); // ���� IP ��������
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