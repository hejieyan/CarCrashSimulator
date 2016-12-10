#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include "mathLibrary.h"

#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/freeglut.h>
#endif

// Requests dimensions of the terrain space
// Ensures user inputs are in the valid range
void getUserInput() {
	do {
		std::cout << "Please enter the desired terrain length [50 - 300]: ";
		std::cin >> terrainLength;
	} while (terrainLength < 50 || terrainLength > 300);

	do {
		std::cout << "Please enter the desired terrain width [50 - 300]: ";
		std::cin >> terrainWidth;		
	}
	while (terrainWidth < 50 || terrainWidth > 300);

	do {
		std::cout << "Please enter the desired maximum terrain height [1 - 20]: ";
		std::cin >> terrainMaxHeight;
	}
	while (terrainMaxHeight < 1 || terrainMaxHeight > 20);
}

void initCamera() {
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	cameraPosition[0] = terrainLength*2.5;
	cameraPosition[1] = terrainWidth;
	cameraPosition[2] = terrainWidth/2;

	gluLookAt(cameraPosition[0], cameraPosition[1], cameraPosition[2],  
			cameraTarget[0], cameraTarget[1], cameraTarget[2], 
			cameraUpVector[0], cameraUpVector[1], cameraUpVector[2]);
	glRotatef(cameraRotationY,1,0,0);
	glRotatef(cameraRotationX,0,1,0);
	glTranslatef(-terrainLength/2, 0, -terrainLength/2);

}

void display() {
	glClearColor(0, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(cameraPosition[0], cameraPosition[1], cameraPosition[2],  
				cameraTarget[0], cameraTarget[1], cameraTarget[2], 
				cameraUpVector[0], cameraUpVector[1], cameraUpVector[2]);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_POINTS);
		glColor3f(1,1,1);
		glVertex3f(0, 0, 0);
		glColor3f(1,0,0);
		glVertex3f(terrainLength, 0, 0);
		glColor3f(0,1,0);
		glVertex3f(0, 0, terrainWidth);
		glColor3f(0,0,1);
		glVertex3f(terrainLength, 0, terrainWidth);
	glEnd();

	glutSwapBuffers();
}

void keyboard(unsigned char key, int xIn, int yIn)
{
	int mod = glutGetModifiers();
	switch (key)
	{
		// ESC: to exit program
		case 27:	//27 is the esc key
			exit(0);
			break;

		// q: to toggle between Smooth and Flat shading
		case 'q':
			switch (shadingMode) {
				case SHADING_FLAT:
					shadingMode = SHADING_SMOOTH;
					break;
				case SHADING_SMOOTH:
					shadingMode = SHADING_FLAT;
					break;
			}
			break;

		// w: Toggles between polygon, wireframe, or both drawings
		case 'w':
			// TODO: Make increment function for enums
			switch (viewingMode) {
				case VIEW_POLYGON:
					viewingMode = VIEW_WIREFRAME;
					break;
				case VIEW_WIREFRAME:
					viewingMode = VIEW_POLYFRAME;
					break;
				case VIEW_POLYFRAME:
					viewingMode = VIEW_POLYGON;
					break;
			}
			break;

		// e: Toggle light On/Off
		case 'e':
			switch (lightOn) {
				case false:
					lightOn = true;
					enableLighting();
					break;
				case true:
					lightOn = false;
					disableLighting();
					break;
			} 
			break;

		// r: Initialize new terrain heightMap with normals and redraw the scene
		case 'r':
			// Delete allocated pointers and re-initialize
			for(int i = 0; i < terrainWidth; ++i) {
			    delete [] heightMap[i];
			}
			delete [] heightMap;
			initHeightMap();
			computeNormals();
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			break;

		// t: Toggle QUAD/TRIANGLE Shapes
		case 't':
			switch (shapeMode) {
				case SHAPE_QUAD_STRIPS:
					shapeMode = SHAPE_TRIANGLE_STRIPS;
					break;
				case SHAPE_TRIANGLE_STRIPS:
					shapeMode = SHAPE_QUAD_STRIPS;
					break;
			}
			break;

		//camera control
		case 's':
			cameraPosition[0] += cameraSpeedFactor;
			break;
		case 'z':
			cameraPosition[1] -= cameraSpeedFactor;
			break;
		case 'x':
			cameraPosition[0] -= cameraSpeedFactor;
			break;
		case 'c':
			cameraPosition[1] += cameraSpeedFactor;
			break;
		case 'a':
			cameraPosition[2] -= cameraSpeedFactor;
			break;
		case 'd':
			cameraPosition[2] += cameraSpeedFactor;
			break;


		//move camera target
		case 'i':
			cameraTarget[0] += cameraSpeedFactor;
			break;
		case 'j':
			cameraTarget[1] -= cameraSpeedFactor;
			break;
		case 'k':
			cameraTarget[0] -= cameraSpeedFactor;
			break;
		case 'l':
			cameraTarget[1] += cameraSpeedFactor;
			break;
		case 'u':
			cameraTarget[2] -= cameraSpeedFactor;
			break;
		case 'o':
			cameraTarget[2] += cameraSpeedFactor;
			break;
	}
	glutPostRedisplay();
}

void special(int key, int x, int y){
  	switch(key){

    	 case GLUT_KEY_LEFT:
    	 	cameraRotationX -= 10;
    	 	glutPostRedisplay();
        	break;

     	 case GLUT_KEY_RIGHT:
     	 	cameraRotationX += 10;
        	break;

      	case GLUT_KEY_UP:
      		cameraRotationY += 10;
        	break;

      	case GLUT_KEY_DOWN:
      		cameraRotationY -= 10;
          	break;

        // case GLUT_KEY_PAGE_UP:
        // 	if (editRadius>=5){
        // 		editRadius += 5;
        // 	}
        // 	break;

        // case GLUT_KEY_PAGE_DOWN:
        // 	if (editRadius <=mapSize){
        // 		editRadius -= 5;
        // 	}
        // 	break;

	}
	// TODO: Swap Buffer instead of redisplay for efficiency
	glutPostRedisplay();
}

void initFunc() {
	initCamera(); // Function to setup camera
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special);
	// glutTimerFunc(0,FPS,0);
}

int (int argc, char** argv) {
	// showInstructions();
	// getUserInput();
	initCamera();
	srand(time(NULL)); //Allows for a random seed based on the time; different every time
	// initHeightMap();
	// computeNormals();

	glutInit(&argc, argv);	
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(50, 50);
	glutCreateWindow("Car Crash Simulator");

	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, 1, 1, 2000);

	initFunc();
	glutMainLoop();

	return 0;
}