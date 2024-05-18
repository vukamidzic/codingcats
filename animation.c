#include "animation.h"
#include <stdlib.h>

SpriteAnim createAnimation(Texture2D atlas, int frames_per_sec, Rectangle rects[], int length) {
	SpriteAnim animation = {
		.atlas = atlas,
		.frames_per_sec = frames_per_sec,
		.rects_len = length
	};

	Rectangle* mem = (Rectangle*)malloc(length*sizeof(Rectangle));
	if (mem == NULL) {
		TraceLog(LOG_FATAL, "Not enough memory for animation...");
		return animation;
	}

	animation.rects = mem;
	for (int i = 0; i < length; ++i) {
		animation.rects[i] = rects[i];
	}

	return animation;
};

void UnloadAnimation(SpriteAnim animation) {
	free(animation.rects);
}

void DrawAnimation(SpriteAnim animation, Rectangle dest) {
	int frame_index = (int)(GetTime() * animation.frames_per_sec) % animation.rects_len;
	Rectangle source = animation.rects[frame_index];

	DrawTexturePro(animation.atlas, source, dest, Vec2(0.0f, 0.0f), 0.0f, RAYWHITE);
}