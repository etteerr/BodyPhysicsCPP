#define _dt 0.001
#define _MAX_WEIGHT 100000000.0
#define _MIN_WEIGHT 100.0
#define _START_SIZE -2.0,2.0


#include <iostream>
#include <math.h>
#include <GL/glut.h>
#include "nbody-fun.h"

//sim settings
const int nParticles = 1024;
double seed = 0;
particle arr1[nParticles];
double simtime = 0;

//mouse move variables and setting
static int sx;
static int sy;
static double dx;
static double dy;
static bool s;
static double modifier = 2.5; //setting

//zoom variables / settings
static double zoom = 1; //start zoom
static double zoomstep = 0.03; //zoom speed
static double minParticle = 2.0; //Min particle size
static double normParticle = 3.0; //norm particle size (2mx2m screen size if zoom=1)

void display(void) {
	//std::cout << simtime << std::endl;
	step(arr1, nParticles);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_ACCUM_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	if (normParticle*zoom > minParticle)
		glPointSize(normParticle*zoom);
	else
		glPointSize(minParticle);
	glBegin(GL_POINTS);

		for (int i = 0; i < nParticles; i++) {
			glColor3f(arr1[i].mass/_MAX_WEIGHT, 0.0, 1.0-arr1[i].mass/_MAX_WEIGHT);
			glVertex2f((arr1[i].pos.x -dx)*zoom, (arr1[i].pos.y -dy)*zoom);
		}
	glEnd();
	glFlush();
	glFinish();
	glutSwapBuffers();
	simtime+= _dt;
}

void keyboard(int key, int mouse_x, int mouse_y) {
	if (key==GLUT_KEY_DOWN) {
		zoom-=zoomstep * zoom;
	}
	if(key==GLUT_KEY_UP) {
		zoom += zoomstep * zoom;
	}

	std::cout << zoom << std::endl;
}


void mouse(int button, int state, int x, int y) {
	using namespace std;
	if (!s) {
		sx = x;
		sy = y;
		s = true;
	}else {
		dx -= (((double)x-(double)sx)/(double)glutGet(GLUT_WINDOW_WIDTH))*(modifier/zoom);
		dy -= (((double)sy-(double)y)/(double)glutGet(GLUT_WINDOW_HEIGHT))*(modifier/zoom);

		s = false;
	}
}

int main(int narg, char** args) {



	//Init arrays
		//Global

	//Generate particles
	fill_random(&arr1[0], nParticles,seed);

	//Init openGL
	glutInit(&narg, args);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(800,600);
	glutInitWindowPosition(0,0);
	glutCreateWindow("N-Body Simulation");
	glutDisplayFunc(display);
	glutIdleFunc(display);
	//glutKeyboardFunc(keyboard);
	glutSpecialFunc(keyboard);
	glutMouseFunc(mouse);

	//Init graphics
	glClearColor(0,0,0,0); //clear to white
	glShadeModel(GL_SMOOTH);
	glEnable(GL_POINT_SMOOTH);

	glViewport(0,0, 800,600);

	//set output precision (cout text)
	std::cout.precision(17);
	//Report simulation details
	std::cout << "Starting simulation using:" << std::endl
			<< "	nParticles: " << nParticles << std::endl
			<< "	delta t:    " << _dt << std::endl
			<< "	precision:  " << sizeof(double) << " bytes" << std::endl;
	//Simulate
	//for(double ctime = 0; ctime < simtime; ctime+=_dt) {
		//step(arr1, nParticles);
		//std::cout << "Update, time: " << ctime << std::endl;
		//std::cout << arr1[0].loc.x << ":" << arr1[0].loc.y << std::endl;
	//}
	glutMainLoop();

	return 0;
}
