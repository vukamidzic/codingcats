#include "emoji.h"

#define Vec2(x,y) (Vector2){(x),(y)} 
#define COLOR(r,g,b,a) (Color){r,g,b,a}

void DrawEmoji(Emoji* emoji) {
    DrawTexturePro(
        emoji->tex,
        (Rectangle) {.x = 0, .y = 0, .width = 16, .height = 16},
        (Rectangle) {
            .x = emoji->x, 
            .y = emoji->y,
            .width = 32, 
            .height = 32
        },
        Vec2(0.0f, 0.0f),
        0.0f,
        COLOR(255, 255, 255, emoji->opacity)
    );

    emoji->y -= 0.025f;
    emoji->opacity -= 0.05f;
    if (emoji->opacity <= 0.0f) emoji->state = EMOJI_IDLE;
}