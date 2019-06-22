#pragma once
#include "Function.h"

void DrawBoard(Vector2 pos, GLfloat scale, GLint amount);
void DrawStone(BoardData board, GLfloat x, GLfloat y);
void DrawAllStone(BoardData board);

void CalliStone(BoardData board, GLfloat* x, GLfloat* y);
Vector2 CallibratorToArray(BoardData board, GLfloat x, GLfloat y);

GLint PutStone(BoardData* board, GLfloat* x, GLfloat* y);
void PutStoneNoCali(BoardData* board, GLint x, GLint y);
GLint Win_Check(BoardData board, GLint x, GLint y, Vector2* winPos);
void WinStencilEffect(BoardData board, Vector2* winPos);
void AI_WinStencilEffect(BoardData board, Vector2* winPos);




void DrawBoard(Vector2 pos, GLfloat scale, GLint amount)
{
	glLineWidth(4);
	glBegin(GL_LINE_LOOP);
	{
		glVertex2f(pos.x - scale * 0.5,( pos.y + scale * 0.5) * 1.78);
		glVertex2f(pos.x - scale * 0.5, (pos.y - scale * 0.5)* 1.78);
		glVertex2f(pos.x + scale * 0.5, (pos.y - scale * 0.5)* 1.78);
		glVertex2f(pos.x + scale * 0.5, (pos.y + scale * 0.5)* 1.78);
	}
	glEnd();

	GLfloat lineSpace = scale / (amount-1);

	glLineWidth(2);
	glBegin(GL_LINES);
	for (int i = 1; i < amount - 1; i++)
	{
		glVertex2f(pos.x - scale * 0.5, ((pos.y + scale * 0.5) - lineSpace * i) * 1.78);
		glVertex2f(pos.x + scale * 0.5, ((pos.y + scale * 0.5) - lineSpace * i) * 1.78);

		glVertex2f((pos.x - scale * 0.5) + lineSpace * i, (pos.y + scale * 0.5) * 1.78);
		glVertex2f((pos.x - scale * 0.5) + lineSpace * i, (pos.y - scale * 0.5) * 1.78);
	}
	glEnd();

}

void CalliStone(BoardData board, GLfloat* x, GLfloat* y)
{

	GLfloat startPointX = board.pos.x - board.scale * 0.5;
	GLfloat startPointY = (board.pos.y + board.scale * 0.5) * 1.78;
	GLfloat lineSpace = board.scale / (board.amount - 1);
	Vector2 distance; Vector2 nowPos;

	if (*x < startPointX || *x > startPointX + board.scale
		|| *y > startPointY || *y < startPointY - board.scale * 1.78)
		return;



	for (int i = 0; i < board.amount; i++) {
		distance.x = startPointX + i * lineSpace - *x;
		distance.y = startPointY - i * lineSpace* 1.78 - *y;

		nowPos.x = startPointX + i * lineSpace;
		nowPos.y = startPointY - i * lineSpace* 1.78;


		if (distance.x > lineSpace * -0.5 && distance.x <= lineSpace * 0.5)
			*x = nowPos.x;

		if (distance.y > lineSpace* 1.78 * -0.5 && distance.y <= lineSpace* 1.78 * 0.5)
			*y = nowPos.y ;

	}

}

// x,y���� �޾ƿͼ� ������ �迭�� �°� ��ȯ�ؼ�, �� ��ġ�� ��ȯ.
Vector2 CallibratorToArray(BoardData board, GLfloat x, GLfloat y)
{
	GLfloat startPointX = board.pos.x - board.scale * 0.5;
	GLfloat startPointY = (board.pos.y + board.scale * 0.5) * 1.78;
	GLfloat lineSpace = board.scale / (board.amount - 1);
	Vector2 distance; Vector2 nowPos;
	Vector2 result;
	GLint putPosX, putPosY;
	putPosX = putPosY = -1;

	for (int i = 0; i < board.amount; i++) {
		distance.x = startPointX + i * lineSpace - x;
		distance.y = startPointY - i * lineSpace* 1.78 - y;

		nowPos.x = startPointX + i * lineSpace;
		nowPos.y = startPointY - i * lineSpace* 1.78;

		if ((distance.x > lineSpace * -0.5 && distance.x <= lineSpace * 0.5) && putPosX == -1)
			putPosX = i;

		if ((distance.y > lineSpace* 1.78 * -0.5 && distance.y <= lineSpace* 1.78 * 0.5) && putPosY == -1)
			putPosY = i;

	}
	result.x = putPosX; result.y = putPosY;
	return result;

}

// �迭�� ��ġ�� �޾ƿͼ� �����ǿ� �°� ��ȯ�ؼ�, �� ��ǥ�� ��ȯ.
Vector2 CallibratorToBoard(BoardData board, GLfloat x, GLfloat y)
{
	GLfloat startPointX = board.pos.x - board.scale * 0.5;
	GLfloat startPointY = (board.pos.y + board.scale * 0.5) * 1.78;
	GLfloat lineSpace = board.scale / (board.amount - 1);
	Vector2 result;


	result.x = + startPointX + x * lineSpace; result.y = startPointY - y * lineSpace * 1.78;
	return result;

}

// x, y ���� ����� ��ġ�� ������ ����� �ϳ��� �׸�.
void DrawStone(BoardData board, GLfloat x, GLfloat y)
{
	CalliStone(board, &x, &y);


	if (board.nowTurn == BLACK)
	{
		glColor4f(1, 1, 1, 0.3);
		DrawCircle((board.scale / board.amount) * 0.5, x, y);
		glColor3f(0.0117, 0.6627, 0.9568);
		DrawCircle((board.scale / board.amount) * 0.45, x, y);

	}
	else if (board.nowTurn == WHITE)
	{

		glColor4f(1, 1, 1, 0.3);
		DrawCircle((board.scale / board.amount) * 0.5, x, y);
	}
}

// ��� ����˵��� �׸���.
void DrawAllStone(BoardData board)
{
	GLfloat startPointX = board.pos.x - board.scale * 0.5;
	GLfloat startPointY = board.pos.y + board.scale * 0.5 * 1.78;
	GLfloat lineSpace = board.scale / (board.amount - 1);

	for (int i = 0; i < board.amount; i++)
		for (int j = 0; j < board.amount; j++)
		{
			if (board.boardArr[i][j] == BLACK) {
				glColor3f(1, 1, 1);
				DrawCircle((board.scale / board.amount) * 0.5, startPointX + i * lineSpace, startPointY - j * lineSpace * 1.78);
				glColor3f(0.0117, 0.6627, 0.9568);
				DrawCircle((board.scale / board.amount) * 0.45, startPointX + i * lineSpace, startPointY - j * lineSpace * 1.78);
			}
			else if(board.boardArr[i][j] == WHITE)
			{
				glColor3f(1, 1, 1);
				DrawCircle((board.scale / board.amount) * 0.5, startPointX + i * lineSpace, startPointY - j * lineSpace * 1.78);
			}
		}
}

// ������� �迭�� �����Ѵ�. ���� �������� 1, ���������� 0�� ��ȯ.
GLint PutStone(BoardData* board, GLfloat* x, GLfloat* y)
{

	//����� ��ġ ã��.
	Vector2 putPos;
	putPos = CallibratorToArray(*board, *x, *y);

	if (board->boardArr[(GLint)putPos.x][(GLint)putPos.y] == EMPTY)
	{
		if (board->nowTurn == BLACK)
		{
			board->boardArr[(GLint)putPos.x][(GLint)putPos.y] = BLACK;
			board->nowTurn = WHITE;
		}
		else if (board->nowTurn == WHITE)
		{
			board->boardArr[(GLint)putPos.x][(GLint)putPos.y] = WHITE;
			board->nowTurn = BLACK;
		}
		return 1;
	}
	return 0;

}

// �迭��ġ�� �ٷ� �޾ƿͼ� ���� ���´�.
void PutStoneNoCali(BoardData* board, GLint x, GLint y)
{


	if (board->boardArr[x][y] == EMPTY)
	{
		if (board->nowTurn == BLACK)
		{
			board->boardArr[x][y] = BLACK;
			board->nowTurn = WHITE;
		}
		else if (board->nowTurn == WHITE)
		{
			board->boardArr[x][y] = WHITE;
			board->nowTurn = BLACK;
		}
	}

}


// BLACK�� �̱�� BLACK�� ��ȯ, WHITE�� �̱�� WHITE�� ��ȯ, winPos�� 5���� �����Ų ������� ��ġ.
GLint Win_Check(BoardData board, GLint x, GLint y, Vector2* winPos)
{
	int i;
	int current;
	int count = 0, win = 0;



	if (board.nowTurn == BLACK)
		current = WHITE;
	else
		current = BLACK;

	////���� ���� �ʾƵ� ȣ�� �Ǳ� ������ ���� ���Ҵ��� üũ�� ���־�� �Ѵ�.
	//if (board.boardArr[x][y] == EMPTY || board.boardArr[x][y] != current)
	//	return 0;

	// y�� üũ.
	for (i = 1; i < 5; i++) {
		if (board.boardArr[x][y + i] == current)
		{
			winPos[count].x = x;
			winPos[count].y = y + i;
			count++;
		}
		else
			break;
	}
	for (i = 1; i < 5; i++) {
		if (board.boardArr[x][y - i] == current)
		{
			winPos[count].x = x;
			winPos[count].y = y - i;
			count++;
		}
		else
			break;
	}

	if (count >= 4)
		win = 1;
	else
	count = 0;

	//x�� üũ
	for (i = 1; i < 5 && win != 1; i++) {
		if (board.boardArr[x + i][y] == current)
		{
			winPos[count].x = x + i;
			winPos[count].y = y;
			count++;
		}
		else
			break;
	}
	for (i = 1; i < 5 && win != 1; i++) {
		if (board.boardArr[x - i][y] == current)
		{
			winPos[count].x = x - i;
			winPos[count].y = y;
			count++;
		}
		else
			break;
	}

	if (count >= 4)
		win = 1;
	else
	count = 0;

	//�� üũ
	for (i = 1; i < 5 && win != 1; i++) {
		if (board.boardArr[x - i][y - i] == current)
		{
			winPos[count].x = x - i;
			winPos[count].y = y - i;
			count++;
		}
		else
			break;
	}
	for (i = 1; i < 5 && win != 1; i++) {
		if (board.boardArr[x + i][y + i] == current)
		{
			winPos[count].x = x + i;
			winPos[count].y = y + i;
			count++;
		}
		else
			break;
	}

	if (count >= 4)
		win = 1;
	else
	count = 0;

	//�� üũ
	for (i = 1; i < 5 && win != 1; i++) {
		if (board.boardArr[x + i][y - i] == current)
		{
			winPos[count].x = x + i;
			winPos[count].y = y - i;
			count++;
		}
		else
			break;
	}
	for (i = 1; i < 5 && win != 1; i++) {
		if (board.boardArr[x - i][y + i] == current)
		{
			winPos[count].x = x - i;
			winPos[count].y = y + i;
			count++;
		}
		else
			break;
	}

	if (count >= 4) {
		win = 1;
		winPos[count].x = x;
		winPos[count].y = y;
	}


	
	if (win) {
		if (current == WHITE)
			return WHITE;
		else if(current == BLACK)
		return BLACK;
	}

	return 0;

}

void WinStencilEffect(BoardData board, Vector2* winPos)
{
	glClearStencil(0);
	glEnable(GL_STENCIL_TEST);
	glClear(GL_STENCIL_BUFFER_BIT);

	glStencilFunc(GL_NEVER, 0x0, 0x0);
	glStencilOp(GL_INCR, GL_INCR, GL_INCR);
	glColor3f(1, 1, 1);
	for (int i = 0; winPos[i].x != -300; i++)
		DrawCircle((board.scale / board.amount) * 0.5, winPos[i].x, winPos[i].y);

	if (board.nowTurn == BLACK)
		glRectf(0.35, -0.141667, 0.75, -0.858333);
	else if (board.nowTurn == WHITE)
		glRectf(0.35, 0.858333, 0.75, 0.141667);

	glStencilFunc(GL_NOTEQUAL, 0x1, 0x1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
}

void AI_WinStencilEffect(BoardData board, Vector2* winPos)
{
	glClearStencil(0);
	glEnable(GL_STENCIL_TEST);
	glClear(GL_STENCIL_BUFFER_BIT);

	glStencilFunc(GL_NEVER, 0x0, 0x0);
	glStencilOp(GL_INCR, GL_INCR, GL_INCR);
	glColor3f(1, 1, 1);
	for (int i = 0; winPos[i].x != -300; i++)
		DrawCircle((board.scale / board.amount) * 0.5, winPos[i].x, winPos[i].y);

	if (board.nowTurn == BLACK)
		glRectf(0.35, -0.101667, 0.75, -0.898333);
	else if (board.nowTurn == WHITE)
		glRectf(0.35, 0.858333, 0.75, 0.141667);

	glStencilFunc(GL_NOTEQUAL, 0x1, 0x1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
}