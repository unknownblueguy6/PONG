#pragma once

#include<fstream>
#include<string>


int arr[23];

int SCREEN_FPS;
int SCREEN_TICK_PER_FRAME;

int PLAYER_ONE_MIN_VEL_Y;
int PLAYER_ONE_MAX_VEL_Y;
int PLAYER_ONE_ACC_Y;
int PLAYER_ONE_WIDTH;
int PLAYER_ONE_HEIGHT;
int PLAYER_ONE_POS_X;
int PLAYER_ONE_POS_Y;

int PLAYER_TWO_MIN_VEL_Y;
int PLAYER_TWO_MAX_VEL_Y;
int PLAYER_TWO_ACC_Y;
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

	PLAYER_ONE_MIN_VEL_Y = arr[1];
	PLAYER_ONE_MAX_VEL_Y = arr[2];
	PLAYER_ONE_ACC_Y = arr[3];
	PLAYER_ONE_WIDTH = arr[4];
	PLAYER_ONE_HEIGHT = arr[5];
	PLAYER_ONE_POS_X = arr[6];
	PLAYER_ONE_POS_Y = arr[7];

	PLAYER_TWO_MIN_VEL_Y = arr[8];
	PLAYER_TWO_MAX_VEL_Y = arr[9];
	PLAYER_TWO_ACC_Y = arr[10];
	PLAYER_TWO_WIDTH = arr[11];
	PLAYER_TWO_HEIGHT = arr[12];
	PLAYER_TWO_POS_X = arr[13];
	PLAYER_TWO_POS_Y = arr[14];

	BALL_MAX_VEL_Y = arr[15];
	BALL_MIN_VEL_Y = arr[16];
	BALL_VEL_X = arr[17];
	BALL_VEL_Y = arr[18];
	BALL_WIDTH = arr[19];
	BALL_HEIGHT = arr[20]; 
	BALL_POS_X = arr[21];
	BALL_POS_Y = arr[22];
}