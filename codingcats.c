#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "cats/cat.h"
#include "./emojis/emoji.h"
#include <raylib.h>

#include <time.h>

// *********************************************************
#define NUM_CATS (10)
static char* cat_names[] = {
	"black", "blue", "brown",
	"calico", "cotton_candy_blue", "cotton_candy_pink",
	"creme", "dark", "game_boy",
	"ghost", "gold", "grey",
	"hairless", "indigo", "orange",
	"peach", "pink", "radioactive",
	"red", "seal_point", "teal",
	"white", "white_grey", "yellow"
};
// *********************************************************

//--------- DEBUG MODE ----------//
// #define DEBUG 
//-------------------------------//

#define CAT(name,x,y,v) createCat(name,x,y,v)
#define INIT_INT_ARRAY(arr, sz) for (int i = 0; i < (sz); ++i) (arr)[i] = -1;

//------- MONITOR DIMENSIONS ---------//
const int MONITORW = 1920;
const int MONITORH = 1080;
//------------------------------------//

//------- WINDOW DIMENSIONS AND POSITION ----------//
const int WINW = MONITORW;
const int WINH = 200;
const int WINPOSX = 0;
const int WINPOSY = MONITORH - WINH;
//-------------------------------------------------//

bool containsIdx(int indices[NUM_CATS], int given_idx) {
	for (int i = 0; i < NUM_CATS; ++i) {
		if (indices[i] == given_idx) return true;
	}
	return false;
}

int main(void) {

// ***********************************************************************
#ifdef DEBUG 
	SetTraceLogLevel(LOG_ALL);
#else 
	SetTraceLogLevel(LOG_NONE);
#endif
// ***********************************************************************
	SetConfigFlags(FLAG_WINDOW_TRANSPARENT | FLAG_WINDOW_UNFOCUSED);
	InitWindow(WINW, WINH, "Coding Cats");
// ***********************************************************************
#ifndef DEBUG
	SetWindowState(FLAG_WINDOW_UNDECORATED  | FLAG_WINDOW_MOUSE_PASSTHROUGH);
#endif
// ***********************************************************************
	SetWindowPosition(WINPOSX, WINPOSY);
	SetTargetFPS(60);

	SetRandomSeed(time(NULL));

	Font font = LoadFont("./fonts/main_font.ttf");

	Image window_icon = LoadImage("./appicon.png");
	SetWindowIcon(window_icon);
	SetExitKey(KEY_Q);

	int indices[NUM_CATS];
	INIT_INT_ARRAY(indices, NUM_CATS);

	Cat cats[NUM_CATS];
	for (int i = 0; i < NUM_CATS; ++i) {
		int curr_idx = GetRandomValue(0,23);
		while (containsIdx(indices, curr_idx)) curr_idx = GetRandomValue(0,23); 

		cats[i] = CAT(
			cat_names[curr_idx], 
			GetRandomValue(100,1800)*1.0f, 
			WINH/2.0f, 
			GetRandomValue(35, 100)*1.0f
		);
		indices[i] = curr_idx;
	}

	double rand_spot_time = GetTime();
	float rand_x_spot = GetRandomValue(0,1920)*1.0f;
	while (!WindowShouldClose()) {
		for (int i = 0; i < NUM_CATS; ++i) UpdateCat(&cats[i]);
		
		double curr_time = GetTime();
		if (curr_time - rand_spot_time >= 20.0f) {
			rand_x_spot = GetRandomValue(0,1920)*1.0f;
			rand_spot_time = curr_time;
			for (int i = 0; i < NUM_CATS; ++i) {
				if (fabsf(cats[i].x - rand_x_spot) <= 300.0f && cats[i].emoji.state == EMOJI_IDLE) {
					cats[i].emoji.x = cats[i].x + 40.0f;
					cats[i].emoji.y = cats[i].y;
					cats[i].emoji.state = EMOJI_ACTIVE;
				}
			}
		}
		

		BeginDrawing();
		#ifdef DEBUG
			ClearBackground(RAYWHITE);	
		#else
			ClearBackground(BLANK);	
		#endif

			for (int i = 0; i < NUM_CATS; ++i) {
				PlayAnim(&cats[i]);
				if (cats[i].emoji.state == EMOJI_ACTIVE) DrawEmoji(&cats[i].emoji);
			}
		EndDrawing();
	}

	for (int i = 0; i < NUM_CATS; ++i) UnloadCat(cats[i]);
	UnloadFont(font);
	UnloadImage(window_icon);

	return 0;
}