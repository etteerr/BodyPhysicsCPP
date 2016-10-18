#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <string>
#include <iostream>
#include <fstream>
#include <chrono>
#define nbodyerrorverbose 1
#define PI 3.141592653589793238
#define pointsInCircle 120
#include "NbodySim.h"
#include "Dwarves.h"
#include "Particle.h"

//Simulator
enbody::NbodySim * simulator;
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
static double zoom = 0.008; // zoom + inital value
static double zoomstep = 0.03; //zoom speed

//Program state
enum state {
	uninit, init, running, stopping, stopped
};

using namespace std;
void printtext(int x, int y, string String)
{
//(x,y) is from the bottom left of the window
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, glutGet(GLUT_WINDOW_WIDTH), 0,glutGet(GLUT_WINDOW_WIDTH), -1.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glPushAttrib(GL_DEPTH_TEST);
    glDisable(GL_DEPTH_TEST);
    glRasterPos2i(x,y);
    for (unsigned int i=0; i<String.size(); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, String[i]);
    }
    glPopAttrib();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

void drawCircle(vec2d p, double r) {
	if (r<=0)
		return;
	double x,y;
	glBegin(GL_POLYGON);
	{
		for(unsigned int i = 0; i < pointsInCircle; i++) {
			double angle = ((2*PI*i)/pointsInCircle);
			x = cos(angle)*r;
			y = sin(angle)*r;
			glVertex2d(zoom*(x+p.x-dx),zoom*(y+p.y-dy));
		}
	}
	glEnd();

}

void display(void) {
	auto start = std::chrono::high_resolution_clock::now();
	nParticles = simulator->getNParticles();
	//if (!simulator->updateBuffer())
	//	exit(1);
	//auto getdt = std::chrono::high_resolution_clock::now();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_ACCUM_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glColor3f(0,.4,1.0);
	for(unsigned int i = 0; i < nParticles; i++) {
		drawCircle(particles[i].getPosition(), particles[i].getRadius());
	}
	auto drawdt = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> dur1, dur2;
	//dur1 = getdt - start;
	dur2 = drawdt -start;// getdt;
	std::chrono::milliseconds ms1, ms2;
	ms1 = std::chrono::duration_cast<std::chrono::milliseconds>(dur1);
	ms2 = std::chrono::duration_cast<std::chrono::milliseconds>(dur2);
	glColor3f(1.0,1.0,1.0);
	char a[120];
	sprintf(a,"SimSpeed %.8f  simstep: %i   dtDraw: %i    dtCalc: %.5f", simulator->getRealtimeFraction(), simulator->getSimulatedSteps(), ms2.count(), simulator->getCalcTime());
	printtext(10,30,a);
	char str[64];
	sprintf(str,"Zoom: %.8f  loc: %.2f:%.2f    time: %.5f", zoom, dx,dy,(double)simulator->getSimulatedSteps()*simulator->deltaT);
	printtext(10,10,str);
	glFlush();
	glFinish();
	glutSwapBuffers();
	this_thread::sleep_for(std::chrono_literals::operator ""ms(15));
}


void keyboard(int key, int mouse_x, int mouse_y) {
	if (key == GLUT_KEY_DOWN) {
		zoom -= zoomstep * zoom;
	}
	if (key == GLUT_KEY_UP) {
		zoom += zoomstep * zoom;
	}

	if (key == GLUT_KEY_HOME) {
		simulator->pauseSimulation();
	}

	if (key == GLUT_KEY_END) {
		simulator->resumeSimulation();
	}

	if (key == GLUT_KEY_PAGE_UP) {
		simulator->setRealtimeFraction(simulator->getRealtimeFraction() * 1.1);
	}
	if (key == GLUT_KEY_PAGE_DOWN) {
		simulator->setRealtimeFraction(simulator->getRealtimeFraction() / 1.1);
	}

	if (key == GLUT_KEY_F5) {
		simulator->setRealtimeFraction(-1);
	}
	if (key == GLUT_KEY_F6) {
		simulator->setRealtimeFraction(1);
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


void parsecmd(int narg, char** args) {

}

int main(int narg, char** args) {
	using namespace std;
	//init sim
	//TODO: commandline init with variables
	//	spread from origin (mean=0, alpha=?)
	//	mean mass
	//  sd mass
	// nparticles

	//Init default
	simulator = new enbody::NbodySim(8);
	simulator->setDT(1.0);
	simulator->setNormalSize(100, 50);
	simulator->setNormalWeight(5000000000000.0, 1000.0);
	//simulator->setWorkingSector(0,0);
	simulator->initRNG(0);

	//particles = simulator->enableReadBuffer();
	particles = simulator->particleArrayPointer;

	simulator->addParticles(30, 1000.0);

	//Init sim (start paused)
	simulator->setRealtimeFraction(1);
	simulator->pauseSimulation();
	simulator->startSimulation();


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

	delete simulator;

	return 0;
}
