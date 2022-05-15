#include <GL/freeglut.h>
#include<math.h>
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

	glLineWidth(1.0f);

	glBegin(GL_LINES);

	GLfloat point = 0.0f;
	GLfloat pointY = 1.0f;

	pointY = pointY / 20;

	for (int i = 0; i <= 20; i++)
	{
		i == 0 ? glColor3f(0.0f, 0.0f, 1.0f) : glColor3f(1.0f, 0.0f, 0.0f);

		glVertex3f(1.0f, point, 0.0f);
		glVertex3f(-1.0f, point, 0.0f);

		i == 0 ? glColor3f(0.0f, 1.0f, 0.0f) : glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3f(point, 1.0f, 0.0f);
		glVertex3f(point, -1.0f, 0.0f);

		point = point + pointY;

	}
	point = 0.0f;

	for (int j = 0; j < 20; j++)
	{
		glColor3f(1.0f, 0.0f, 0.0f);
		point = point - pointY;
		glVertex3f(1.0f, point, 0.0f);
		glVertex3f(-1.0f, point, 0.0f);

		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3f(point, 1.0f, 0.0f);
		glVertex3f(point, -1.0f, 0.0f);
	}
	glEnd();


	glBegin(GL_POINTS);
	glColor3f(1.0f, 1.0f, 1.0f);
	const float pi = 3.1415f;
	for (float angle = 0.0f;angle < 2.0f*pi;angle = angle + 0.01f)
	{
		glVertex3f(cos(angle), sin(angle), 0.0f);

	}
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

