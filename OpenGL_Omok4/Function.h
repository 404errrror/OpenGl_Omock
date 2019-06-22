#pragma once
#include"GL\glut.h""
#include<string.h>
#include<stdlib.h>
#include<Windows.h>
#include<stdio.h>
#include<math.h>
#include<time.h>
#include "Structs.h"


void PrintText(char* text, float posX, float posY);
void MoveAnimation(Vector2* myPos, AnimationInfo ani);
void SmoothMoveAnimation(Vector2* myPos, AnimationInfo ani, float* acceleration);
void SmoothStartMoveAnimation(Vector2* myPos, AnimationInfo ani, float* acceleration);
void SmoothEndMoveAnimation(Vector2* myPos, float dstX, float dstY, float speed);
void SmoothEndReshapeCirlceAnimation(float* myRad, float endRad, float speed);

GLubyte *LoadBmp(const char *Path, int *Width, int *Height);
GLubyte *LoadBmpBlur(const char *Path, int *Width, int *Height);
int _LoadImage(const char *Path, float posX, float posY, float scaleX, float scaleY);
int _LoadAlphaImage(const char *Path, const char *MaskPath, float posX, float posY, float scaleX, float scaleY);

void GaussianKernel(GLfloat* result,GLint kernelSize);

void ClippingMaskRect(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2);
void ClippingMaskCir(GLfloat rad, GLfloat x, GLfloat y);
void ClippingMaskCirReverse(GLfloat rad, GLfloat x, GLfloat y);

GLboolean CheckRectButton(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, GLfloat mouseX, GLfloat mouseY);

void DrawCircle(GLfloat rad, GLfloat x, GLfloat y);
void FpsCounter(GLint* fpsStorage, GLfloat* lastTime);


void PrintText(char* text, float posX, float posY)
{
	int length = (int)strlen(text);
	glRasterPos2f(posX, posY);

	for (int i = 0; i < length; i++)
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, text[i]);
	glFlush();
}

//�׳� �̵�
void MoveAnimation(Vector2* myPos, AnimationInfo ani)
{

	Vector2 moveWay;
	moveWay.x = (ani.end.x - ani.start.x) * 0.005;
	moveWay.y = (ani.end.y - ani.start.y) * 0.005;

	myPos->x += moveWay.x * ani.speed;
	myPos->y += moveWay.y *  ani.speed;
}

// ���� ��ġ, �ִϸ��̼� ����, ���ӵ� ������ ����.
void SmoothMoveAnimation(Vector2* myPos, AnimationInfo ani, float* acceleration)
{
	Vector2 distance;
	Vector2 moveDistance;
	distance.x = ani.end.x - ani.start.x;
	distance.y = ani.end.y - ani.start.y;

	moveDistance.x = myPos->x - ani.start.x;
	moveDistance.y = myPos->y - ani.start.x;

	if (distance.x < 0)
		distance.x *= -1;
	if (distance.y < 0)
		distance.y *= -1;

	if (moveDistance.x < 0)
		moveDistance.x *= -1;
	if (moveDistance.y < 0)
		moveDistance.y *= -1;

	if (distance.x != 0 && distance.x * 0.5 > moveDistance.x)
		SmoothStartMoveAnimation(myPos, ani, acceleration);//////////////////////////////
	else if (distance.x != 0 && distance.x * 0.5 <= moveDistance.x)
		SmoothEndMoveAnimation(myPos, ani.end.x, ani.end.y, ani.speed);
	else if (distance.y != 0 && distance.y * 0.5 > moveDistance.y)
		SmoothStartMoveAnimation(myPos, ani, acceleration);//////////////////////////////
	else if (distance.y != 0 && distance.y * 0.5 <= moveDistance.y)
		SmoothEndMoveAnimation(myPos, ani.end.x, ani.end.y, ani.speed);



}

// ������ �������� �̵�.
void SmoothStartMoveAnimation(Vector2* myPos, AnimationInfo ani, float* acceleration)
{

	Vector2 moveWay;
	moveWay.x = (ani.end.x - ani.start.x) * 0.005 * (*acceleration);
	moveWay.y = (ani.end.y - ani.start.y) * 0.005 * (*acceleration);

	myPos->x += moveWay.x * ani.speed;
	myPos->y += moveWay.y *  ani.speed;

	*acceleration += 0.1;

}

//���������� ������ ������ �����ϸ� �̵�
void SmoothEndMoveAnimation(Vector2* myPos, float dstX, float dstY, float speed)
{
	myPos->x += (dstX - myPos->x) * 0.05 * speed;
	myPos->y += (dstY - myPos->y) * 0.05 * speed;
}


//���ϴ� ũ�⸸ŭ ������ �����ϸ鼭 ũ�� ��ȯ.
void SmoothEndReshapeCirlceAnimation(float* myRad, float endRad, float speed)
{
	*myRad += (endRad - *myRad) * 0.05 * speed;
}

////////////////////////////////////////////// �̹��� �����Լ� //////////////////////////////////////////////////////////////

//�̹������� ���İ��� �ǵ��� ����� ����� �ϰ�, �簢���� �¾ƾ���. ���� �̹��� ũ��� 2�� �ŵ�����.
int _LoadImage(const char *Path, float posX, float posY, float scaleX, float scaleY)
{
	GLubyte *_data;
	int width, height;

	glEnable(GL_TEXTURE_2D);

	//glRasterPos2f(0.5, 0);
	//glutBitmapCharacter(GLUT_BITMAP_8_BY_13, "test");

	_data = LoadBmp(Path, &width, &height);
	//glDrawPixels(512, 512, GL_RGB, GL_UNSIGNED_BYTE, data);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, _data);
	free(_data);
	glDisable(GL_DEPTH_TEST); // <7>
	glEnable(GL_BLEND); // <8>
	glBlendFunc(GL_ONE, GL_ONE); // �ҽ��̹����� ����� ������, �� 0�̹Ƿ� ����̹����� �׷�����, �ҽ��̹����� �������� �ƴ� �κ��� ����ũ�� ���� 0�̹Ƿ� �ҽ��̹����� �׷�����.

	glBegin(GL_QUADS);
	{
		glColor3f(1, 1, 1);
		glTexCoord2f(0, 1);
		glVertex2f(posX - scaleX * 0.5, posY + scaleY * 0.5);
		glTexCoord2f(0, 0);
		glVertex2f(posX - scaleX * 0.5, posY - scaleY * 0.5);
		glTexCoord2f(1, 0);
		glVertex2f(posX + scaleX * 0.5, posY - scaleY * 0.5);
		glTexCoord2f(1, 1);
		glVertex2f(posX + scaleX * 0.5, posY + scaleY * 0.5);
	}
	glEnd();

	glutSwapBuffers();
	glFlush();

	return 0;
}

//����ũ �̹����� ����� ����̰� ������ �������̿����ϸ�, �� �̹����� ����� �������̿�����. �̹��� ũ��� 2�� �ŵ������̿��� ��.
int _LoadAlphaImage(const char *Path, const char *MaskPath, float posX, float posY, float scaleX, float scaleY)
{

	GLubyte *_data;
	int width, height;


	glEnable(GL_TEXTURE_2D);
	_data = LoadBmp(MaskPath, &width, &height);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, _data);
	free(_data);

	// �ؽ�ó ���� ����


	glBlendFunc(GL_DST_COLOR, GL_ZERO); // GL_DST_COLOR : RD GD BD AD, RGB�� �������ϋ� 0�̹Ƿ� DST�� ������� ������, RGB�� ����϶� 1�̹Ƿ� DST�� ���� ����. ���� ����� ���ŵǴ� ȿ��.


	glBegin(GL_QUADS);
	{
		glColor3f(1, 1, 1);
		glTexCoord2f(0, 1);
		glVertex2f(posX - scaleX * 0.5, posY + scaleY * 0.5);
		glTexCoord2f(0, 0);
		glVertex2f(posX - scaleX * 0.5, posY - scaleY * 0.5);
		glTexCoord2f(1, 0);
		glVertex2f(posX + scaleX * 0.5, posY - scaleY * 0.5);
		glTexCoord2f(1, 1);
		glVertex2f(posX + scaleX * 0.5, posY + scaleY * 0.5);
	}
	glEnd();

	_data = LoadBmp(Path, &width, &height);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, _data);
	free(_data);

	glBlendFunc(GL_ONE, GL_ONE); // �ҽ��̹����� ����� ������, �� 0�̹Ƿ� ����̹����� �׷�����, �ҽ��̹����� �������� �ƴ� �κ��� ����ũ�� ���� 0�̹Ƿ� �ҽ��̹����� �׷�����.

	glBegin(GL_QUADS);
	{
		glColor3f(1, 1, 1);
		glTexCoord2f(0, 1);
		glVertex2f(posX - scaleX * 0.5, posY + scaleY * 0.5);
		glTexCoord2f(0, 0);
		glVertex2f(posX - scaleX * 0.5, posY - scaleY * 0.5);
		glTexCoord2f(1, 0);
		glVertex2f(posX + scaleX * 0.5, posY - scaleY * 0.5);
		glTexCoord2f(1, 1);
		glVertex2f(posX + scaleX * 0.5, posY + scaleY * 0.5);
	}
	glEnd();

	glDisable(GL_TEXTURE_2D); 

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	return 0;

}

//������ byte* �� �޾ƿͼ� �׸��� �Լ��� ����� ���� ���� ���� ��. ���߿�.
//����ũ �̹����� ����� ����̰� ������ �������̿����ϸ�, �� �̹����� ����� �������̿�����. �̹��� ũ��� 2�� �ŵ������̿��� ��.
int _LoadPreloadAlphaImage(const char *Path, const char *MaskPath, float posX, float posY, float scaleX, float scaleY)
{

	GLubyte *_data;
	int width, height;


	glEnable(GL_TEXTURE_2D);
	//glRasterPos2f(-1, -1);
	//glDrawPixels(512, 512, GL_RGB, GL_UNSIGNED_BYTE, data);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, _data);

	// �ؽ�ó ���� ����
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);	 // MIN : ����Ҷ�, NEAREST : ���伥 �ִ��Դ� ȿ��, LINEAR : ���伥 �ε巴�� ȿ��.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);	// �� ����Ѱ� �����Ƿ� �ִ��Դ�.

	glDisable(GL_DEPTH_TEST); // <7>
	glEnable(GL_BLEND); // <8>
	glBlendFunc(GL_DST_COLOR, GL_ZERO); // GL_DST_COLOR : RD GD BD AD, RGB�� �������ϋ� 0�̹Ƿ� DST�� ������� ������, RGB�� ����϶� 1�̹Ƿ� DST�� ���� ����. ���� ����� ���ŵǴ� ȿ��.


	glBegin(GL_QUADS);
	{
		glColor3f(1, 1, 1);
		glTexCoord2f(0, 1);
		glVertex2f(posX - scaleX * 0.5, posY + scaleY * 0.5);
		glTexCoord2f(0, 0);
		glVertex2f(posX - scaleX * 0.5, posY - scaleY * 0.5);
		glTexCoord2f(1, 0);
		glVertex2f(posX + scaleX * 0.5, posY - scaleY * 0.5);
		glTexCoord2f(1, 1);
		glVertex2f(posX + scaleX * 0.5, posY + scaleY * 0.5);
	}
	glEnd();

	_data = LoadBmp(Path, &width, &height);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, _data);
	free(_data);
	glDisable(GL_DEPTH_TEST); // <7>
	glEnable(GL_BLEND); // <8>
	glBlendFunc(GL_ONE, GL_ONE); // �ҽ��̹����� ����� ������, �� 0�̹Ƿ� ����̹����� �׷�����, �ҽ��̹����� �������� �ƴ� �κ��� ����ũ�� ���� 0�̹Ƿ� �ҽ��̹����� �׷�����.

	glBegin(GL_QUADS);
	{
		glColor3f(1, 1, 1);
		glTexCoord2f(0, 1);
		glVertex2f(posX - scaleX * 0.5, posY + scaleY * 0.5);
		glTexCoord2f(0, 0);
		glVertex2f(posX - scaleX * 0.5, posY - scaleY * 0.5);
		glTexCoord2f(1, 0);
		glVertex2f(posX + scaleX * 0.5, posY - scaleY * 0.5);
		glTexCoord2f(1, 1);
		glVertex2f(posX + scaleX * 0.5, posY + scaleY * 0.5);
	}
	glEnd();

	glDisable(GL_TEXTURE_2D);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	return 0;

}



//�ð��� �ʹ� ���� �ɸ��� �ڷᵵ ���� �ȳ��´�.. �׳� ���伥���� ����.
GLubyte *LoadBmpBlur(const char *Path, int *Width, int *Height)
{
	HANDLE hFile;
	DWORD FileSize, dwRead;
	BITMAPFILEHEADER *fh = NULL;
	BITMAPINFOHEADER *ih;
	BYTE *pRaster;

	hFile = CreateFileA(Path, GENERIC_READ, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		return NULL;
	}

	FileSize = GetFileSize(hFile, NULL);
	fh = (BITMAPFILEHEADER *)malloc(FileSize);
	ReadFile(hFile, fh, FileSize, &dwRead, NULL);
	CloseHandle(hFile);

	int len = FileSize - fh->bfOffBits;
	pRaster = (GLubyte *)malloc(len);
	memcpy(pRaster, (BYTE *)fh + fh->bfOffBits, len);

	//�ӽ����� ����.
	//�� ó���Ҷ� ������ ������� �ϸ�, ������ �� ä������ ���� ���� ���� �� ���� ó���ϱ� ������
	//�ӽ������� ������� ������ �� ä�����ִ� ���� ���� ���� ó���ؾ��Ѵ�.
	BYTE* temp;
	temp = (GLubyte *)malloc(len);
	memcpy(temp, pRaster, len);



	// RGB�� ������ �ٲ۴�.
	for (BYTE *p = pRaster;p < pRaster + len - 3;p += 3) {
		BYTE b = *p ;
		*p = *(p + 2);

	}

	for (BYTE *p = temp;p < temp + len - 3;p += 3) {
		BYTE b = *p;
		*p = *(p + 2);

	}



	ih = (BITMAPINFOHEADER *)((PBYTE)fh + sizeof(BITMAPFILEHEADER));
	*Width = ih->biWidth;
	*Height = ih->biHeight;

	int widthIndex = 0;
	int tempIndex = 0;

	//width.
	for (BYTE *p = pRaster; p < pRaster + len - 3 ;p +=3)
	{
		widthIndex++;
		GLfloat gaussianResult[15];
		GaussianKernel(gaussianResult,15);

		if(widthIndex > 0 && widthIndex < *Width * *Height)
			for (GLint i = -7; i < 8; i++)
			{
				BYTE tempR = temp[i * 3 + tempIndex] * gaussianResult[i + 7];
				BYTE tempG = temp[i * 3 + 1 + tempIndex] * gaussianResult[i + 7];
				BYTE tempB = temp[i * 3 + 2 + tempIndex] * gaussianResult[i + 7];

				BYTE* tempP1 = (p);
				BYTE* tempP2 = (p + 1);
				BYTE* tempP3 = (p + 2);

				*tempP1 += tempR;
				*tempP2 += tempG;
				*tempP3 += tempB;


			}

		tempIndex += 3;

	}

	//for (int k = 0;k < 1;k++) {
	//	widthIndex = 0;
	//	tempIndex = 0;
	//	memcpy(temp, pRaster, len);	//�� �ߴ� �̹����� ���� �� ó��.

	//	//heigh
	//	for (BYTE *p = pRaster; p < pRaster + len - 3;p += 3)
	//	{
	//		widthIndex++;
	//		GLfloat gaussianResult[15];
	//		GaussianKernel(gaussianResult, 15);

	//		if (widthIndex > 0 && widthIndex < (*Width) * (*Height))
	//			for (GLint i = -7; i < 8; i++)
	//			{
	//				BYTE tempR = temp[i * 3 * *Width + tempIndex] * gaussianResult[i + 7];
	//				BYTE tempG = temp[i * 3 * *Width + 1 + tempIndex] * gaussianResult[i + 7];
	//				BYTE tempB = temp[i * 3 * *Width + 2 + tempIndex] * gaussianResult[i + 7];

	//				int test = i * 3 * *Width + tempIndex;

	//				BYTE* tempP1 = (p);
	//				BYTE* tempP2 = (p + 1);
	//				BYTE* tempP3 = (p + 2);

	//				*tempP1 += tempR;
	//				*tempP2 += tempG;
	//				*tempP3 += tempB;


	//			}

	//		tempIndex += 3;

	//	}
	//}
	free(fh);
	return pRaster;
}

// ����þ� Ŀ��. sigma : ǥ������, kernelSize : �ɰ�����(Ȧ��).
// �����͸� ���� ����, �ɰ� ����(Ȧ��). 3 ~ 15
void GaussianKernel(GLfloat* result,GLint kernelSize)
{

	
	//GLfloat c = 2 * sigma * sigma;
	//GLfloat* result = (GLfloat*)malloc(sizeof(GLfloat) * kernelSize);

	//for (GLint i = 0; i < kernelSize; i++)
	//{
	//	// ����þ� Ŀ�� ���� :  (-x^2/2sigma^2)E / ��2sigma^2 * PI
	//	GLint x = i - kernelSize / 2;

	//	result[i] = exp(-x * x / c) / sqrt(c * 3.14159265359);
	//}


	//����þ� Ŀ�� ��꿡 �ʹ� ���� ���ҽ��� �Ծ ����� �����ֱ����... �� �밡��
	//����� ǥ�������� 1.


	if (kernelSize == 5)
	{
		result[0] = 0.06136;
		result[1] = 0.24477;
		result[2] = 0.38774;
		result[3] = 0.24477;
		result[4] = 0.06136;
	}
	else if (kernelSize == 7)
	{
		result[0] = 0.00598;
		result[1] = 0.060626;
		result[2] = 0.241843;
		result[3] = 0.383103;
		result[4] = 0.241843;
		result[5] = 0.060626;
		result[6] = 0.00598;
	}
	else if (kernelSize == 9)
	{
		result[0] = 0.000229;
		result[1] = 0.005977;
		result[2] = 0.060598;
		result[3] = 0.241732;
		result[4] = 0.382928;
		result[5] = 0.241732;
		result[6] = 0.060598;
		result[7] = 0.005977;
		result[8] = 0.000229;
	}
	else if (kernelSize == 11)
	{
		result[0] = 0.000003;
		result[1] = 0.000229;
		result[2] = 0.005977;
		result[3] = 0.060598;
		result[4] = 0.24173;
		result[5] = 0.382925;
		result[6] = 0.24173;
		result[7] = 0.060598;
		result[8] = 0.005977;
		result[9] = 0.000229;
		result[10] = 0.000003;
	}
	else if (kernelSize == 13)
	{
		result[0] = 0;
		result[1] = 0.000003;
		result[2] = 0.000229;
		result[3] = 0.005977;
		result[4] = 0.060598;
		result[5] = 0.24173;
		result[6] = 0.382925;
		result[7] = 0.24173;
		result[8] = 0.060598;
		result[9] = 0.005977;
		result[10] = 0.000229;
		result[11] = 0.000003;
		result[12] = 0;
	}
	else if (kernelSize == 15)
	{
		//sigma3
		result[0] = 0.009033;
		result[1] = 0.018476;
		result[2] = 0.033851;
		result[3] = 0.055555;
		result[4] = 0.08167;
		result[5] = 0.107545;
		result[6] = 0.126854;
		result[7] = 0.134032;
		result[8] = 0.126854;
		result[9] = 0.107545;
		result[10] = 0.08167;
		result[11] = 0.055555;
		result[12] = 0.033851;
		result[13] = 0.018476;
		result[14] = 0.009033;

	}


}


GLubyte *LoadBmp(const char *Path, int *Width, int *Height)
{
	HANDLE hFile;
	DWORD FileSize, dwRead;
	BITMAPFILEHEADER *fh = NULL;
	BITMAPINFOHEADER *ih;
	BYTE *pRaster;

	hFile = CreateFileA(Path, GENERIC_READ, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		return NULL;
	}

	FileSize = GetFileSize(hFile, NULL);
	fh = (BITMAPFILEHEADER *)malloc(FileSize);
	ReadFile(hFile, fh, FileSize, &dwRead, NULL);
	CloseHandle(hFile);

	int len = FileSize - fh->bfOffBits;
	pRaster = (GLubyte *)malloc(len);
	memcpy(pRaster, (BYTE *)fh + fh->bfOffBits, len);

	// RGB�� ������ �ٲ۴�.
	for (BYTE *p = pRaster;p < pRaster + len - 3;p += 3) {
		BYTE b = *p;
		*p = *(p + 2);
		*(p + 2) = b;
	}

	ih = (BITMAPINFOHEADER *)((PBYTE)fh + sizeof(BITMAPFILEHEADER));
	*Width = ih->biWidth;
	*Height = ih->biHeight;

	free(fh);
	return pRaster;
}
////////////////////////////////////////////// �̹��� ���� �� //////////////////////////////////////////////////////////////



///////////////////////////////////////////////Ŭ���� ����ũ(���ٽ�)/////////////////////////////////////////////////////////

// (x1, y1) ���� (x2, y2) ���� �簢������ Ŭ���� ����ũ ����.
void ClippingMaskRect(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2)
{
	glClearStencil(0);
	glEnable(GL_STENCIL_TEST);
	glClear(GL_STENCIL_BUFFER_BIT);

	glStencilFunc(GL_NEVER, 0x0, 0x0);
	glStencilOp(GL_INCR, GL_INCR, GL_INCR);
	glRectf(x1, y1, x2, y2);

	glStencilFunc(GL_EQUAL, 0x1, 0x1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

}

// (x1, y1) ���� (x2, y2) ���� �簢������ �Ųٷ�(?)�� Ŭ���� ����ũ ����.
void ClippingMaskRectReverse(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2)
{
	glClearStencil(0);
	glEnable(GL_STENCIL_TEST);
	glClear(GL_STENCIL_BUFFER_BIT);

	glStencilFunc(GL_NEVER, 0x0, 0x0);
	glStencilOp(GL_INCR, GL_INCR, GL_INCR);
	glRectf(x1, y1, x2, y2);

	glStencilFunc(GL_NOTEQUAL, 0x1, 0x1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

}

//  (x,y)��ġ�� ������ ���̰� rad�� �� Ŭ���� ����ũ ����.
void ClippingMaskCir(GLfloat rad, GLfloat x, GLfloat y)
{
	glClearStencil(0);
	glEnable(GL_STENCIL_TEST);
	glClear(GL_STENCIL_BUFFER_BIT);

	glStencilFunc(GL_NEVER, 0x0, 0x0);
	glStencilOp(GL_INCR, GL_INCR, GL_INCR);
	glColor3f(1, 1, 1);
	DrawCircle(rad, x, y);

	glStencilFunc(GL_EQUAL, 0x1, 0x1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

}

//  (x,y)��ġ�� ������ ���̰� rad�� �� (�Ųٷ�?)Ŭ���� ����ũ. Ŭ���� ����ũ ������ ������ �κи� �׷���.
void ClippingMaskCirReverse(GLfloat rad, GLfloat x, GLfloat y)
{
	glClearStencil(0);
	glEnable(GL_STENCIL_TEST);
	glClear(GL_STENCIL_BUFFER_BIT);

	glStencilFunc(GL_NEVER, 0x0, 0x0);
	glStencilOp(GL_INCR, GL_INCR, GL_INCR);
	glColor3f(1, 1, 1);
	DrawCircle(rad, x, y);

	glStencilFunc(GL_NOTEQUAL, 0x1, 0x1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

}


void RenderScene()
{
	GLdouble dAngle;        // Looping variable

							// Clear blue window
	//glClearColor(0.0f, 0.0f, 1.0f, 0.0f);

	// Use 0 for clear stencil, enable stencil test
	glClearStencil(0.0f);
	glEnable(GL_STENCIL_TEST);

	// Clear stencil buffer
	glClear(GL_STENCIL_BUFFER_BIT);

	// ���Ŀ� �׷����� ���� �ƹ��͵� �׷����� ������
	// ���ٽ� ���۾ȿ� �׷��� ������ 0x1�� �ٲ��.
	glStencilFunc(GL_NEVER, 0x0, 0x0);					//�ƹ��͵� �����Ű�� �ʱ�.
	glStencilOp(GL_INCR, GL_INCR, GL_INCR);				//ù��° �Ķ��Ÿ : ���ٽ� ���� ���м�, �ι�° : ���ٽ� ���� ����, ���� ����, ����° ���ٽ�, ���� �Ѵ� ����.

														// Spiral pattern will create stencil pattern
														// Draw the spiral pattern with white lines. We
														// make the lines  white to demonstrate that the
														// stencil function prevents them from being drawn
	glColor3f(1, 1.0f, 1.0f);
	glBegin(GL_LINE_STRIP);
	for (dAngle = -100; dAngle < 400.0; dAngle += 0.1)
	{
		glVertex2f(-10, dAngle);
		glVertex2f(10, dAngle);
	}
	glEnd();

	// 0x1 && 0x1 = 0x1 �� �ƴѰ��� ���.
	// ���� ������ �׷ȴ� ������ ������ ���� ������ϴ� ���� ����Ѵ�.
	glStencilFunc(GL_NOTEQUAL, 0x1, 0x1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);		//���ٽ� ���� �ȿ��ִ� ���� ����.

												// ���ٽ� ���ۿ� 0x1�� �ƴѰ��� �簢���� �׸���.
												// ��, �տ� ���� �׸� ���� ������ ���� �簢���� �׸���. 

	glColor3f(1.0f, 0.0f, 0.0f);
	glRectf(0, 0, 0 + 0.5, 0 - 0.5);

	glDisable(GL_STENCIL_TEST);					//���ٽ� ��� ��Ȱ��ȭ.
	// All done, do the buffer swap
}



// (x1, y1) ���� (x2, y2)������ ��ư �������� ����. ���콺�� ��ư ���� ���� ������ 
GLboolean CheckRectButton(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, GLfloat mouseX, GLfloat mouseY)
{
	mouseX = mouseX/(1280 * 0.5) - 1;
	mouseY = (mouseY/(720 * 0.5) - 1) * -1;

	if (mouseX >= x1 && mouseX <= x2
		&& mouseY <= y1 && mouseY >= y2)
		return 1;
	else
		return 0;
}




//�� �׸��� �Լ�. rad : ������ -> �ػ� ������ y�� 1.78�� �������Ƿ� ũ�� ����.
void DrawCircle(GLfloat rad, GLfloat posX, GLfloat posY)
{
	glBegin(GL_POLYGON);
	for (int i = 0;i<360;i++)
	{
		double angle = i*3.141592 / 180;
		double x = rad*cos(angle);
		double y = rad*sin(angle )* 1.78;
		glVertex2f(x + posX, y + posY);
	}
	glEnd();
}

GLint fpsTmp = 0;
void FpsCounter(GLint* fpsStorage, GLfloat* lastTime)
{
	GLfloat currentTime = clock();
	fpsTmp++;

	if (currentTime - (*lastTime) >= 1000)
	{
		*lastTime = clock();
		*fpsStorage = fpsTmp;
		fpsTmp = 0;
	}
	
}