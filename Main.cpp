#include <iostream>
#include <cstdio>
#include <cstdlib>
using namespace std;

#include <time.h>
#include <gl/glut.h>
#include "Game1.cpp"
//#include <irrKlang.h>
#include "SoundManager.h"

// link to the irrKlang lib
//#pragma comment(lib, "irrKlang.lib")

// use Audio library
using namespace irrklang;


ISoundEngine* soundEngine;
int gameTime = 0;

const int BLOCKSIZE = 30;
const int VPWIDTH = COLS * BLOCKSIZE; // 10 * 30 = 300
const int VPHEIGHT = ROWS * BLOCKSIZE; // 20 * 30 = 600

void BitmapText(char *str, int wcx, int wcy)
{
	glRasterPos2i(wcx, wcy);
	for (int i=0; str[i] != '\0'; i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);
	}
}
//
//// utility function - random - return a random number in the interval [0,1]
//double random()
//{ 
//	int r = rand();
//	return double(r) / RAND_MAX;
//}

// Structure to maintain the state of a game
Game1 game;

// timer - Callback for timers
void timer(int id)
{

	if (game.killed) {	// mode for when the user has been killed
		if (id < 10) {
			game.paused = true;
			glutTimerFunc(200, timer, id+1);
			game.clearGrid();
			glutPostRedisplay();
		}
		else {
			game.killed = false;
			game.restart();
			glutTimerFunc(game.timer, timer, 0);
		}
		glutPostRedisplay();
	} else if (!game.paused) {	// normal operation is when game
		game.update();		// is not paused
		if (game.killed) {
			glutTimerFunc(10, timer, 1);
		} else {
			gameTime += game.timer;
			glutPostRedisplay();
			glutTimerFunc(game.timer, timer, gameTime);
		}
	}
}

// keyboard - Callback for handling user's keyboard actions.
void keyboard(unsigned char key, int x, int y)
{
	if (key == 'p') {
		game.paused = !game.paused;
		if (game.paused)
			glutTimerFunc(0, timer, gameTime); // Pause the timer
		else
			glutTimerFunc(game.timer, timer, gameTime); // Resume the timer
	}
	if (key == 'p') {
		game.paused = game.paused;
		if (!game.paused)
			glutTimerFunc(game.timer, timer, 0);
	} else if (!game.paused && !game.killed && key == ' ') {
		game.update();
	}
	glutPostRedisplay();
}



void menu(int item) {
	switch(item) {
		case 1:
			//pause game
			game.paused = !game.paused;
			if (!game.paused)
				glutTimerFunc(game.timer, timer, 0);
			break;
		case 2:
			gameTime = 0;
			game.restart();
			soundEngine->play2D("bell.wav", false);
			break;
		case 3:
			//exit game
			exit(0);
			break;
	}
}

// keyboard - Callback for handling user's arrow keys.
void special(int key, int x, int y)
{
	if (!game.paused && !game.killed) {
		if (key == GLUT_KEY_LEFT) {
			game.move(-1);
		} else if (key == GLUT_KEY_RIGHT) {
			game.move(1);
		}else if (key == GLUT_KEY_UP) {
			game.rotateShape(1);
		}else if (key == GLUT_KEY_DOWN) {
			game.rotateShape(-1);
		}
	}
	glutPostRedisplay();
}


// display - Callback for displaying the grid and statistics.
void display(void)
{
	glClearColor(0.2f, 0.2f, 0.2f, 0.72);
	glClear(GL_COLOR_BUFFER_BIT);

// Draw grid
	glViewport(0, 0, VPWIDTH, VPHEIGHT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, COLS, ROWS, 0);

	//if(!game.paused)
		for (int r=0; r<ROWS; r++) {
			for (int c=0; c<COLS; c++) {
				Square &square = game.grid[r][c];
				if (square.filled){
					glColor3f(square.red, square.green, square.blue);
					glRectd(c+.1, r+.1, c+.9, r+.9);
				}
				else {
					glColor3f(0.22, 0.22, 0.22);
					glRectd(c+.1, r+.1, c+.9, r+.9);
				}
				
			}
		}

// Write messages on right hand side of screen
	glViewport(VPWIDTH, 0, VPWIDTH+100, VPHEIGHT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, 100, 0, 500);
	const int N = 100;
	char msg[N+1];
	
	char score[N+1];
	sprintf_s(score, N, "Score: %d", game.lines); 
    glColor3f(1,140/255,0);
    glRasterPos2f(10, VPHEIGHT*1/10);
    for (int i = 0; i < N; i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, score[i]);
    }

	glColor3f(0,1,1);
	sprintf_s(msg, N, "1BG20CS101");
	BitmapText(msg, 10, 180);
	
	glColor3f(1,1,0);
	sprintf_s(msg, N, "Shrilakshmi R");
	BitmapText(msg, 10, 200);

	glColor3f(0,1,1);
	sprintf_s(msg, N, "1BG20CS096");
	BitmapText(msg, 10, 230);

	glColor3f(1,1,0);
	sprintf_s(msg, N, "Sharanna Das");
	BitmapText(msg, 10, 250);

	glColor3f(0,1,1);
	sprintf_s(msg, N, "Created by:");
	BitmapText(msg, 10, 280);

	glColor3f(0,1,1);
	sprintf_s(msg, N, "A Computer Graphics Mini-project");
	BitmapText(msg, 10, 310);

	string title = "TETRIS";
    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(10, VPHEIGHT*3/4);
    int len = title.length();
    for (int i = 0; i < len; i++) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, title[i]);
    }



	//middle bar
	glBegin(GL_LINES);
		glColor3f(1.0, 1.0, 1.0);
		glVertex2d(1,0);
		glVertex2d(1, glutGet(GLUT_WINDOW_HEIGHT));
		glEnd();

		char timerText[N + 1];
		sprintf_s(timerText, N, "Time: %d seconds", gameTime / 1000); // Convert milliseconds to seconds
		glColor3f(1, 0, 0);
		BitmapText(timerText, 10, 100);
	glutSwapBuffers();
}

void main(int argc, char *argv[])
{
	//srand(time(0));
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);	
	soundEngine = createIrrKlangDevice();
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(VPWIDTH + 400, VPHEIGHT);

	glutCreateWindow("Tetris");

	glutDisplayFunc(display);
	glutSpecialFunc(special);
	glutKeyboardFunc(keyboard);
	glutTimerFunc(game.timer, timer, gameTime);
	glutCreateMenu(menu);
	glutAddMenuEntry("Pause/Resume Game",1);
	glutAddMenuEntry("Reset Game", 2);
	glutAddMenuEntry("Quit Game",3);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	soundEngine->play2D("breakout.mp3", true);
	glutMainLoop();
	soundEngine->drop();
}