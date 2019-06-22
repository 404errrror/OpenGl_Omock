#pragma once

#define BOARD_SIZE 19
#define EMPTY 0
#define WHITE 1
#define BLACK 2

#define LOCAL 3
#define MULTI 4
#define SERVER 5
#define CLIENT 6

#define NETWORK_NOT_CONNECT 7
#define NETWORK_WAIT 8
#define NETWORK_PLAY 9

typedef struct Vector2
{
	GLfloat x;
	GLfloat y;

}Vector2;

typedef struct AnimationInfo
{
	Vector2 start;
	Vector2 end;
	GLfloat speed;
}AnimationInfo;

typedef struct Color3
{
	GLfloat r;
	GLfloat g;
	GLfloat b;
}Color3;

typedef struct Color4
{
	GLfloat r;
	GLfloat g;
	GLfloat b;
	GLfloat a;
}Color4;

typedef struct BoardData
{
	Vector2 pos;
	GLfloat scale;
	GLint amount;
	GLint nowTurn;
	GLint boardArr[BOARD_SIZE][BOARD_SIZE];
}BoardData;

typedef struct IPv4
{
	unsigned char b1, b2, b3, b4;
}IPv4;