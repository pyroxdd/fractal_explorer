#pragma once

#include <shared.h>

extern uvec2 screen_size;

// unsigned int create_texture(int width, int height, void *data);
// void render_texture(unsigned int tex_id, fvec2 position, fvec2 size, float rotation);
void render_rect(unsigned int tex_id, fvec2 in_position, fvec2 in_size, float rotation);
void gl_init();
void gl_clear();
void gl_resize();