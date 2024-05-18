#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "cats/cat.h"
#include <raylib.h>

#include <time.h>

/*  
Wave functions:
	* WAVE_FN(c1,c2) => f(x) = sin(c1*x)/c2; c1,c2 -> consts
	* CUSTOM_WAVE_FN_1(c1,c2) => f(x) = -abs(sin(c1*x)/c2); c1,c2 -> consts
	* CUSTOM_WAVE_FN_2(c1,c2) => f(x) = -abs(sin(c1*x) + cos(x)/c2); c1,c2 -> consts
*/

#define WAVE_FN(c1,c2) (sin(GetTime()*(c1))/(c2)) 
#define CUSTOM_WAVE_FN_1(c1,c2) (-fabs(sin(GetTime()*(c1)))/(c2)) 
#define CUSTOM_WAVE_FN_2(c1,c2) (-fabs(sin(GetTime()*(c1)) + cos(GetTime()))/(c2))
#define EMOJI_PADDING (75.0f)

// *********************************************************
#define NUM_CATS (8)
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
static int cmd_res;
// *********************************************************

//--------- DEBUG MODE ----------//
// #define DEBUG 
//-------------------------------//

#define CAT(name,x,y,v) createCat(name,x,y,v)

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
	InitWindow(WINW, WINH, "CODING CATS");
// ***********************************************************************
#ifndef DEBUG
	SetWindowState(FLAG_WINDOW_UNDECORATED | FLAG_WINDOW_MOUSE_PASSTHROUGH);
#endif
// ***********************************************************************
	SetWindowPosition(WINPOSX, WINPOSY);
	SetTargetFPS(60);

	SetRandomSeed(time(NULL));

	Font font = LoadFont("./fonts/main_font.ttf");

	Image window_icon = LoadImage("./appicon.png");
	SetWindowIcon(window_icon);

	int indices[NUM_CATS];
	for (int i = 0; i < NUM_CATS; ++i) indices[i] = -1;

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

	Texture2D emojis = LoadTexture("./emojis/emojis.png");

	while (!WindowShouldClose()) {
		cmd_res = atoi(LoadFileText("./info.config"));
		for (int i = 0; i < NUM_CATS; ++i) UpdateCat(&cats[i]);

		BeginDrawing();
		#ifdef DEBUG
			ClearBackground(RAYWHITE);	
		#else
			ClearBackground(BLANK);	
		#endif
			for (int i = 0; i < NUM_CATS; ++i) PlayAnim(&cats[i]);
			for (int i = 0; i < NUM_CATS; ++i) {
				int xpos = cats[i].x, ypos = cats[i].y;
				if (cmd_res == 0) {
					DrawTexturePro(
						emojis,
						(Rectangle) {.x = 0, .y = 0, .width = 16, .height = 16},
						(Rectangle) {
							.x = (cats[i].state == WALK)? ((cats[i].dir == LEFT)? xpos + 40.0f 
								: xpos + 52.0f) : xpos + 50.0f, 
							.y = ypos * WAVE_FN(8.0f,9.0f) + EMOJI_PADDING,
							.width = 32, 
							.height = 32
						},
						Vec2(0.0f, 0.0f),
						0.0f,
						RAYWHITE
					);
				}
				else {
					DrawTexturePro(
						emojis,
						(Rectangle) {.x = 0, .y = 112, .width = 16, .height = 16},
						(Rectangle) {
							.x = (cats[i].state == WALK)? ((cats[i].dir == LEFT)? xpos + 40.0f 
								: xpos + 52.0f) : xpos + 50.0f,
							.y = ypos * WAVE_FN(8.0f,9.0f) + EMOJI_PADDING, 
							.width = 32, 
							.height = 32
						},
						Vec2(0.0f, 0.0f),
						0.0f,
						RAYWHITE
					);
				}
			}
		EndDrawing();
	}

	for (int i = 0; i < NUM_CATS; ++i) UnloadCat(cats[i]);
	UnloadFont(font);
	UnloadImage(window_icon);

	return 0;
}