#include <GL/freeglut.h>
#include<math.h>
//function declerations
void display(void);
void resize(int, int);
void keyboard(unsigned char, int, int);
void mouse(int, int, int, int);
void initialize(void);
void uninitialize(void);

int circle_points = 100;
#define PI 3.1412

bool bFullscreen = false; 
enum eColor
{
	eRed = 1,
	eGreen = 2,
	eBlue = 3,
	eGray = 4,
	eOrange = 5,
	eWhite = 6,
	eCyan = 7,
	eMagenta = 8,
	eYellow = 9,
	eBrown = 10
};
int main(int argc, char** argv)
{
	
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA|GLUT_DEPTH);

	glutInitWindowSize(800, 800); 
	glutInitWindowPosition(100, 100); 
	glutCreateWindow("MyWindow ");

	initialize();

	glutDisplayFunc(display);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutCloseFunc(uninitialize);

	glutMainLoop();

	//	return(0); 
}
void ApplyColors(int iColorCode)
{
	switch (iColorCode)
	{
	case eColor::eRed:
		glColor3f(1.0f, 0.0f, 0.0f);
		break;
	case eColor::eGreen:
		glColor3f(0.0f, 1.0f, 0.0f);
		break;
	case eColor::eBlue:
		glColor3f(0.0f, 0.0f, 1.0f);
		break;
	case eColor::eGray:
		glColor3f(0.75f, 0.75f, 0.75f);
		break;
	case eColor::eOrange:
		glColor3f(0.8f, 0.25f, 0.0f);
		break;
	case eColor::eWhite:
		glColor3f(1.0f, 1.0f, 1.0f);
		break;
	case eColor::eCyan:
		glColor3f(0.0f, 1.0f, 1.0f);
		break;
	case eColor::eMagenta:
		glColor3f(1.0f, 0.0f, 1.0f);
		break;
	case eColor::eYellow:
		glColor3f(1.0f, 1.0f, 0.0f);
		break;
	case eColor::eBrown:
		glColor3f(0.75f, 0.25f, 0.25f);
		break;
	}
}
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	//glFlush(); Step4. Remove the flush call and call swapbuffer instead of it
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glLineWidth(1.0f);
	GLfloat j = 1.0f;
	GLfloat parts = 1.0f / 10;
	int iColorCounter = 1;
	for (j = 1.0f; j > 0.0f; j = j - parts)
	{
		glBegin(GL_LINE_LOOP);
		ApplyColors(iColorCounter++);

		for (int i = 0;i<circle_points;i++)
		{

			GLfloat angle = 0.0f;
			angle = 2 * PI*i / circle_points;
			glVertex3f(cos(angle)*j, sin(angle)*j, 0.0f);
		}
		glEnd();
	}
	glutSwapBuffers();
}

void initialize(void)
{
	
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); 
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27: 
		glutLeaveMainLoop();
		break;
	case 'F':
	case 'f':
		if (bFullscreen == false)
		{
			glutFullScreen();
			bFullscreen = true;
		}
		else
		{
			glutLeaveFullScreen();
			bFullscreen = false;
		}
		break;
	default:
		break;
	}
}

void mouse(int button, int state, int x, int y)
{
	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		break;
	default:
		break;
	}
}

void resize(int width, int height)
{
	if (height == 0)
		height = 1;
	
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);

}

void uninitialize(void)
{

}

