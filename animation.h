#ifndef ANIMATION_H
#define ANIMATION_H

#include <raylib.h>

#define FRAMESIZE (32)
#define Vec2(x,y) (Vector2){(x),(y)} 
#define Rect(x,y,w,h) (Rectangle){(x),(y),(w),(h)}

typedef struct SpriteAnim {
	Texture2D atlas;
	int frames_per_sec;
	Rectangle* rects;
	int rects_len;
} SpriteAnim;

SpriteAnim createAnimation(Texture2D atlas, int frames_per_sec, Rectangle rects[], int length);
void UnloadAnimation(SpriteAnim animation);
void DrawAnimation(SpriteAnim animation, Rectangle dest);

#endif