#include <GL/freeglut.h>
//function declerations
void display(void);
void resize(int, int);
void keyboard(unsigned char, int, int);
void mouse(int, int, int, int);
void initialize(void);
void uninitialize(void);

bool bFullscreen = false;

static int shoulder = 0, elbow = 0;

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
	glClear(GL_COLOR_BUFFER_BIT);
	
	glPushMatrix();
	
	glTranslatef(-1.0f,0.0f,0.0f);
	glRotatef((GLfloat)shoulder,0.0f,0.0f,1.0f);
	glTranslatef(1.0f,0.0f,0.0f);
	glPushMatrix();
	glScalef(2.0f,0.4,1.0f);
	glutWireCube(1.0f);
	glPopMatrix();

	glTranslatef(1.0f,0.0f,0.0f);
	glRotatef((GLfloat)elbow,0.0f,0.0f,1.0f);
	glTranslatef(1.0f,0.0f,0.0f);
	glPushMatrix();
	glScalef(2.0f,0.4f,1.0f);
	glutWireCube(1.0);
	glPopMatrix();
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
	case 's':
		shoulder = (shoulder + 5)%360;
		glutPostRedisplay();
		break;
	case 'S':
		shoulder = (shoulder - 5) % 360;
		glutPostRedisplay();
		break;
	case 'e':
		elbow = (elbow + 3) % 360;
		glutPostRedisplay();
		break;
	case 'E':
		elbow = (elbow - 3) % 360;
		glutPostRedisplay();
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
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0f, (GLfloat)width / (GLfloat)height, 1.0f, 20.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

}

void uninitialize(void)
{

}

