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

//그냥 이동
void MoveAnimation(Vector2* myPos, AnimationInfo ani)
{

	Vector2 moveWay;
	moveWay.x = (ani.end.x - ani.start.x) * 0.005;
	moveWay.y = (ani.end.y - ani.start.y) * 0.005;

	myPos->x += moveWay.x * ani.speed;
	myPos->y += moveWay.y *  ani.speed;
}

// 현재 위치, 애니메이션 정보, 가속도 저장할 변수.
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

// 서서히 빨라지게 이동.
void SmoothStartMoveAnimation(Vector2* myPos, AnimationInfo ani, float* acceleration)
{

	Vector2 moveWay;
	moveWay.x = (ani.end.x - ani.start.x) * 0.005 * (*acceleration);
	moveWay.y = (ani.end.y - ani.start.y) * 0.005 * (*acceleration);

	myPos->x += moveWay.x * ani.speed;
	myPos->y += moveWay.y *  ani.speed;

	*acceleration += 0.1;

}

//도착점까지 서서히 느리게 감속하며 이동
void SmoothEndMoveAnimation(Vector2* myPos, float dstX, float dstY, float speed)
{
	myPos->x += (dstX - myPos->x) * 0.05 * speed;
	myPos->y += (dstY - myPos->y) * 0.05 * speed;
}


//원하는 크기만큼 서서히 감속하면서 크기 변환.
void SmoothEndReshapeCirlceAnimation(float* myRad, float endRad, float speed)
{
	*myRad += (endRad - *myRad) * 0.05 * speed;
}

////////////////////////////////////////////// 이미지 관리함수 //////////////////////////////////////////////////////////////

//이미지에는 알파값을 의도한 배경이 없어야 하고, 사각형에 맞아야함. 물론 이미지 크기는 2의 거듭제곱.
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
	glBlendFunc(GL_ONE, GL_ONE); // 소스이미지의 배경은 검정색, 즉 0이므로 배경이미지가 그려지고, 소스이미지가 검정색이 아닌 부분은 마스크로 인해 0이므로 소스이미지가 그려진다.

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

//마스크 이미지의 배경은 흰색이고 내용은 검정색이여야하며, 본 이미지의 배경은 검정색이여야함. 이미지 크기는 2의 거듭제곱이여야 함.
int _LoadAlphaImage(const char *Path, const char *MaskPath, float posX, float posY, float scaleX, float scaleY)
{

	GLubyte *_data;
	int width, height;


	glEnable(GL_TEXTURE_2D);
	_data = LoadBmp(MaskPath, &width, &height);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, _data);
	free(_data);

	// 텍스처 필터 설정


	glBlendFunc(GL_DST_COLOR, GL_ZERO); // GL_DST_COLOR : RD GD BD AD, RGB가 검은색일떄 0이므로 DST에 관계없이 검은색, RGB가 흰색일때 1이므로 DST의 색상에 따라감. 따라서 흰색이 제거되는 효과.


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

	glBlendFunc(GL_ONE, GL_ONE); // 소스이미지의 배경은 검정색, 즉 0이므로 배경이미지가 그려지고, 소스이미지가 검정색이 아닌 부분은 마스크로 인해 0이므로 소스이미지가 그려진다.

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

//데이터 byte* 를 받아와서 그리는 함수를 만들면 좀더 성능 좋을 것. 나중에.
//마스크 이미지의 배경은 흰색이고 내용은 검정색이여야하며, 본 이미지의 배경은 검정색이여야함. 이미지 크기는 2의 거듭제곱이여야 함.
int _LoadPreloadAlphaImage(const char *Path, const char *MaskPath, float posX, float posY, float scaleX, float scaleY)
{

	GLubyte *_data;
	int width, height;


	glEnable(GL_TEXTURE_2D);
	//glRasterPos2f(-1, -1);
	//glDrawPixels(512, 512, GL_RGB, GL_UNSIGNED_BYTE, data);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, _data);

	// 텍스처 필터 설정
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);	 // MIN : 축소할때, NEAREST : 포토샵 최단입단 효과, LINEAR : 포토샵 부드럽게 효과.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);	// 난 깔끔한게 좋으므로 최단입단.

	glDisable(GL_DEPTH_TEST); // <7>
	glEnable(GL_BLEND); // <8>
	glBlendFunc(GL_DST_COLOR, GL_ZERO); // GL_DST_COLOR : RD GD BD AD, RGB가 검은색일떄 0이므로 DST에 관계없이 검은색, RGB가 흰색일때 1이므로 DST의 색상에 따라감. 따라서 흰색이 제거되는 효과.


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
	glBlendFunc(GL_ONE, GL_ONE); // 소스이미지의 배경은 검정색, 즉 0이므로 배경이미지가 그려지고, 소스이미지가 검정색이 아닌 부분은 마스크로 인해 0이므로 소스이미지가 그려진다.

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



//시간이 너무 많이 걸리고 자료도 많이 안나온다.. 그냥 포토샵에서 하자.
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

	//임시파일 생성.
	//블러 처리할때 원본을 대상으로 하면, 색상이 다 채워지기 전의 값을 토대로 또 블러를 처리하기 때문에
	//임시파일을 대상으로 색상이 다 채워져있는 값을 토대로 블러를 처리해야한다.
	BYTE* temp;
	temp = (GLubyte *)malloc(len);
	memcpy(temp, pRaster, len);



	// RGB로 순서를 바꾼다.
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
	//	memcpy(temp, pRaster, len);	//블러 했던 이미지를 통해 블러 처리.

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

// 가우시안 커널. sigma : 표준편차, kernelSize : 쪼갤갯수(홀수).
// 데이터를 담을 공간, 쪼갤 갯수(홀수). 3 ~ 15
void GaussianKernel(GLfloat* result,GLint kernelSize)
{

	
	//GLfloat c = 2 * sigma * sigma;
	//GLfloat* result = (GLfloat*)malloc(sizeof(GLfloat) * kernelSize);

	//for (GLint i = 0; i < kernelSize; i++)
	//{
	//	// 가우시안 커널 공식 :  (-x^2/2sigma^2)E / √2sigma^2 * PI
	//	GLint x = i - kernelSize / 2;

	//	result[i] = exp(-x * x / c) / sqrt(c * 3.14159265359);
	//}


	//가우시안 커널 계산에 너무 많은 리소스를 먹어서 결과값 던져주기로함... 는 노가다
	//참고로 표준편차는 1.


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

	// RGB로 순서를 바꾼다.
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
////////////////////////////////////////////// 이미지 관리 끝 //////////////////////////////////////////////////////////////



///////////////////////////////////////////////클리핑 마스크(스텐실)/////////////////////////////////////////////////////////

// (x1, y1) 에서 (x2, y2) 까지 사각형으로 클리핑 마스크 생성.
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

// (x1, y1) 에서 (x2, y2) 까지 사각형으로 거꾸로(?)된 클리핑 마스크 생성.
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

//  (x,y)위치에 반지름 길이가 rad인 원 클리핑 마스크 생성.
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

//  (x,y)위치에 반지름 길이가 rad인 원 (거꾸로?)클리핑 마스크. 클리핑 마스크 영역을 제외한 부분만 그려짐.
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

	// 이후에 그려지는 것은 아무것도 그려지지 않지만
	// 스텐실 버퍼안에 그려진 영역은 0x1로 바뀐다.
	glStencilFunc(GL_NEVER, 0x0, 0x0);					//아무것도 통과시키지 않기.
	glStencilOp(GL_INCR, GL_INCR, GL_INCR);				//첫번째 파라메타 : 스텐실 버퍼 실패서, 두번째 : 스텐실 버퍼 성공, 깊이 실패, 세번째 스텐실, 깊이 둘다 성공.

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

	// 0x1 && 0x1 = 0x1 이 아닌곳에 허용.
	// 따라서 위에서 그렸던 영영을 제외한 곳에 드로잉하는 것을 허용한다.
	glStencilFunc(GL_NOTEQUAL, 0x1, 0x1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);		//스텐실 버퍼 안에있는 값을 유지.

												// 스텐실 버퍼에 0x1이 아닌곳에 사각형을 그린다.
												// 즉, 앞에 선을 그린 곳을 제외한 곳에 사각형을 그린다. 

	glColor3f(1.0f, 0.0f, 0.0f);
	glRectf(0, 0, 0 + 0.5, 0 - 0.5);

	glDisable(GL_STENCIL_TEST);					//스텐실 기능 비활성화.
	// All done, do the buffer swap
}



// (x1, y1) 에서 (x2, y2)까지를 버튼 영역으로 설정. 마우스가 버튼 영역 내에 있을때 
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




//원 그리는 함수. rad : 반지름 -> 해상도 때문에 y는 1.78을 곱했으므로 크기 주의.
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