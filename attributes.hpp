#pragma once

#include<fstream>
#include<string>


int arr[21];

int SCREEN_FPS;
int SCREEN_TICK_PER_FRAME;

int PLAYER_ONE_VEL_X;
int PLAYER_ONE_VEL_Y;
int PLAYER_ONE_WIDTH;
int PLAYER_ONE_HEIGHT;
int PLAYER_ONE_POS_X;
int PLAYER_ONE_POS_Y;

int PLAYER_TWO_VEL_X;
int PLAYER_TWO_VEL_Y;
int PLAYER_TWO_WIDTH;
int PLAYER_TWO_HEIGHT;
int PLAYER_TWO_POS_X;
int PLAYER_TWO_POS_Y;

int BALL_MAX_VEL_Y;
int BALL_MIN_VEL_Y;
int BALL_VEL_X;
int BALL_VEL_Y;
int BALL_WIDTH;
int BALL_HEIGHT; 
int BALL_POS_X;
int BALL_POS_Y;

void readValues(){
	std :: ifstream attributes ("config.txt");
	std :: string line;
	int pos, num = 0;
	if(attributes.is_open()){
		while(getline(attributes, line)){
			if ( line.length() == 0 || line[0] == '#') continue;
			pos = line.find('=');
			arr[num] = std::stoi(line.substr(pos+2));
			++num;
		}
	}
	attributes.close();
	SCREEN_FPS = arr[0];
	SCREEN_TICK_PER_FRAME = 1000 / SCREEN_FPS;

	PLAYER_ONE_VEL_X = arr[1];
	PLAYER_ONE_VEL_Y = arr[2];
	PLAYER_ONE_WIDTH = arr[3];
	PLAYER_ONE_HEIGHT = arr[4];
	PLAYER_ONE_POS_X = arr[5];
	PLAYER_ONE_POS_Y = arr[6];

	PLAYER_TWO_VEL_X = arr[7];
	PLAYER_TWO_VEL_Y = arr[8];
	PLAYER_TWO_WIDTH = arr[9];
	PLAYER_TWO_HEIGHT = arr[10];
	PLAYER_TWO_POS_X = arr[11];
	PLAYER_TWO_POS_Y = arr[12];

	BALL_MAX_VEL_Y = arr[13];
	BALL_MIN_VEL_Y = arr[14];
	BALL_VEL_X = arr[15];
	BALL_VEL_Y = arr[16];
	BALL_WIDTH = arr[17];
	BALL_HEIGHT = arr[18]; 
	BALL_POS_X = arr[19];
	BALL_POS_Y = arr[20];
}