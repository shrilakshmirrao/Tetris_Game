//Values for how many pieces, how many rotations for each piece, and how much memory is needed to store each piece
const int numPieces		= 7;
const int numRotations	= 4;
const int numSpaces		= 8;

//Stores the all 7 pieces and each of their rotations in a 3-dimensional grid

const int gamePiece[numPieces][numRotations][8] =
{
	{
		{0, 0, 1, 0, 0, 1, 1, 1}, //Square and its rotations 
		{0, 0, 1, 0, 0, 1, 1, 1},
		{0, 0, 1, 0, 0, 1, 1, 1},
		{0, 0, 1, 0, 0, 1, 1, 1},
	},
	{
		{0, 0, 0, 1, 0, 2, 0, 3}, //Line piece straight up
		{0, 0, 1, 0, 2, 0, 3, 0}, //sideways
		{0, 0, 0, 1, 0, 2, 0, 3},
		{0, 0, 1, 0, 2, 0, 3, 0},
	},
	{
		{0, 0, 0, 1, 1, 1, 0, 2}, //t-shape
		{1, 0, 0, 1, 1, 1, 2, 1},
		{0, 1, 1, 0, 1, 1, 1, 2}, 
		{0, 0, 1, 0, 2, 0, 1, 1}
	},
	{	{0, 0, 1, 0, 0, 1, 0, 2},// X
		{0, 0, 0, 1, 1, 1, 2, 1},// XXX
		{1, 0, 1, 1, 0, 2, 1, 2},
		{0, 0, 1, 0, 2, 0, 2, 1}
	},
	{	{0, 0, 1, 0, 1, 1, 1, 2},// XXX
		{0, 0, 1, 0, 2, 0, 0, 1},// X
		{0, 0, 0, 1, 0, 2, 1, 2},
		{2, 0, 0, 1, 1, 1, 2, 1}
	},
	{	{0, 0, 0, 1, 1, 1, 1, 2}, //backwards z-shape
		{1, 0, 2, 0, 0, 1, 1, 1},
		{0, 0, 0, 1, 1, 1, 1, 2},
		{1, 0, 2, 0, 0, 1, 1, 1}
	},
	{	{1, 0, 0, 1, 1, 1, 0, 2}, //z-shape
		{0, 0, 1, 0, 1, 1, 2, 1},
		{1, 0, 0, 1, 1, 1, 0, 2},
		{0, 0, 1, 0, 1, 1, 2, 1}
	}
};


//The piece class
class Piece1 {
public:

	//x and y position of the piece
	int x;
	int y;

	//color values
	float redVal, blueVal, greenVal;

	//type of piece (0-6) and rotation (0-3)
	int type;
	int rotation;

	//values for the piece rotations
	//const int* rotations();

	Piece1() {
		Piece1(0);
	}

	/*
	Creates a new piece and sets the piece type as well as color and sets rotation to first rotation
	*/
	Piece1(int typ) {
		type = typ;
		switch (type) {
			case 0:		color(1.0, 0.0, 1.0); break;
			case 1:		color(0.5, 0.5, 0.5); break;
			case 2:		color(0.0, 1.0, 1.0); break;
			case 3: 	color(0.0, 1.0, 0.0); break;
			case 4:		color(1.0, 0.0, 0.0); break;
			case 5:		color(1.0, 1.0, 0.0); break;
			case 6:		color(0.0, 0.0, 0.8); break; 
		}
		rotation = 0;
	}

	/*
	Returns the array holding the specific piece's information based on the rotation
	*/
	const int* rotations() {
		return gamePiece[type][rotation];
	}

	//Sets the pieces color to be the ones designated in the constructor
	void color(float r, float g, float b){
		redVal = r;
		greenVal = g;
		blueVal = b;
	}

	//Increments (or decrements) the game pieces rotation state
	void rotatePiece(int dir){
		if(dir > 0) {
			if (rotation == 3)
				rotation = 0;
			else
				rotation += dir;
		}
		else if (rotation < 0) {
			if (rotation == 0)
				rotation = 3;
			else
				rotation -= dir;
		}
	}

};

