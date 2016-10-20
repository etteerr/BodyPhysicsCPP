#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#define nbodyerrorverbose 0
#define PI 3.141592653589793238
#define pointsInCircle 120
#include "NbodySim.h"
#include "Dwarves.h"
#include "Particle.h"

//Simulator
enbody::NbodySim * simulator;
enbody::Particle * particles;
unsigned int nParticles;
unsigned long int maxTraceDraw = 500000; //30 to 40 ms rendertime
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

void drawTrace() {
	try {
	glPointSize(1.0);
	double x,y;
	unsigned int csteps = simulator->getSimulatedSteps();
	if (csteps > maxTraceDraw)
		csteps = maxTraceDraw;
	glBegin(GL_POINTS);
	std::lock_guard<std::mutex> lock(simulator->loggerlock);
	for(unsigned int j = 0; j < simulator->nLoggers; j++) {
		long unsigned int i = 0;
		for(auto it = simulator->loggers[j].log.end(); it != simulator->loggers[j].log.begin(); it--) {
			glColor3d((double)i/(double)csteps, 1.0-((double)i/(double)csteps), 0.0);
			x = (*it).x;
			y = (*it).y;
			glVertex2d(zoom*(x-dx),zoom*(y-dy));
			i++;
			if (i >= maxTraceDraw)
				break;
		}
	}
	glEnd();
	}catch(std::exception * e) {
		//cout << e->what() << endl;
	}
}

void display(void) {
	auto start = std::chrono::high_resolution_clock::now();
	nParticles = simulator->getNParticles();
	particles = simulator->particleArrayPointer;
	//if (!simulator->updateBuffer())
	//	exit(1);
	//auto getdt = std::chrono::high_resolution_clock::now();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_ACCUM_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glColor3f(0,.4,1.0);
	for(unsigned int i = 0; i < nParticles; i++) {
		drawCircle(particles[i].getPosition(), particles[i].getRadius());
	}
	drawTrace();
	auto drawdt = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> dur1, dur2;
	//dur1 = getdt - start;
	dur2 = drawdt -start;// getdt;
	std::chrono::milliseconds ms1, ms2;
	ms1 = std::chrono::duration_cast<std::chrono::milliseconds>(dur1);
	ms2 = std::chrono::duration_cast<std::chrono::milliseconds>(dur2);
	glColor3f(1.0,1.0,1.0);
	char a[120];
	sprintf(a,"SimSpeed %.8f  simstep: %i   dtDraw: %i    dtCalc: %.5f", simulator->getRealtimeFraction(), simulator->getSimulatedSteps(), (int)ms2.count(), simulator->getCalcTime());
	printtext(10,30,a);
	char str[64];
	sprintf(str,"Zoom: %.10f  loc: %.2f:%.2f    time: %.5f", zoom, dx,dy,(double)simulator->getSimulatedSteps()*(double)simulator->deltaT);
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
		simulator->setRealtimeFraction(simulator->getRealtimeFraction() + 0.01);
	}
	if (key == GLUT_KEY_PAGE_DOWN) {
		simulator->setRealtimeFraction(simulator->getRealtimeFraction() - 0.01);
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

bool parsescript(std::string file); //required
bool parseCommand(string incommands) {
	unsigned int _nParticles = simulator->getNParticles();
	std::istringstream iss;
	iss.str(incommands);
	std::string commands[256],command;
	int i = 0;
	std::string item;
	while(std::getline(iss, item, ' ')) {
		commands[i++] = item;
	}
	int nargs = i;
	i = 0;
	command = commands[i++];

	//static variables
	static unsigned int selected = 0;
	static bool hasSelected = false;

	//vars
	bool wasPaused = simulator->isPaused();

	//stand alone commands

	if (command == string("script")) {
		simulator->pauseSimulation();
		string f = commands[i++];
		if (!parsescript(f)) {
			cout << "failed to parse script" << endl;
			return false;
		}
		if (!wasPaused) simulator->resumeSimulation();
		return true;
	}

	if (command == string("addrng")) {
		if (nargs < 5) {
			cout << "Invalid usage" << endl <<
					"Use: addrng [amount] [scale] [startx] [starty]" << endl;
			return false;
		}
		simulator->pauseSimulation();
		int n = atoi(commands[i++].data());
		double s = atof(commands[i++].data());
		int x = atof(commands[i++].data());
		int y = atof(commands[i++].data());
		simulator->addParticles(n,s,vec2d(x,y));
		if (!wasPaused) simulator->resumeSimulation();
		return true;
	}
	if (command == string("setview"))
	{
		if (nargs == 3){
			dx = atof(commands[i++].data());
			dy = atof(commands[i++].data());
			return true;
		}
		return false;
	}

	if (command == string("setzoom"))
		{
			if (nargs == 2){
				zoom = atof(commands[i++].data());
				return true;
			}
			return false;
		}

	if (command == string("wait")) { //wait in seconds  (simulator time)
		command = commands[i++];
		if (command == string("until")) { //wait until simulator time
			double t;
			t = atof(commands[i++].data());
			while (t > (simulator->deltaT*(double)simulator->getSimulatedSteps())) {
				this_thread::yield();
			}
		}else
		if (command == string("for")) {
			double t;
			t = atof(commands[i++].data());
			t+=(simulator->deltaT*(double)simulator->getSimulatedSteps());
			while (t > (simulator->deltaT*(double)simulator->getSimulatedSteps())) {
				this_thread::yield();
			}
		}else {
			cout << "Invalid usage of wait." << endl <<
					"use: wait for [time in seconds] " << endl <<
					"or" << endl <<
					"wait until [time in seconds]" << endl <<
					"where time is simulator time, not real time." << endl;
		}
		return true;
	}

	if (command == string("quit")) {
		exit(0);//TODO: Dont do this
	}
	if (command == string("savelog")) {
		if (nargs != 2)
			return false;
		simulator->pauseSimulation();
		fstream f;
		f.open(commands[i++].data(), fstream::out | fstream::app);
		f.precision(std::numeric_limits<long double>::digits10);
		for(unsigned int i = 0; i < simulator->nLoggers; i++) {
			f << simulator->loggers[i].id << ";";
		}
		f << endl;
		for (unsigned int j = 0; j < simulator->getSimulatedSteps(); j++) {
			for(unsigned int i = 0; i < simulator->nLoggers; i++) {
				f << simulator->loggers[i].log[j].x << "-" << simulator->loggers[i].log[j].y << ";";
			}
			f << endl;
		}
		f.close();
		if (!wasPaused) simulator->resumeSimulation();
		return true;
	}

	if (command == string("addlog")) {
		simulator->pauseSimulation();
		simulator->addLogger(stoul(commands[i++].data()));
		if (!wasPaused) simulator->resumeSimulation();
		return true;
	}

	if (command == string("add")) {
		simulator->pauseSimulation();
		enbody::Particle newPar;
		double speed,x,y,px,py;
		if (nargs>1) { //args given
			while(i < nargs) {
				command = commands[i++];
				if (command == string("weight")) {
					newPar.mass = strtold(commands[i++].data(),0);
				}
				if (command == string("speed")) {

					speed = strtold(commands[i++].data(),0);
				}
				if (command == string("direction")) {
					x = strtold(commands[i++].data(),0);
					y = strtold(commands[i++].data(),0);
				}
				if (command == string("position")) {
					px = strtold(commands[i++].data(),0);
					py = strtold(commands[i++].data(),0);
				}
				if (command == string("radius")) {
					newPar.radius = strtold(commands[i++].data(),0);
				}
			}

		}else {
			cout << "weight: ";
			cin >> newPar.mass;
			cout << endl << "direction x: ";
			cin >> x;
			cout << endl << "direction y: ";
			cin >> y;
			cout << endl << "position x: ";
			cin >> px;
			cout << endl << "position y: ";
			cin >> py;
			cout << endl << "speed: ";
			cin >> speed;
			cout << endl << "radius: ";
			cin >> newPar.radius;
			cout << endl;
		}
		newPar.velocity = vec2d(x,y).normalize() * speed;
		newPar.position = vec2d(px,py);
		simulator->addParticle(newPar);
		if (!wasPaused) simulator->resumeSimulation();
		return true;
	}

	if (command == string("select")) {
		selected = stoul(commands[i++].data());
		if (selected < 0 || selected >= _nParticles) {
			cout << "Invalid selected" << endl;
			selected = 0;
			return false;
		}
		hasSelected = true;
		return true;
	}

	if (command == string("unselect")) {
		hasSelected = false;
		return true;
	}

	if (command == string("list")) {
		for (unsigned int i = 0; i < _nParticles; i++)
			particles[i].print(i);
		return true;
	}

	if (command == string("identify")) {
		simulator->pauseSimulation();
		//print id for every planet
		//if (!wasPaused) simulator->resumeSimulation();
		cout << "Simulation automatically paused" << endl;
		return true;
	}
	if (command == string("pause")) {
		simulator->pauseSimulation();
		return true;
	}
	if (command == string("resume")) {
		simulator->resumeSimulation();
		return true;
	}
	if (command == string("setSpeed")) {
		if (nargs != 2)
			return false;
		double speed;
		speed = atof(commands[i++].data());
		simulator->setRealtimeFraction(speed);
		return true;
	}
	if (command == string("settracelength")) {
		if (nargs != 2)
			return false;
		maxTraceDraw = atol(commands[i++].data());
		return true;
	}
	//Command parsing 	(may be used as command [particle] [args]
	// 					 or as command [args] when select was used
	while(i<=nargs) {
		if (command == string("remove")) {
			cout << "Unimplemented!" << endl;
			return false;
		}
		if (command == string("vars")) {
			simulator->pauseSimulation();
			if (hasSelected) {
				particles[selected].print(selected);
			}else {
				selected = stoul(commands[i++].data());
				if (selected < 0 || selected >= _nParticles) {
					if (!wasPaused) simulator->resumeSimulation();
					cout << "Invalid id" << endl;
					return false;
				}
				particles[selected].print(selected);
			}
			if (!wasPaused) simulator->resumeSimulation();
			return true;
		}
		if (command == string("edit")) {
			simulator->pauseSimulation();
			if (!hasSelected) {
				selected = stoul(commands[i++].data());
				if (selected < 0 || selected >= _nParticles) {
					if (!wasPaused) simulator->resumeSimulation();
					cout << "Invalid id" << endl;
					return false;
				}
			}
			enbody::Particle newPar = particles[selected];
			double speed,x,y;
			if (nargs>1) { //args given
				while(i<nargs) {
					command = commands[i++];
					if (command == string("weight")) {
						newPar.mass = strtold(commands[i++].data(),0);
					}
					if (command == string("speed")) {

						speed = strtold(commands[i++].data(),0);
						newPar.velocity = newPar.velocity.normalize() * speed;
					}
					if (command == string("direction")) {
						x = strtold(commands[i++].data(),0);
						y = strtold(commands[i++].data(),0);
						newPar.velocity = vec2d(x,y);
					}
					if (command == string("position")) {
						x = strtold(commands[i++].data(),0);
						y = strtold(commands[i++].data(),0);
						newPar.position = vec2d(x,y);
					}
					if (command == string("radius")) {
						newPar.radius = strtold(commands[i++].data(),0);
					}
				}
			}else {
				cout << "Invalid command" << endl;
				return false;
			}
			particles[selected] = newPar;
			if (!wasPaused) simulator->resumeSimulation();
			return true;
		} //end edit
		command = commands[i++];
	}
	return false;
}

bool parsescript(std::string file) {
	std::fstream f;
	try {
		f.open(file,std::fstream::in);
	}catch (std::exception * e) {
		cout << "Failed to open " << file << endl <<
				e->what() << endl;
		return false;
	}

	unsigned int line = 0;
	char buff[256];
	while(!f.eof()) {
		line++;
		f.getline(buff, 256);
		if (!parseCommand(buff))
		{
			printf("Error on line %i of %s", line, file.data());
			f.close();
			return false;
		}
		particles = simulator->particleArrayPointer;
		nParticles = simulator->getNParticles();
	}
	f.close();

	return true;


}

void parsecmdl(int narg, char** args) {
	for(int i = 1; i < narg; i++) {
		string cmd = args[i];
		if (cmd== string("--script")){ // load [filename]
			if (!parsescript(args[++i]))
				exit(2);
		}else
		if (cmd==  string("--sol")){//start on load
			simulator->resumeSimulation();
		}else
		if (cmd==  string("--speed")){ //set realtime fraction (--speed [rtf])
			simulator->setRealtimeFraction(atof(args[++i]));
		}else
		if (cmd==  string("--dt")){ //set dt
			simulator->setDT(strtold(args[++i],0));
		}else
		if (cmd== string("--help")){
			cout << "Galaxy simulator ?v1.0?" << endl <<
					"Commandline arguments:" << endl <<
					"   --script filename" << endl <<
					"       => loads file [filename] as linewise runtime commands" << endl <<
					"   --sol => stars immediately after load" << endl <<
					"   --speed realvalue => sets the speed to [realvalue] times the normal time" << endl <<
					"   --dt realvalue => sets the timestep to [realvalue] in seconds" << endl;
			exit(0);
		}else{
			cout << "Invalid input argument... type --help" << endl;
			exit(2);

		}
	}
}

void cmdthread (bool * run) {
	char buffer[256];
	while (run) {
		cout << ">";
		cin.getline(buffer,256);
		try {
			if (!parseCommand(buffer))
				cout << "invalid command" << endl;
		}catch (std::exception * e) {
			cout << e->what() << endl;
		}
	}
}

int main(int narg, char** args) {
	//init sim
	//TODO: commandline init with variables
	//	spread from origin (mean=0, alpha=?)
	//	mean mass
	//  sd mass
	// nparticles

	//Init default
	simulator = new enbody::NbodySim(8);
	simulator->setDT(100.0);
	simulator->setNormalSize(100, 50);
	simulator->setNormalWeight(5000000000000.0, 1000.0);
	//simulator->setWorkingSector(0,0);
	simulator->initRNG(0);

	//particles = simulator->enableReadBuffer();
	particles = simulator->particleArrayPointer;

	//simulator->addParticles(30, 1000.0);

	//Init sim (start paused)
	simulator->setRealtimeFraction(1);
	simulator->pauseSimulation();
	simulator->startSimulation();

	parsecmdl(narg, args);

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

	//Start command processor
	bool run = true;
	std::thread cmdproc(cmdthread, &run);
	cmdproc.detach();

	//Start
	glutMainLoop();
	run = false;
	cmdproc.~thread();

	delete simulator;

	return 0;
}
