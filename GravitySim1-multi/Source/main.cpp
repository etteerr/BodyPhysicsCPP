#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <string>

//mouse move variables and setting
static int sx;
static int sy;
static double dx;
static double dy;
static bool s;
static double modifier = 2.5; //setting

//zoom variables / settings
static double zoom = 1; // zoom + inital value
static double zoomstep = 0.03; //zoom speed

//Program state
enum state {
	uninit, init, running, stopping, stopped
};

using namespace std;

void display(void) {

}

void printtext(int x, int y, string String) {
//(x,y) is from the bottom left of the window
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, glutGet(GLUT_WINDOW_WIDTH), 0, glutGet(GLUT_WINDOW_WIDTH), -1.0f,
			1.0f);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glPushAttrib(GL_DEPTH_TEST);
	glDisable(GL_DEPTH_TEST);
	glRasterPos2i(x, y);
	for (unsigned int i = 0; i < String.size(); i++) {
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, String[i]);
	}
	glPopAttrib();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void keyboard(int key, int mouse_x, int mouse_y) {
	if (key == GLUT_KEY_DOWN) {
		zoom -= zoomstep * zoom;
	}
	if (key == GLUT_KEY_UP) {
		zoom += zoomstep * zoom;
	}

}

void mouse(int button, int state, int x, int y) {
	using namespace std;
	if (!s) {
		sx = x;
		sy = y;
		s = true;
	} else {
		dx -= (((double) x - (double) sx) / (double) glutGet(GLUT_WINDOW_WIDTH))
				* (modifier / zoom);
		dy -=
				(((double) sy - (double) y)
						/ (double) glutGet(GLUT_WINDOW_HEIGHT))
						* (modifier / zoom);

		s = false;
	}
}

int main(int narg, char** args) {

	//Init openGL
	glutInit(&narg, args);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("N-Body Simulation");
	glutDisplayFunc(display);
	glutIdleFunc(display);
	//glutKeyboardFunc(keyboard);
	glutSpecialFunc(keyboard);
	glutMouseFunc(mouse);

	//Init graphics
	glClearColor(0, 0, 0, 0); //clear to white
	glShadeModel(GL_SMOOTH);
	glEnable(GL_POINT_SMOOTH);

	glViewport(0, 0, 800, 600);

	//Start
	glutMainLoop();

	return 0;
}
