#include <GL/freeglut.h>
//function declerations
void display(void);
void resize(int, int);
void keyboard(unsigned char, int, int);
void mouse(int, int, int, int);
void initialize(void);
void uninitialize(void);

bool bFullscreen = false;
static int year = 0, day = 0;

int main(int argc, char** argv)
{

	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

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
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT );
	glColor3f(1.0f,1.0f,1.0f);
	glPushMatrix();
	glutWireSphere(1.0f,20.0,16.0);
	glRotatef((GLfloat)year,0.0f,1.0f,0.0f);
	glTranslatef(2.0f,0.0f,0.0f);
	glRotatef((GLfloat)day,0.0f,1.0f,0.0f);
	glutWireSphere(0.2f,10,8);
	glPopMatrix();

	glutSwapBuffers();
}

void initialize(void)
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glShadeModel(GL_FLAT);
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
	case 'd':
		day = (day - 10) % 360;
		glutPostRedisplay();
		break;
	case 'D':
		day = (day + 10) % 360;
		glutPostRedisplay();
		break;
	case 'y':
		year = (year + 5) % 360;
		glutPostRedisplay();
		break;
	case 'Y':
		year = (year - 5) % 360;
		glutPostRedisplay();
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
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0f,(GLfloat)width/(GLfloat)height,1.0f,20.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

}

void uninitialize(void)
{

}

