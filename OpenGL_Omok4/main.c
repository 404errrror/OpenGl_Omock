#include"GL\glut.h"
#include<Windows.h>
#include"Start.h"
#include"SceneManager.h"
//
void Initialize();
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	Initialize();
	glutCreateWindow("OpenGL_Omok");

	glutDisplayFunc(SceneManager_DoDisplay);

	glutMainLoop();

}

void Initialize()
{

	glutInitWindowSize(1280, 720);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_STENCIL);	 //더블버퍼링 활성화, 스텐실 버퍼.
	glEnable(GL_BLEND);								//블렌드 활성화

}