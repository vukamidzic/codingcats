#include "cat.h"
#include "../timer.h"

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define FRAME(x,y,w,h) (Rectangle) {x,y,w,h}
#define FRAMES(...) (Rectangle[]) {__VA_ARGS__}

Cat createCat(char name[], float x, float y, float velocity) {
    Cat cat = {
        .x = x,
        .y = y,
        .velocity = velocity,
		.dir = GetRandomValue(0,1),
		.timer = createTimer(GetRandomValue(15, 30)*1.0),
		.timer1 = createTimer(GetRandomValue(5, 10)*1.0)
    };

	strcpy(cat.name, "./cats/spritesheets/");
	strcat(cat.name, name);
	strcat(cat.name, "_0.png");

	cat.tex = LoadTexture(cat.name);

    cat.sit = createAnimation(
	    cat.tex,
		4,
		FRAMES(
			FRAME(0,32, FRAMESIZE,FRAMESIZE),
			FRAME(32,32, FRAMESIZE,FRAMESIZE),
			FRAME(64,32, FRAMESIZE,FRAMESIZE),
			FRAME(96,32, FRAMESIZE,FRAMESIZE),
			FRAME(0,64, FRAMESIZE,FRAMESIZE),
			FRAME(32,64, FRAMESIZE,FRAMESIZE),
			FRAME(64,64, FRAMESIZE,FRAMESIZE)
		),
		7
	);

	cat.idle = createAnimation(
		cat.tex,
		3,
		FRAMES(
			FRAME(128,32, FRAMESIZE,FRAMESIZE),
			FRAME(160,32, FRAMESIZE,FRAMESIZE),
			FRAME(192,32, FRAMESIZE,FRAMESIZE),
			FRAME(224,32, FRAMESIZE,FRAMESIZE),
			FRAME(128,64, FRAMESIZE,FRAMESIZE)
		),
		5
	);

	cat.left_walk = createAnimation(
	    cat.tex,
		4,
		FRAMES(
			FRAME(384,160, FRAMESIZE,FRAMESIZE),
			FRAME(416,160, FRAMESIZE,FRAMESIZE),
			FRAME(448,160, FRAMESIZE,FRAMESIZE),
			FRAME(480,160, FRAMESIZE,FRAMESIZE)
		),
		4
	);

	cat.right_walk = createAnimation(
	    cat.tex,
		4,
		FRAMES(
			FRAME(384,416, FRAMESIZE,FRAMESIZE),
			FRAME(416,416, FRAMESIZE,FRAMESIZE),
			FRAME(448,416, FRAMESIZE,FRAMESIZE),
			FRAME(480,416, FRAMESIZE,FRAMESIZE)
		),
		4
	);

    return cat;
}

static inline void PlaySitAnim(Cat cat) {
	DrawAnimation(cat.sit, Rect(cat.x,cat.y, 4*FRAMESIZE,4*FRAMESIZE));
}

static inline void PlayWalkAnim(Cat cat) {
	if (cat.dir == LEFT) DrawAnimation(cat.left_walk, Rect(cat.x,cat.y, 4*FRAMESIZE,4*FRAMESIZE));
	else DrawAnimation(cat.right_walk, Rect(cat.x,cat.y, 4*FRAMESIZE,4*FRAMESIZE));
}

static inline void PlayIdleAnim(Cat cat) {
	DrawAnimation(cat.idle, Rect(cat.x,cat.y, 4*FRAMESIZE,4*FRAMESIZE));
}

static inline bool sitAnimFinished(Cat* cat) {
	int frame_index = (int)(GetTime() * cat->sit.frames_per_sec) % cat->sit.rects_len;
	return frame_index == cat->sit.rects_len-1;
}

void PlayAnim(Cat* cat) {
	switch (cat->state) {
		case WALK : {
			PlayWalkAnim(*cat);
		} break;
		case SIT : {
			PlaySitAnim(*cat);
			if (sitAnimFinished(cat)) {
				cat->timer1.start_time = GetTime();
				cat->state = IDLE;
			} 
		} break;
		case IDLE : {
			PlayIdleAnim(*cat);
			if (timeout(&cat->timer1)) {
				cat->timer.start_time = GetTime();
				cat->timer.life_time = GetRandomValue(15, 30)*1.0;
				cat->state = WALK;
				cat->dir = GetRandomValue(0,1);
				cat->velocity = GetRandomValue(40, 100)*1.0f;
			}
		} break;
	}
}

void UpdateCat(Cat* cat) {
	if (timeout(&cat->timer)) cat->state = SIT;

	if (cat->state == WALK) {
		switch (cat->dir) {
			case LEFT : {
				cat->x -= cat->velocity * GetFrameTime();
				if (cat->x < 0) { cat->x = 0; cat->dir = RIGHT; }
				break;
			}
			case RIGHT : {
				cat->x += cat->velocity * GetFrameTime();
				if (cat->x > 1920 - 4*FRAMESIZE) { cat->x = 1920 - 4*FRAMESIZE; cat->dir = LEFT; }
				break;
			}
		}
	}
}

void UnloadCat(Cat cat) {
    UnloadAnimation(cat.left_walk);
	UnloadAnimation(cat.right_walk);
    UnloadAnimation(cat.sit); 
	UnloadTexture(cat.tex);   
}