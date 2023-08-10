#include "Piece1.cpp"
#include <cstdlib>
#include <cstdio>
#include <gl/glut.h>
#include <iostream>
//#include <irrKlang.h>
#include "SoundManager.h"





using namespace std;

//Rows and columns
const int ROWS = 20;
const int COLS = 10;

//The actual object that the game grid is constructed of. Each has a filled atribute, an active for designating which is the current piece,
//a toBeDeleted boolean that sets whether it is to be cleared, and color designations.
struct Square {
	bool filled;
	bool isActive;
	bool toBeDeleted;
	float red, green, blue;
};

//the game class where everything takes place
class Game1 {
public:
	
	//the game pieces that are put onto the board
	Piece1 active;
	Piece1 next;
	Piece1 activeCopy;

	//The actual game board
	Square grid[ROWS][COLS];

	//Game data
	int lines;
	int shapesProc;
	bool deleteLines;
	bool killed;	// has the game ended
	bool paused;	// is the game paused
	int timer;		// timer

//game.cpp

	/*
	Gets called every time the timer on the game ticks without the game being over or paused
	*/
	void update() {
		//checks for collision with the movement set for down
		if(collision(0)) { //If there is a collision at its current position (without rotation or movement of piece)
			if(active.y <= 2) { //check if it ended the game
				killed = true;
			}
			else { //if the game is still running
				updateActiveAfterCollision();  //lock the piece in place and no longer be active
				checkLine(); //Check the lines for a full row
				if (deleteLines) { //If there are lines to delete
					clearLine();//delete the lines
					soundEngine->play2D("bell.wav", false);
				}
				nextPiece(); //create the next piece
				updatePiece(); //update it on the game grid
			}
		}//if collision
		else { //else if no collision move the active piece down the game board
			updateActive();
			active.y++;
			updatePiece();
		}
	}

	//Starts a new game and initializes everything to empty
	void restart()
	{
		clearGrid(); //clears grid so everything is empty
		lines = 0;  //player has cleared 0 line
		killed = false;
		paused = false;
		active = Piece1(rand() % 7);  //Create a new random piece
		
		active.x = COLS/2; //Set its x and y location to the starting points, in the top middle position
		active.y = 0;
		updatePiece(); //Have the gameboard acknowledge where the piece starts out

		//Create the next piece
		next = Piece1(rand() % 7);
		next.x = COLS/2;
		next.y = 0;

		//The user has seen one shape so far
		shapesProc = 1;
		deleteLines = false;

	}


	/*
	Updates the game grid to show what the active piece is.  
	*/
	void updateActive() {
		//Finds the pieces data based on type and orientation
		const int* trans = active.rotations();
		for(int i = 0; i < 8; i += 2){
			Square &square = grid[active.y + trans[i + 1]][active.x + trans[i]];
			//set the square as not being active or as being filled
			square.filled = false;
			square.isActive = false;
		}	
	}

	/*
	Create the next game piece
	*/
	void nextPiece() {
		active = next;
		next = Piece1(rand() % 7);
		next.x = COLS/2;
		next.y = 0;
		//increment the number of shapes seen by the user
		shapesProc++;
	}

	/*
	move the active game piece to the left or right
	*/
	void move(int dir)
	{
		if(collision(dir)) //If there is a collision with the direction don't do anything
			return;
		updateActive();
		active.x += dir;
		updatePiece();
	}



	/*
	Sets every game grid square to not be filled
	*/
	void clearGrid()
	{
		for (int r=0; r<ROWS; r++) {
			for (int c=0; c<COLS; c++) {
				grid[r][c].filled = false;
				grid[r][c].isActive = false;
			}
		}
	}

	/*
	Does all the updating for the pieces on board.
	*/
	void updatePiece() {
		//pointer to the array where all the transformations are stored
		const int* trans = active.rotations();
		for(int i = 0; i < 8; i += 2){
			//Find the square in the game grid that is designated by the game piece
			Square &square = grid[active.y + trans[i + 1]][active.x + trans[i]];
			//Set that piece to the square and set the color of the square to the color of the piece
			square.filled = true;
			square.isActive = true;
			square.red = active.redVal;
			square.green = active.greenVal;
			square.blue = active.blueVal;
		}	
	}

	/*
	Constructor
	*/
	Game1()
	{
		restart();
		timer = 500;
	}

	/*
	Rotates the current shape by first checking against a copy if it is possible and then actually reflect the changes 
	*/
	void rotateShape(int rot) {
		//Create the copy of the active piece to test to rotation on
		activeCopy = Piece1(rand() % 7);
		activeCopy.x = active.x;
		activeCopy.y = active.y;
		activeCopy.rotation = active.rotation;
		activeCopy.type = active.type;
		//Rotate the piece
		activeCopy.rotatePiece(rot);
		
		//If the piece is able to be rotated, update the active piece
		if(canRotate(activeCopy)){
			updateActive();
			active.rotatePiece(rot);
			updatePiece();
		}

	}

	/*
	Check whether the game piece can be rotated without the new shape running into a collision
	*/
	bool canRotate(Piece1 activeP) {
		if(collision(-3)) {  //-3 because I just set the collision method to check for rotations if a value < -2 is being passed
			return false;
		}
		else
			return true;

	}

	/*
	Check for collisions in the game from either moving down, left, right, or rotating the piece
	*/
	bool collision(int dir) {
		//temporary values for the modified x and y
		int x, y;
		bool ret = false;
		if(dir < -2) {       //If the user is trying to rotate a piece
			const int* trans = activeCopy.rotations();
			for(int i = 0; i < 8; i += 2){
				x = activeCopy.x + trans[i];
				y = activeCopy.y + trans[i + 1];

				if(x >= COLS || y >= ROWS || x < 0 || (grid[y][x].filled && !grid[y][x].isActive))
					return true;
			}
		}
		else {
			const int* trans = active.rotations();
			for(int i = 0; i < 8; i += 2){
				x = active.x + trans[i];
				y = active.y + trans[i + 1];
				if(dir == 0)
					y += 1;
				else
					x += dir;
				if(x >= COLS ||  y >= ROWS || x < 0 || (grid[y][x].filled && !grid[y][x].isActive))
					return true;
			}
		}
		return ret;
	}

	/*
	After the active piece has settled, update where it settled to no longer be the active piece
	*/
	void updateActiveAfterCollision() {
		const int* trans = active.rotations();
		for(int i = 0; i < 8; i += 2){
			Square &square = grid[active.y + trans[i + 1]][active.x + trans[i]];
			square.isActive = false;
		}
	}

	/*
	Check the row and if an entire row is filled set them to be deleted
	*/
	void checkLine() {
		int fullRows = 0;
		for (int r=0; r<ROWS; r++) {
			bool fullRow = false;
				for (int c=0; c<COLS; c++) {
					Square &square = grid[r][c];
					if (square.filled){
						fullRow = true; //Keep it at true
					}
					else { //If there is a space, you know this line isn't full
						fullRow = false;
						break;
					}
				}
			if(fullRow) { //If the entire row was checked and they were all full
				for ( int c =0; c < COLS; c++){
					grid[r][c].toBeDeleted = true;
				}
				deleteLines = true;
				lines++;
			}
		}
	}

	/*
	Clear the lines from the game board and drop each line above to the line down
	*/
	void clearLine(){
		for (int r = ROWS-1; r > 0; r--){  //checks for each row
			int linesDeleted = 0;
			if (grid[r][0].toBeDeleted){
				for (int r2 = r; r2>0; r2--){  //moves each row down one
					for (int c = 0; c < COLS; c++){ //check the columns
						grid[r2][c].filled=grid[r2-1][c].filled;
						grid[r2][c].isActive=grid[r2-1][c].isActive;
						grid[r2][c].toBeDeleted=grid[r2-1][c].toBeDeleted;
						grid[r2][c].red=grid[r2-1][c].red;
						grid[r2][c].green=grid[r2-1][c].green;
						grid[r2][c].blue=grid[r2-1][c].blue;
					}
				}
				r++;
			}
		}
		deleteLines = false;

	}


};

