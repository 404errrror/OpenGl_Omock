#include<GL\glut.h>
#include<Windows.h>
void DoDisplay();
void Initialize();
void Test();

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	Initialize();
	glutCreateWindow("OpenGL_Omok");

	glutMainLoop();

}

void Initialize()
{
	glutInitWindowSize(1280, 720);
	//gluLookAt(0.0f, 0.0f, 1000.0f, 0.0f, 10.f, 0.0f, 0.0f, 1.0f, 0.0f);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);	 //������۸� Ȱ��ȭ.
	glEnable(GL_BLEND);								//���� Ȱ��ȭ
}

void DoDisplay()
{

}

