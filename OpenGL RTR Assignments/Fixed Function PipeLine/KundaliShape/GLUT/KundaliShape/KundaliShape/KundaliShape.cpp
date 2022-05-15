#include <GL/freeglut.h>
//function declerations
void display(void);
void resize(int, int);
void keyboard(unsigned char, int, int);
void mouse(int, int, int, int);
void initialize(void);
void uninitialize(void);

bool bFullscreen = false; 
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

void display(void)
{

	glClear(GL_COLOR_BUFFER_BIT);
	//glFlush(); Step4. Remove the flush call and call swapbuffer instead of it
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	GLfloat point1 = 0.5f;
	GLfloat point2 = -0.5f;
	glBegin(GL_LINE_LOOP);

	glColor3f(1.0f, 0.0f, 0.0f);

	glVertex3f(point2, point1, 0.0f);
	glVertex3f(point2, point2, 0.0f);

	glVertex3f(point1, point2, 0.0f);
	glVertex3f(point1, point1, 0.0f);

	glColor3f(0.0f, 0.0f, 1.0f);

	glEnd();

	glBegin(GL_LINE_LOOP);

	//using mid-point formula
	glVertex3f((point2 + point2) / 2, (point1 + point2) / 2, 0.0f);
	glVertex3f((point2 + point1) / 2, (point2 + point2) / 2, 0.0f);
	glVertex3f((point1 + point1) / 2, (point1 + point2) / 2, 0.0f);
	glVertex3f((point1 + point2) / 2, (point1 + point1) / 2, 0.0f);

	glEnd();

	glBegin(GL_LINES);

	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(point1, point1, 0.0f);
	glVertex3f(point2, point2, 0.0f);

	glVertex3f(point2, point1, 0.0f);
	glVertex3f(point1, point2, 0.0f);
	glEnd();

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

