#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <string>
#include <iostream>
#define nbodyerrorverbose 1
#include "NbodySim.h"
#include "Dwarves.h"
#include "Particle.h"

//Simulator
enbody::NbodySim simulator;
enbody::Particle * particles;
unsigned int nParticles;

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

	if (key == GLUT_KEY_HOME) {
		simulator.pauseSimulation();
	}

	if (key == GLUT_KEY_END) {
		simulator.resumeSimulation();
	}

	if (key == GLUT_KEY_PAGE_UP) {
		simulator.setRealtimeFraction(simulator.getRealtimeFraction() + 0.01);
	}
	if (key == GLUT_KEY_PAGE_DOWN) {
		simulator.setRealtimeFraction(simulator.getRealtimeFraction() - 0.01);
	}

	if (key == GLUT_KEY_F5) {
		simulator.setRealtimeFraction(-1);
	}
	if (key == GLUT_KEY_F6) {
		simulator.setRealtimeFraction(1);
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
	using namespace std;
	//init sim
	//TODO: commandline init with variables
	//	spread from origin (mean=0, alpha=?)
	//	mean mass
	//  sd mass
	// nparticles
	if (narg !=  5) {
		cout << "Using default values..." << endl
			 << "Use GravitySim1-multi [spread] [meanMass] [sdMass] [nParticles]" << endl
			 << "[spread] = 0.0 0.0 1.0 1.0		Means from (0,0) to (1,1) particles may appear" << endl
			 << "[meanMass] = 10.0  		    Means mass is 10.0 +- sdMass" << endl
			 << "[sdMass] = 5.0					Means see [meanMass]" << endl
			 << "[nParticles] = 1024			Means amount of particles to generate" << endl;
		//Init default
		enbody::NbodySim simulator(512);
		simulator.setDT(0.01);
		simulator.setNormalSize(0.1, 0.05);
		simulator.setNormalWeight(5.0, 1.0);
		simulator.setWorkingSector(0,0);
		simulator.initRNG(0);
		simulator.addParticles(1024);

		particles = simulator.enableReadBuffer();

		simulator.setRealtimeFraction(-1);
		simulator.pauseSimulation();
		simulator.startSimulation();


	}else
		return 1;

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
