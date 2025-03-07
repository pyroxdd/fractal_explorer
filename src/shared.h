#pragma once

struct ivec2 {
    int x;
    int y;
};

struct uvec2 {
    unsigned int x;
    unsigned int y;
};

struct fvec2 {
    float x;
    float y;
};

struct input_struct {
    bool lmb;
    bool rmb;
    bool lmb_now;
    bool rmb_now;
    int scroll;
    ivec2 mouse_pos;
    bool left;
    bool right;
    bool up;
    bool down;
};