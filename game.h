#pragma once

const unsigned SCREEN_WIDTH = 640;
const unsigned SCREEN_HEIGHT = 480;
enum axes
{
	X,
	Y
};

class Particle
{
	public:
		void move();
		void changeVel(bool choice, int v);
		void reverse(bool choice);

		int vx; //velocity along x-axis
		int vy; //velocity along y-axis
		unsigned x;
		unsigned y;
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
	ball = {10, 20, SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 20, 20};
}

void collision(Particle &a, Particle b = {0, 0, 0, 0, 0 ,0}){
	if (!b.w){
		if (a.x <= 0 || a.x >= SCREEN_WIDTH - a.w){
			a.reverse(X);
		}
		if (a.y <= 0 || a.y >= SCREEN_HEIGHT - a.h){
			a.reverse(Y);
		}
	}
	else{

	}
}