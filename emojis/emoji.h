#ifndef EMOJI_H
#define EMOJI_H

#include <raylib.h>

typedef enum EmojiState {
    EMOJI_IDLE,
    EMOJI_ACTIVE
} EmojiState;

typedef struct Emoji {
    Texture2D tex;
    float x;
    float y;
    float opacity;
    EmojiState state;
} Emoji;

void DrawEmoji(Emoji* emoji);

#endif