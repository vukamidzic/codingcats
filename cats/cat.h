#ifndef CAT_H
#define CAT_H

#include <raylib.h>
#include "../animation.h"
#include "../timer.h"
#include "../emojis/emoji.h"

#define Vec2(x,y) (Vector2){(x),(y)} 
#define Rect(x,y,w,h) (Rectangle){(x),(y),(w),(h)}

typedef enum Direction {
    LEFT,
    RIGHT 
} Direction;

typedef enum State {
    WALK,
    SIT,
    IDLE
} State;

#define PATHMAX (100)

typedef struct Cat {
    Texture2D tex;
    float x;
    float y;
    float velocity;
    State state;
    Direction dir;
    // SpriteAnim sit;
    // SpriteAnim idle;
    // SpriteAnim left_walk;
    // SpriteAnim right_walk;
    SpriteAnim anims[4];
    // Timer timer;
    // Timer timer1;
    Timer timers[2];
    Emoji emoji;
} Cat;

Cat createCat(char name[], float x, float y, float velocity);
void PlayAnim(Cat* cat);
void UnloadCat(Cat cat);
void UpdateCat(Cat* cat);

#endif 