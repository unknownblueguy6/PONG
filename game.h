#pragma once


const unsigned SCREEN_WIDTH = 640;
const unsigned SCREEN_HEIGHT = 480;

//Change base attributes here.
const int PLAYER_ONE_VEL_X = 0;
const int PLAYER_ONE_VEL_Y = 10;
const int PLAYER_ONE_POS_X = 10;
const int PLAYER_ONE_POS_Y = SCREEN_HEIGHT/2 - SCREEN_HEIGHT/5;
const unsigned PLAYER_ONE_WIDTH = SCREEN_WIDTH/30;
const unsigned PLAYER_ONE_HEIGHT = SCREEN_HEIGHT/5;

const int PLAYER_TWO_VEL_X = 0;
const int PLAYER_TWO_VEL_Y = 10;
const int PLAYER_TWO_POS_X = SCREEN_WIDTH - 10 - SCREEN_WIDTH/30;
const int PLAYER_TWO_POS_Y = SCREEN_HEIGHT/2 - SCREEN_HEIGHT/5;
const unsigned PLAYER_TWO_WIDTH = SCREEN_WIDTH/30;
const unsigned PLAYER_TWO_HEIGHT = SCREEN_HEIGHT/5;

const int BALL_VEL_X = 5;
const int BALL_VEL_Y = 10;
const unsigned BALL_WIDTH = 20;
const unsigned BALL_HEIGHT = 20; 
const int BALL_POS_X = SCREEN_WIDTH/2 - BALL_WIDTH/2;
const int BALL_POS_Y = SCREEN_HEIGHT/2 - BALL_HEIGHT; 


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


Particle PLAYERONE, PLAYERTWO, BALL;

void newGame(){
	PLAYERONE = {PLAYER_ONE_VEL_X, PLAYER_ONE_VEL_Y, PLAYER_ONE_POS_X, PLAYER_ONE_POS_Y, PLAYER_ONE_WIDTH, PLAYER_ONE_HEIGHT};
	PLAYERTWO = {PLAYER_TWO_VEL_X, PLAYER_TWO_VEL_Y, PLAYER_TWO_POS_X, PLAYER_TWO_POS_Y, PLAYER_TWO_WIDTH, PLAYER_TWO_HEIGHT};
	BALL = {BALL_VEL_X, BALL_VEL_Y, BALL_POS_X, BALL_POS_Y, BALL_WIDTH, BALL_HEIGHT};
}

void collision(Particle &a){
		if (a.y <= 0 || a.y >= SCREEN_HEIGHT - a.h){
			a.reverse(Y);
		}
}

int getCollisionPoint(bool choice, int x1, int y1, int vx, int vy, int corner){
	float m = (float)vy/vx;
	if(choice == X){
		return x1 + (corner-y1)/m;
	}
	else{
		return y1 + m*(corner - x1);
	}
}

void collision(Particle &ball, Particle player){
	
	if(ball.getCorner(X, TL) > player.getCorner(X, TL) && ball.getCorner(X, TL) < player.getCorner(X, BR) && ball.getCorner(Y, TL) > player.getCorner(Y, TL) && ball.getCorner(Y, TL) < player.getCorner(Y, BR)){
		if(ball.y - ball.vy < player.getCorner(Y, BR)){
			ball.y = getCollisionPoint(Y, ball.getCorner(X, TL), ball.getCorner(Y, TL), ball.vx, ball.vy, player.getCorner(X, BR));
			ball.x = player.getCorner(X, BR);
		}
		else if(ball.y - ball.vy > player.getCorner(Y, BR)){
			ball.x = getCollisionPoint(X, ball.getCorner(X, TL), ball.getCorner(Y, TL), ball.vx, ball.vy, player.getCorner(Y, BR));
			ball.y = player.getCorner(Y, BR);
			ball.changeVel(Y, -(ball.vy - player.vy));
		}
		else{
			ball.x = player.getCorner(X, BR);
			ball.y = player.getCorner(Y, BR);
			//add special here
		}
		ball.reverse(X);
	}
	if(ball.getCorner(X, TR) > player.getCorner(X, TL) && ball.getCorner(X, TR) < player.getCorner(X, BR) && ball.getCorner(Y, TR) > player.getCorner(Y, TL) && ball.getCorner(Y, TR) < player.getCorner(Y, BR)){
		if(ball.y - ball.vy < player.getCorner(Y, BL)){
			ball.y = getCollisionPoint(Y, ball.getCorner(X, TR), ball.getCorner(Y, TR), ball.vx, ball.vy, player.getCorner(X, BL));
			ball.x = player.getCorner(X, BL) - ball.w;
		}
		else if(ball.y - ball.vy > player.getCorner(Y, BL)){
			ball.x = getCollisionPoint(X, ball.getCorner(X, TR), ball.getCorner(Y, TR), ball.vx, ball.vy, player.getCorner(Y, BL)) - ball.w;
			ball.y = player.getCorner(Y, BL);
			ball.changeVel(Y, -(ball.vy - player.vy));
		}
		else{
			ball.x = player.getCorner(X, BL)-ball.w;
			ball.y = player.getCorner(Y, BL);
			//add special here
		}
		ball.reverse(X);
	}
	if(ball.getCorner(X, BL) > player.getCorner(X, TL) && ball.getCorner(X, BL) < player.getCorner(X, BR) && ball.getCorner(Y, BL) > player.getCorner(Y, TL) && ball.getCorner(Y, BL) < player.getCorner(Y, BR)){
		if(ball.y - ball.vy > player.getCorner(Y, TR)){
			ball.y = getCollisionPoint(Y, ball.getCorner(X, TR), ball.getCorner(Y, TR), ball.vx, ball.vy, player.getCorner(X, TR)) - ball.h;
			ball.x = player.getCorner(X, TR);
		}
		else if(ball.y - ball.vy < player.getCorner(Y, TR)){
			ball.x = getCollisionPoint(X, ball.getCorner(X, TR), ball.getCorner(Y, TR), ball.vx, ball.vy, player.getCorner(Y, TR));
			ball.y = player.getCorner(Y, TR) - ball.h;
			ball.changeVel(Y, -(ball.vy - player.vy));
		}
		else{
			ball.x = player.getCorner(X, TR);
			ball.y = player.getCorner(Y, TR) - ball.h;
			//add special here
		}
		ball.reverse(X);
	}
	if(ball.getCorner(X, BR) > player.getCorner(X, TL) && ball.getCorner(X, BR) < player.getCorner(X, BR) && ball.getCorner(Y, BR) > player.getCorner(Y, TL) && ball.getCorner(Y, BR) < player.getCorner(Y, BR)){
		if(ball.y - ball.vy > player.getCorner(Y, TL)){
			ball.y = getCollisionPoint(Y, ball.getCorner(X, TR), ball.getCorner(Y, TR), ball.vx, ball.vy, player.getCorner(X, TL)) - ball.h;
			ball.x = player.getCorner(X, TL) - ball.w;
		}
		else if(ball.y - ball.vy < player.getCorner(Y, TL)){
			ball.x = getCollisionPoint(X, ball.getCorner(X, TR), ball.getCorner(Y, TR), ball.vx, ball.vy, player.getCorner(Y, TL)) - ball.w;
			ball.y = player.getCorner(Y, TL) - ball.h;
			ball.changeVel(Y, -(ball.vy - player.vy));
		}
		else{
			ball.x = player.getCorner(X, TL) - ball.w;
			ball.y = player.getCorner(Y, TL) - ball.h;
			//add special here
		}
		ball.reverse(X);
	}
}

void reset(Particle &ball){
	if (ball.x + ball.w <= 0 || ball.x >= SCREEN_WIDTH){
		ball.x = BALL_POS_X;
		ball.y = BALL_POS_Y;
		ball.vx = BALL_VEL_X;
		ball.vy = BALL_VEL_Y;
	}
}