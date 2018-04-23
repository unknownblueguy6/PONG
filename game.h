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
		int getCorner(bool choice, int choice);

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
	if (!choice){
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
	if(a.x > )
}