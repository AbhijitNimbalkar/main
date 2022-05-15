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
	GLfloat glDelta = 1.0f / 11;

	glClear(GL_COLOR_BUFFER_BIT);
	//glFlush(); Step4. Remove the flush call and call swapbuffer instead of it
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glLineWidth(1.0f);

	int iColorCounter = 1;
	GLfloat point = 0.5f;


	glBegin(GL_LINE_LOOP);
	glColor3f(1.0f, 0.0f, 0.0f);



	glVertex3f(0.0f, point, 0.0f);
	glVertex3f(-point, -point, 0.0f);
	glVertex3f(point, -point, 0.0f);
	glEnd();

	glBegin(GL_LINES);
	glColor3f(1.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, 0.5f, 0.0f);
	glVertex3f((-point + point) / 2, (-point + -point) / 2, 0.0f);
	glEnd();

	GLfloat lenthAB = sqrt(pow(-point, 2.0) + pow((-point - point), 2.0));

	GLfloat lenthBC = sqrt(pow((point + point), 2.0) + pow((-point + point), 2.0));
	GLfloat lenthAC = sqrt(pow(-point, 2.0) + pow((point + point), 2.0));


	GLfloat sideOfTraingle = 1.0f;
	GLfloat pPerimeter = lenthAB + lenthBC + lenthAC; // as this is equilateral traingle
	GLfloat semiPerimeter = pPerimeter / 2;

	GLfloat xIncentreOfTriangle = ((lenthBC*(0.0f)) + (lenthAC*-point) + (lenthAB*point)) / pPerimeter;
	GLfloat yIncentreOfTriangle = ((lenthBC*point) + (lenthAC*-point) + (lenthAB*-point)) / pPerimeter;


	GLfloat radius = sqrt(semiPerimeter*(semiPerimeter - lenthAB)*(semiPerimeter - lenthBC)*(semiPerimeter - lenthAC)) / semiPerimeter;

	glBegin(GL_LINE_LOOP);
	glColor3f(0.0f, 0.0f, 1.0f);
	for (size_t i = 0; i < circle_points; i++)
	{
		GLfloat angle = 0.0f;
		angle = 2 * PI*i / circle_points;
		glVertex3f((cos(angle)*radius) + xIncentreOfTriangle, (sin(angle)*radius) + yIncentreOfTriangle, 0.0f);
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

