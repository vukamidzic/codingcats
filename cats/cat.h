#ifndef CAT_H
#define CAT_H

#include <raylib.h>
#include "../animation.h"
#include "../timer.h"

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

#define NAMEMAX (100)

typedef struct Cat {
    char name[NAMEMAX];
    Texture2D tex;
    float x;
    float y;
    SpriteAnim sit;
    SpriteAnim idle;
    SpriteAnim left_walk;
    SpriteAnim right_walk;
    float velocity;
    Direction dir;
    State state;
    Timer timer;
    Timer timer1;
} Cat;

Cat createCat(char name[], float x, float y, float velocity);
void PlayAnim(Cat* cat);
void UnloadCat(Cat cat);
void UpdateCat(Cat* cat);

#endif 