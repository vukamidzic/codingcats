#include "cat.h"
#include "../timer.h"

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define IDLE_STATE (0)
#define LEFT_WALK_STATE (1)
#define RIGHT_WALK_STATE (2)
#define SIT_STATE (3)

#define FRAME(x,y,w,h) (Rectangle) {x,y,w,h}
#define FRAMES(...) (Rectangle[]) {__VA_ARGS__}
#define ANIMATION(atlas,fps,frames,len) (createAnimation(atlas,fps,frames,len))

Cat createCat(char name[], float x, float y, float velocity) {
    Cat cat = {
        .x = x,
        .y = y,
        .velocity = velocity,
		.dir = GetRandomValue(0,1),
		.timers = {
			createTimer(GetRandomValue(15, 30)*1.0), 
			createTimer(GetRandomValue(5, 10)*1.0)
		},
		.emoji = (Emoji) {
			.opacity = 255.0f,
			.state = EMOJI_IDLE,
			.tex = LoadTexture("./emojis/emojis.png")
		}
    };

	char cat_sprite_path[PATHMAX] = "./cats/spritesheets/";
	strcat(cat_sprite_path, name);
	strcat(cat_sprite_path, "_0.png");

	cat.tex = LoadTexture(cat_sprite_path);

	cat.anims[IDLE_STATE] = ANIMATION(
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

	cat.anims[LEFT_WALK_STATE] = ANIMATION(
		cat.tex,
		3,
		FRAMES(
			FRAME(384,160, FRAMESIZE,FRAMESIZE),
			FRAME(416,160, FRAMESIZE,FRAMESIZE),
			FRAME(448,160, FRAMESIZE,FRAMESIZE),
			FRAME(480,160, FRAMESIZE,FRAMESIZE)
		),
		4
	);

	cat.anims[RIGHT_WALK_STATE] = ANIMATION(
		cat.tex,
		3,
		FRAMES(
			FRAME(384,416, FRAMESIZE,FRAMESIZE),
			FRAME(416,416, FRAMESIZE,FRAMESIZE),
			FRAME(448,416, FRAMESIZE,FRAMESIZE),
			FRAME(480,416, FRAMESIZE,FRAMESIZE)
		),
		4
	);

	cat.anims[SIT_STATE] = ANIMATION(
		cat.tex,
		3,
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

    return cat;
}

static inline void PlaySitAnim(Cat cat) {
	DrawAnimation(cat.anims[SIT_STATE], Rect(cat.x,cat.y, 4*FRAMESIZE,4*FRAMESIZE));
}

static inline void PlayWalkAnim(Cat cat) {
	if (cat.dir == LEFT) DrawAnimation(cat.anims[LEFT_WALK_STATE], Rect(cat.x,cat.y, 4*FRAMESIZE,4*FRAMESIZE));
	else DrawAnimation(cat.anims[RIGHT_WALK_STATE], Rect(cat.x,cat.y, 4*FRAMESIZE,4*FRAMESIZE));
}

static inline void PlayIdleAnim(Cat cat) {
	DrawAnimation(cat.anims[IDLE_STATE], Rect(cat.x,cat.y, 4*FRAMESIZE,4*FRAMESIZE));
}

static inline bool sitAnimFinished(Cat* cat) {
	int frame_index = (int)(GetTime() * cat->anims[SIT_STATE].frames_per_sec) % cat->anims[SIT_STATE].rects_len;
	return frame_index == cat->anims[SIT].rects_len-1;
}

void PlayAnim(Cat* cat) {
	switch (cat->state) {
		case WALK : {
			PlayWalkAnim(*cat);
		} break;
		case SIT : {
			PlaySitAnim(*cat);
			if (sitAnimFinished(cat)) {
				cat->timers[1].start_time = GetTime();
				cat->state = IDLE;
			} 
		} break;
		case IDLE : {
			PlayIdleAnim(*cat);
			if (timeout(&cat->timers[1])) {
				cat->timers[0].start_time = GetTime();
				cat->timers[0].life_time = GetRandomValue(15,30)*1.0f;
				cat->state = WALK;
				cat->dir = GetRandomValue(0,1);
				cat->velocity = GetRandomValue(40, 100)*1.0f;
			}
		} break;
	}
}

void UpdateCat(Cat* cat) {
	if (timeout(&cat->timers[0])) cat->state = SIT;

	if (cat->emoji.state == EMOJI_ACTIVE) {
		cat->emoji.y -= 2.0f;
		cat->emoji.opacity -= 5.0f;
		if (cat->emoji.opacity <= 0.0f) {
			cat->emoji.state = EMOJI_IDLE;
			cat->emoji.opacity = 255.0f;	
		}
	}

	if (cat->state == WALK) {
		float v = cat->velocity * GetFrameTime();
		switch (cat->dir) {
			case LEFT : {
				cat->x -= v;
				if (cat->x < 0) { cat->x = 0; cat->dir = RIGHT; }
			} break;
			case RIGHT : {
				cat->x += v;
				if (cat->x > 1920 - 4*FRAMESIZE) { cat->x = 1920 - 4*FRAMESIZE; cat->dir = LEFT; }
			} break;
		}
	}
}

void UnloadCat(Cat cat) {
	for (int i = 0; i < 4; ++i) UnloadAnimation(cat.anims[i]);
	UnloadTexture(cat.tex);   
	UnloadTexture(cat.emoji.tex);
}