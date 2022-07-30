//Include necessary header files
#include "glut.h"//Header file for OpenGL version glut
#include <cstdlib>
#include <cmath>
#include <iostream>
using namespace std;

//Link with necessary libraries.
//This need to be written in at least one .cpp file in the project
#pragma comment (lib, "glut32.lib")

//Function prototypes
void render();
void onKeyboard(unsigned char, int, int);
void SpecialKeys(int key, int x, int y);
void onFrame();
void ResizeWindow(int w, int h);

#define

// 0: sun, 1: earth, 2: moon, 3: planet3, 4:planet4, 5: planet5
float Radii[6] = { 1.0, 0.4, 0.1, 0.5, 0.7, 0.2 };
float OrbitingSpeeds[6] = { 0.0, 10.0, 100.0, 20.0, 15.0, 25.0 };
float SpinningSpeeds[6] = { 13.0, 10.0, 100.0, 9.0, 15.0, 25.0 };
float rev = 0.0; // variable for changing the orbiting speed
float spin = 0.0; // variable for changing the spinning speed (around its own axis)

int g_Win;	//Stores window identifier (handle) returned by glutCreateWindow
//We need this handle to terminate the window

float g_X = 0.0f, g_Y = 0.0f, g_Z = 7.0f;

int main(int argc, char **argv){
	//==========================================
	//Step 1: Initialize GLUT & GLEW
	//==========================================
	glutInit(&argc, argv);

	//GLUT_RGBA: Red, Green, Blue, Alpha color representation.
	//GLUT_DOUBLE: Double buffering 
	//GLUT_DEPTH: Create a depth buffer
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

	//Create Window
	glutInitWindowSize(900, 600);
	glutInitWindowPosition(100, 100);
	g_Win = glutCreateWindow("Solar System");

	glShadeModel(GL_FLAT); //?
	glClearDepth(1.0);
	//Enable depth test
	glEnable(GL_DEPTH_TEST);

	//Set the color to be used to clear display buffer.
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	//Set projection transformation
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, 1.0f, 0.1f, 500.0f);
	glMatrixMode(GL_MODELVIEW);

	//Use better (but slower) texture coordinate and color interpolation
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	//==========================================
	//Step 2: Register callback functions
	//==========================================
	glutDisplayFunc(render);
	glutKeyboardFunc(onKeyboard);
	glutSpecialFunc(SpecialKeys);

	// Set up the callback function for resizing windows
	glutReshapeFunc(ResizeWindow);

	//==========================================
	//Step 4: Enter main loop
	//==========================================
	glutMainLoop();

	return 0;
}

//Callback function definitions
void render(void){

	// Clear the redering window
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Update the animation state
	rev += 0.1;
	spin += 0.1;

	// Clear the current matrix (Modelview)
	glLoadIdentity();

	gluLookAt(g_X, g_Y, g_Z, g_X, g_Y, 0, 0, 1, 0.0);

	// Back off to be able to view from the origin.
	glTranslatef(0.0, 0.0, -7.0);

	// Rotate the plane of the elliptic
	// (rotate the model's plane about the x axis by fifteen degrees)
	glRotatef(15.0, 1.0, 0.0, 0.0);

	/******************* Sun ********************/
	// Draw the sun	-- as a yellow, wireframe sphere
	glColor3f(1.0, 1.0, 0.0);
	glutWireSphere(Radii[0], 50, 50);

	// ==>
	glPushMatrix();


	
	/******************* Earth ********************/
	// First position it around the sun
	glRotatef(OrbitingSpeeds[1] * rev, 0.0, 1.0, 0.0);
	glTranslatef(2.0, 0.0, 0.0);

	/// ==>
	glPushMatrix();						// Save matrix state
	
	// Second, rotate the earth on its axis.
	glRotatef(SpinningSpeeds[1] * spin, 0.0, 1.0, 0.0);
	
	// Third, draw the earth as a wireframe sphere.
	glColor3f(0.2, 0.2, 1.0);
	glutWireSphere(Radii[1], 30, 10);
	/// <==
	glPopMatrix();						// Restore matrix state

	/******************* Moon ********************/
	glRotatef(OrbitingSpeeds[2] * rev, 0.0, 1.0, 0.0);
	glTranslatef(0.7, 0.0, 0.0);

	glRotatef(SpinningSpeeds[2] * spin, 0.0, 1.0, 0.0);
	glColor3f(1.0, 1.0, 1.0);
	glutWireSphere(Radii[2], 5, 5);

	// <==
	glPopMatrix();

	/******************* Planet 3********************/
	glPushMatrix();
	glRotatef(OrbitingSpeeds[3] * rev, 0.0, 1.0, 0.0);

	glTranslatef(4, 0.0, 0.0);
	glRotatef(SpinningSpeeds[3] * spin, 0.0, 1.0, 0.0);

	glColor3f(1.0, 0.0, 0.0);
	glutWireSphere(Radii[3], 15, 15);
	glPopMatrix();

	/******************* Planet 4********************/
	glPushMatrix();
	glRotatef(OrbitingSpeeds[4] * rev, 0.0, 1.0, 0.0);
	glTranslatef(6, 0.0, 0.0);

	glRotatef(SpinningSpeeds[4] * spin, 0.0, 1.0, 0.0);
	glColor3f(1.0, 1.0, 1.0);
	glutWireSphere(Radii[4], 15, 15);
	glPopMatrix();

	/******************* Planet 5********************/
	glPushMatrix();
	glRotatef(OrbitingSpeeds[5] * rev, 0.0, 1.0, 0.0);
	glTranslatef(7, 0.0, 0.0);

	glRotatef(SpinningSpeeds[5] * spin, 0.0, 1.0, 0.0);
	glColor3f(1.0, 0.0, 1.0);
	glutWireSphere(Radii[5], 15, 15);
	glPopMatrix();
	
	// Flush the pipeline, and swap the buffers	//Output color buffer to screen
	glFlush();
	glutSwapBuffers();
	glutPostRedisplay();		// Request a re-draw for animation purposes
}

void onKeyboard(unsigned char key, int, int)
{
	switch (key)
	{
	case 'a':
		g_X += 0.1f;
		break;

	case 'd':
		g_X -= 0.1f;
		break;

	case 's':
		g_Y += 0.1f;
		break;

	case 'w':
		g_Y -= 0.1f;
		break;
	}

	glutPostRedisplay();
}

void SpecialKeys(int key, int x, int y){
	switch (key){

	case GLUT_KEY_UP:
		g_Z -= 0.1f;
		break;

	case GLUT_KEY_DOWN:
		g_Z += 0.1f;
		break;
	}
}

static void ResizeWindow(int w, int h)
{
	float aspectRatio;
	h = (h == 0) ? 1 : h;
	w = (w == 0) ? 1 : w;
	glViewport(0, 0, w, h);	// View port uses whole window
	aspectRatio = (float)w / (float)h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, aspectRatio, 1.0, 30.0);

	// Select the Modelview matrix
	glMatrixMode(GL_MODELVIEW);
}
