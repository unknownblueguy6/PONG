#pragma once

const unsigned SCREEN_WIDTH = 640;
const unsigned SCREEN_HEIGHT = 480;

enum axes
{
	X,
	Y
};

enum corners{
	TL, //top left corner
	TR, //top right corner
	BL, //bottom left
	BR //bottom right
};

class Particle
{
	public:
		void move();
		void changeVel(bool choice, int v);
		void reverse(bool choice);
		int getCorner(bool choice1, int choice2);

		int vx; //velocity along x-axis
		int vy; //velocity along y-axis
		int x;
		int y;
		unsigned w;
		unsigned h; 
};

void Particle :: move(){
	x += vx;
	y += vy;
}

void Particle :: changeVel(bool choice, int v){
	if (choice == X){
		vx = v;
	}
	else{
		vy = v;
	}
}

void Particle :: reverse(bool choice){
	if(!choice){
		vx *= -1;
	}
	else{
		vy *= -1;
	}
}

int Particle :: getCorner(bool choice1, int choice2){
	if(choice1 == X){
		if(choice2 == TL){
			return x;
		}
		if(choice2 == TR){
			return x+w;
		}
		if(choice2 == BL){
			return x;
		}
		if(choice2 == BR){
			return x+w;
		}
	}
	else{
		if(choice2 == TL){
			return y;
		}
		if(choice2 == TR){
			return y;
		}
		if(choice2 == BL){
			return y+h;
		}
		if(choice2 == BR){
			return y+h;
		}
	}
}



Particle p1, p2, ball;

void newGame(){
	p1 = {0, 10, 10, (SCREEN_HEIGHT/2 - SCREEN_HEIGHT/5), SCREEN_WIDTH/30, SCREEN_HEIGHT/5};
	p2 = {0, 10, SCREEN_WIDTH - 10 - SCREEN_WIDTH/30, SCREEN_HEIGHT/2 - SCREEN_HEIGHT/5, SCREEN_WIDTH/30, SCREEN_HEIGHT/5};
	ball = {5, 10, SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 20, 20};
}

void collision(Particle &a){
		if (a.x <= 0 || a.x >= SCREEN_WIDTH - a.w){
			a.reverse(X);
		}
		if (a.y <= 0 || a.y >= SCREEN_HEIGHT - a.h){
			a.reverse(Y);
		}
}

void collision(Particle &a, Particle b){
	if(a.x > b.x && a.x < b.getCorner(X, BR) && a.y > b.y && a.y < b.getCorner(Y, BR)){
		a.reverse(X);
	}
	if(a.getCorner(X, TR) > b.x && a.getCorner(X, TR) < b.getCorner(X, BR) && a.getCorner(Y, TR) > b.y && a.getCorner(Y, TR) < b.getCorner(Y, BR)){
		a.reverse(X);
	}
	if(a.getCorner(X, BL) > b.x && a.getCorner(X, BL) < b.getCorner(X, BR) && a.getCorner(Y, BL) > b.y && a.getCorner(Y, BL) < b.getCorner(Y, BR)){
		a.reverse(X);
	}
	if(a.getCorner(X, BR) > b.x && a.getCorner(X, BR) < b.getCorner(X, BR) && a.getCorner(Y, BR) > b.y && a.getCorner(Y, BR) < b.getCorner(Y, BR)){
		a.reverse(X);
	}
}