#define _dt 0.0001

#include <iostream>
#include <math.h>
#include <GL/glut.h>
#include "nbody-fun.h"

const int nParticles = 2;
double seed = 0;
particle arr1[nParticles];
double simtime = 0;

void display(void) {
	//std::cout << simtime << std::endl;
	step(arr1, nParticles);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BITS);

	glBegin(GL_POINTS);
		glPointSize(3.0);
		glColor3f(1.0, 1.0, 1.0);
		for (int i = 0; i < nParticles; i++)
			glVertex2f(arr1[i].pos.x, arr1[i].pos.y);
	glEnd();
	glFlush();
	glFinish();
	glutSwapBuffers();
	simtime+= _dt;
}

void keyboard(unsigned char key, int mouse_x, int mouse_y) {

}

void mouse(int button, int state, int x, int y) {

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
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);

	//Init graphics
	glClearColor(0,0,0.2,0); //clear to darkblue
	glShadeModel(GL_SMOOTH);
	glEnable(GL_POINT_SMOOTH);

	glViewport(0,0, 800,600);

	//set output precision (cout text)
	std::cout.precision(17);
	//Report simulation details
	std::cout << "Starting simulation using:" << std::endl
			<< "	nParticles: " << nParticles << std::endl
			<< "	detla t:    " << _dt << std::endl
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
