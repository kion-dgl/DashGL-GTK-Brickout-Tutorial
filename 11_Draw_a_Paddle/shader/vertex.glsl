#version 120

uniform mat4 mvp;
uniform mat4 orthograph;
attribute vec2 coord2d;

void main (void) {
	
	vec4 pixel_pos = mvp * vec4(coord2d, 0.0, 1.0);
	gl_Position = orthograph * pixel_pos;

}
