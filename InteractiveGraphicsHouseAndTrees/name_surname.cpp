
//#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartupKey\"" )
#include <GL/freeglut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include "camera.h"
#include "Primitives.h"
#include <iostream>
#include <time.h>
#include <fstream>
#include <string>

using namespace std;

#define WINDOW_WIDTH  1200
#define WINDOW_HEIGHT 600

// Perspective Camera Parameters
#define FOVY 70.0f
#define NEAR_PLANE 0.1f
#define FAR_PLANE  100.0f

#define  TIMER_PERIOD  16 // Period for the timer.
#define  TIMER_ON     1     // 0:disable timer, 1:enable timer

#define D2R 0.0174532

/* Global Variables for Template File */
bool upKey = false, downKey = false, rightKey = false, leftKey = false;
bool wKey = false, sKey = false, aKey = false, dKey = false, spaceKey = false;
int  winWidth, winHeight; // current Window width and height
string entityType = "Castle"; //Kiz Kulesi

Camera cam(0, 3, 10, 0, 0, 0.2); // at the origin, looking at -z, speed: 0.1

int mx = 200, my = 200;

const char* infoText;

typedef struct {
	double x, y, z;
} position_t;

typedef struct {
	unsigned char r, g, b;
} color_t;

typedef struct {
	position_t pos;
	double rotate;
	string type;
} entity;

#define NUM 50
entity entities[NUM];

#define BARKS_NUM 25
int bark_loc[BARKS_NUM];
int bark_xAxis[BARKS_NUM];
int bark_yAxis[BARKS_NUM];
int bark_zAxis[BARKS_NUM];

#define LEAVES_NUM 200
double leaves_t_xAxis[LEAVES_NUM];
double leaves_t_yAxis[LEAVES_NUM];
double leaves_t_zAxis[LEAVES_NUM];

int globalAngle = 0;
int flagAngle = 0;
int flagDirection = 1;

int entityCount = 0;

void saveScene() {
	ofstream ofs("environment.txt");
	ofs << entityCount << "\n";
	for (int i = 0; i < entityCount; i++) {
		ofs << entities[i].pos.x << " " << entities[i].pos.y << " " << entities[i].pos.z <<  
			" " << entities[i].rotate << " " << entities[i].type << "\n";
	}
	ofs.close();
}

void loadScene() {
	fstream envFile("environment.txt", std::ios_base::in);
	envFile >> entityCount;
	
	for (int i = 0; i < entityCount; i++) {
		envFile >> entities[i].pos.x;
		envFile >> entities[i].pos.y;
		envFile >> entities[i].pos.z;
		envFile >> entities[i].rotate;
		envFile >> entities[i].type;
	}
}

void initEntity() {
	for (int i = 0; i < 60; i++) {
		cam.Forward();
	}
	
	double xPos = cam.getX();
	double yPos = cam.getY() + 10;
	double zPos = cam.getZ();
	double angle = cam.getLeftRight();
	
	for (int i = 0; i < 60; i++) {
		cam.Backward();
	}

	entities[entityCount].pos.x = xPos;
	entities[entityCount].pos.y = yPos;
	entities[entityCount].pos.z = zPos;
	entities[entityCount].rotate = angle;
	entities[entityCount].type = entityType;

	entityCount++;
}


void Tree() {
	//trunk
	double trunkBase = 0.07; double trunkHeight = 2.0;
	glColor3f(0.4, 0.3, 0.1);
	glPushMatrix();
	glRotatef(-90, 1, 0, 0);
	Cylinder(trunkBase, trunkHeight, 20, 1);
	glPopMatrix();

	//grass base
	double baseWidth = 1.5; double baseHeight = 0.05; double baseLength = 1.5;
	glPushMatrix();
	glColor3f(0, 0.8, 0);
	glScalef(baseWidth, baseHeight, baseLength);
	glTranslatef(0, -trunkHeight/2.0, 0);
	Cube(1);
	glPopMatrix();

	//barks
	double coneBase = 0.01; double coneHeight = 0.7;
	
	for (int i = 0; i < BARKS_NUM; i++) {
		glPushMatrix();
		glColor3f(0.4, 0.3, 0.1);
		glTranslatef(0, trunkHeight, 0);
		glRotatef(bark_loc[i], bark_xAxis[i], bark_yAxis[i], bark_zAxis[i]);
		glTranslatef(0, coneHeight, 0);
		glRotatef(90, 1, 0, 0);
		Cone(coneBase, coneHeight, 4, 4);
		glPopMatrix();
	}

	//pink leaves
	for (int i = 0; i < LEAVES_NUM; i++) {
		glPushMatrix();
		glColor3f(1.0, 0.3, 0.6);
		glTranslatef(leaves_t_xAxis[i], leaves_t_yAxis[i], leaves_t_zAxis[i]);
		glTranslatef(0, trunkHeight, 0);
		Sphere(0.04, 4, 4);
		glPopMatrix();
	}
}

void Castle() {
	//base
	double baseWidth = 6; double baseHeight = 0.05; double baseLength = 6;
	glPushMatrix();
	glColor3f(0.7, 0.5, 0.3);
	glScalef(baseWidth, baseHeight, baseLength);
	Cube(1);
	glPopMatrix();

	//shorter building
	double sbWidth = 4; double sbHeight = 1.0; double sbLength = 1.5;
	glPushMatrix();
	glColor3f(0.7, 0.6, 0.4);
	glTranslatef(0, baseHeight+(sbHeight/2.0), baseLength/4.0);
	glScalef(sbWidth, sbHeight, sbLength);
	Cube(1);
	glPopMatrix();

	//taller building
	double tbWidth = 2.5; double tbHeight = 1.8; double tbLength = 1.5;
	glPushMatrix();
	glColor3f(0.7, 0.6, 0.4);
	glTranslatef(baseWidth/6.0, baseHeight+(tbHeight/2.0), 0);
	glScalef(tbWidth, tbHeight, tbLength);
	Cube(1);
	glPopMatrix();

	//tower
	double towerWidth = 1.2; double towerHeight = 4.5; double towerLength = 1.2;
	glPushMatrix();
	glColor3f(0.95, 0.95, 0.95);
	glTranslatef(-baseWidth / 6.0, baseHeight + (towerHeight / 2.0), 0);
	glScalef(towerWidth, towerHeight, towerLength);
	Cube(1);
	glPopMatrix();

	//tower top
	double coneBase = 1.4; double coneHeight = 1.0;
	glPushMatrix();
	glColor3f(0.8, 0.8, 0.8);
	glTranslatef(-baseWidth / 6.0, baseHeight + towerHeight, 0);
	glRotatef(-90, 1, 0, 0);
	Cone(coneBase, coneHeight, 4, 4);
	glPopMatrix();

	//flag rod
	double rodBase = 0.05; double rodHeight = 2.4;
	glColor3f(0.4, 0.4, 0.4);
	glPushMatrix();
	glTranslatef(-baseWidth / 6.0, baseHeight + towerHeight + coneHeight, 0);
	glRotatef(-90, 1, 0, 0);
	Cylinder(rodBase, rodHeight, 20, 10);
	glPopMatrix();

	//rod top
	double rodTopSize = 0.15;
	glPushMatrix();
	glColor3f(0.4, 0.4, 0.4);
	glTranslatef(-baseWidth / 6.0, baseHeight + towerHeight + coneHeight + rodHeight, 0);
	Sphere(rodTopSize, 10, 10);
	glPopMatrix();


	(flagDirection == 1) ? flagAngle++ : flagAngle--;
	if (flagAngle%180 == 0)
		flagDirection *= -1;
	//flag 
	double flagWidth = 0.8; double flagHeight = 0.5; double flagLength = 0.01;
	glColor3f(1.0, 0, 0);
	glPushMatrix();
	glTranslatef(-baseWidth/6.0+(flagWidth/2.0), baseHeight + towerHeight + coneHeight + rodHeight/2.0, 0);
	glTranslatef(-flagWidth/2.0, 0, 0);
	glRotatef(flagAngle, 0, 1, 0);
	glTranslatef(flagWidth/2.0, 0, 0);
	glScalef(flagWidth, flagHeight, flagLength);
	Cube(1);
	glPopMatrix();


	//rooftops
	glPushMatrix();
	double sRoofWidth = (sbWidth/2.0); double sRoofHeight = 0.1; double sRoofLength = 1.1*sbLength;
	glColor3f(0.4, 0, 0);
	glPushMatrix();
	glTranslatef(-sbWidth/4.0, sbHeight+(sRoofHeight*1.3), baseLength / 4.0);
	glRotatef(8, 0, 0, 1);
	glScalef(sRoofWidth, sRoofHeight, sRoofLength);
	Cube(1);
	glPopMatrix();
	glPushMatrix();
	glColor3f(0.4, 0, 0);
	glPushMatrix();
	glTranslatef(sbWidth / 4.0, sbHeight + (sRoofHeight * 1.3), baseLength / 4.0);
	glRotatef(-8, 0, 0, 1);
	glScalef(sRoofWidth, sRoofHeight, sRoofLength);
	Cube(1);
	glPopMatrix();

	glPushMatrix();
	double tRoofWidth = (tbWidth / 2.0); double tRoofHeight = 0.1; double tRoofLength = 1.1 * tbLength;
	glColor3f(0.4, 0, 0);
	glPushMatrix();
	glTranslatef(baseWidth / 6.0 - tbWidth/4.0, tbHeight + (tRoofHeight * 1.3), 0);
	glRotatef(8, 0, 0, 1);
	glScalef(tRoofWidth, tRoofHeight, tRoofLength);
	Cube(1);
	glPopMatrix();
	glPushMatrix();
	glColor3f(0.4, 0, 0);
	glPushMatrix();
	glTranslatef(baseWidth / 6.0 + tbWidth / 4.0, tbHeight + (tRoofHeight * 1.3), 0);
	glRotatef(-8, 0, 0, 1);
	glScalef(tRoofWidth, tRoofHeight, tRoofLength);
	Cube(1);
	glPopMatrix();
}

void Scene() {
	for (int i = 0; i < entityCount; i++) {
		glPushMatrix();
		glTranslatef(entities[i].pos.x, entities[i].pos.y, entities[i].pos.z);
		glRotatef(entities[i].rotate, 0, 1, 0);
		entities[i].type == "Castle" ? Castle() : Tree();
		glPopMatrix(); 
	}
}

// display text with variables.
void vprint(int x, int y, void *font, const char *string, ...)
{
	va_list ap;
	va_start(ap, string);
	char str[1024];
	vsprintf_s(str, string, ap);
	va_end(ap);

	int len, i;
	glRasterPos2f(x, y);
	len = (int)strlen(str);
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(font, str[i]);
	}
}

void display()
{
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Perspective Camera
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(FOVY, winWidth * 1.0f / winHeight, NEAR_PLANE, FAR_PLANE);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	cam.LookAt();

	
	floor();
	Scene();
	
	// OVERLAY - Orthographic Camera
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-winWidth / 2, winWidth / 2, -winHeight / 2, winHeight / 2, 100, -100);
	glMatrixMode(GL_MODELVIEW);

	// Info
	glLoadIdentity();
	glPushMatrix();
	glTranslatef(-winWidth / 2.15 , -winHeight / 2.15, 0);
	glScalef(10.0, 10.0, 10.0);
	glRotatef(globalAngle++, 0, 1, 0);
	entityType == "Castle" ? Castle() : Tree();
	string entityName = (entityType == "Castle") ? "Kiz Kulesi" : "Cherry Blossom";
	glPopMatrix();
	glColor3f(0, 0, 0);
	string info = "Spacebar: Add " + entityName + " Tab: Change  Move: w/a/s/d  F1: Save F2: Load (PLEASE USE MINIMUM NO. OF TREES)";
	infoText = info.c_str();
	vprint(-winWidth / 2 + 90, -winHeight / 2 + 30, GLUT_BITMAP_9_BY_15, infoText);

	glutSwapBuffers();
}

//
// key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
//
void onKeydown(unsigned char key, int x, int y)
{
	// exit when ESC is pressed.
	if (key == 27)
		exit(0);

	switch (key) {
	case 'w':
	case 'W': wKey = true; break;
	case 's':
	case 'S': sKey = true; break;
	case 'a':
	case 'A': aKey = true; break;
	case 'd':
	case 'D': dKey = true; break;
	case ' ': spaceKey = true; 
		if(entityCount!=NUM-1)
			initEntity(); 
	break;
	case '\t':
		entityType = (entityType == "Castle") ? "Tree" : "Castle";
	break;
	}

	// to refresh the window it calls display() function
	// glutPostRedisplay();
}

void onKeyup(unsigned char key, int x, int y)
{
	// exit when ESC is pressed.
	if (key == 27)
		exit(0);

	switch (key) {
	case 'w':
	case 'W': wKey = false; break;
	case 's':
	case 'S': sKey = false; break;
	case 'a':
	case 'A': aKey = false; break;
	case 'd':
	case 'D': dKey = false; break;
	case ' ': spaceKey = false; break;
	}

	// to refresh the window it calls display() function
	// glutPostRedisplay();
}
//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_upKey, GLUT_KEY_downKey, GLUT_KEY_rightKey, GLUT_KEY_rightKey
//
void onSpecialKeydown(int key, int x, int y)
{
	// Write your codes here.
	switch (key) {
	case GLUT_KEY_UP: upKey = true; break;
	case GLUT_KEY_DOWN: downKey = true; break;
	case GLUT_KEY_LEFT: leftKey = true; break;
	case GLUT_KEY_RIGHT: rightKey = true; break;
	case GLUT_KEY_F1: saveScene(); break;
	case GLUT_KEY_F2: loadScene(); break;
	}
	// to refresh the window it calls display() function
	// glutPostRedisplay();
}


//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_upKey, GLUT_KEY_downKey, GLUT_KEY_rightKey, GLUT_KEY_rightKey
//
void onSpecialKeyup(int key, int x, int y)
{
	// Write your codes here.
	switch (key) {
	case GLUT_KEY_UP: upKey = false; break;
	case GLUT_KEY_DOWN: downKey = false; break;
	case GLUT_KEY_LEFT: leftKey = false; break;
	case GLUT_KEY_RIGHT: rightKey = false; break;

	}
	// to refresh the window it calls display() function
	// glutPostRedisplay();
}


//
// When a click occurs in the window,
// It provides which button
// buttons : GLUT_leftKey_BUTTON , GLUT_rightKey_BUTTON
// states  : GLUT_upKey , GLUT_downKey
// x, y is the coordinate of the point that mouse clicked.
//
void onClick(int button, int stat, int x, int y)
{
	// Write your codes here.



	// to refresh the window it calls display() function
	// glutPostRedisplay();
}

//
// This function is called when the window size changes.
// w : is the new width of the window in pixels.
// h : is the new height of the window in pixels.
//
void onResize(int w, int h)
{
	winWidth = w;
	winHeight = h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    gluPerspective(FOVY, winWidth * 1.0f / winHeight, NEAR_PLANE, FAR_PLANE);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClearColor(0, 0, 0, 0);
	display(); // refresh window.
}

void onMovedownKey(int x, int y) {
	// Write your codes here.



	// to refresh the window it calls display() function	
	// glutPostRedisplay();
}

void onMove(int x, int y) {
	// Write your codes here.
	mx = x;
	my = y;


	// to refresh the window it calls display() function
	// glutPostRedisplay();
}

#if TIMER_ON == 1
void onTimer(int v) {

	glutTimerFunc(TIMER_PERIOD, onTimer, 0);
	// Write your codes here.


	if (wKey) cam.Forward();
	if (sKey) cam.Backward();
	if (aKey) cam.Left();
	if (dKey) cam.Right();
	
	//allows multiple entities to be created at the same time and fall simultaneously
	for (int i = 0; i < entityCount; i++) {
		if (entities[i].pos.y > 0.2)
			entities[i].pos.y-=0.2;
	}

	cam.TurnRightLeft((200 - mx) / 4.0);
	cam.TurnUpDown((200 - my) / 4.0);
	glutWarpPointer(200, 200);
	
	// to refresh the window it calls display() function
	glutPostRedisplay(); // display()

}
#endif

bool does_envFile_exist(const char* fileName)
{
	std::ifstream envFile(fileName);
	return envFile.good();
}

void Init() {
	
	glutWarpPointer(200, 200);
	glutSetCursor(GLUT_CURSOR_NONE);


	glEnable(GL_DEPTH_TEST);
	// Smoothing shapes
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	srand(unsigned(time(NULL)));

	for (int i = 0; i < BARKS_NUM; i++) {
		int randomNum = 10 + (rand() % (170 - 10 + 1));
		bark_loc[i] = randomNum;
		bark_xAxis[i] = -1 + (rand() % (3));
		bark_yAxis[i] = -1 + (rand() % (3));
		bark_zAxis[i] = -1 + (rand() % (3));
	}

	for (int j = 0; j < LEAVES_NUM; j++) {
		leaves_t_xAxis[j] = -0.7 + ((rand() % 15000) / 10000.0);
		leaves_t_yAxis[j] = -0.7 + ((rand() % 15000) / 10000.0);
		leaves_t_zAxis[j] = -0.7 + ((rand() % 15000) / 10000.0);
	}

	if (does_envFile_exist("environment.txt"))
		loadScene();
}


void main(int argc, char *argv[])
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("Primitive Solid Objects");

	Init();
	
	glutDisplayFunc(display);
	glutReshapeFunc(onResize);
	//
	// keyboard registration
	//
	glutKeyboardFunc(onKeydown);
	glutSpecialFunc(onSpecialKeydown);

	glutKeyboardUpFunc(onKeyup);
	glutSpecialUpFunc(onSpecialKeyup);

	//
	// mouse registration
	//
	glutMouseFunc(onClick);
	glutMotionFunc(onMovedownKey);
	glutPassiveMotionFunc(onMove);

#if  TIMER_ON == 1
	// timer event
	glutTimerFunc(TIMER_PERIOD, onTimer, 0);
#endif
	glutMainLoop();
}