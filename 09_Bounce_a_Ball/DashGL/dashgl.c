/*
    This file is part of Dash Graphics Library
    Copyright 2017 Benjamin Collins

    Permission is hereby granted, free of charge, to any person obtaining 
    a copy of this software and associated documentation files (the
    "Software"), to deal in the Software without restriction, including 
    without limitation the rights to use, copy, modify, merge, publish, 
    distribute, sublicense, and/or sell copies of the Software, and to 
    permit persons to whom the Software is furnished to do so, subject to 
    the following conditions:

    The above copyright notice and this permission notice shall be included     
    in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
    OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF 
    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
    IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY 
    CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, 
    TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE 
    SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
    
*/

#include <stdio.h>
#include <stdlib.h>
#include <epoxy/gl.h>
#include <epoxy/glx.h>
#include "dashgl.h"

/**********************************************************************/
/** Shader Utilities                                                 **/	
/**********************************************************************/
	
void shader_print_log(GLuint object) {

	char *log;
	GLint log_length;

	if (glIsShader(object)) {
		glGetShaderiv(object, GL_INFO_LOG_LENGTH, &log_length);
	} else if (glIsProgram(object)) {
		glGetProgramiv(object, GL_INFO_LOG_LENGTH, &log_length);
	} else {
		fprintf(stderr, "Not a shader or program\n");
		return;
	}

	log = (char*)malloc(log_length);

	if(glIsShader(object)) {
		glGetShaderInfoLog(object, log_length, NULL, log);
	} else if(glIsProgram(object)) {
		glGetProgramInfoLog(object, log_length, NULL, log);
	}

	fprintf(stderr, "%s\n", log);
	free(log);

}

GLuint shader_load_file(const char *filename, GLenum type) {

	FILE *fp;
	unsigned int file_len;
	char *source;
	GLuint shader;
	GLint compile_ok;

	fp = fopen(filename, "r");
	if(fp == NULL) {
		fprintf(stderr, "Could not open %s for reading\n", filename);
		return 0;
	}
	
	fseek(fp, 0, SEEK_END);
	file_len = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	source = malloc(file_len + 1);
	fread(source, file_len, sizeof(char), fp);
	fclose(fp);
	source[file_len] = '\0';
	
	const GLchar *sources[] = { source };

	shader = glCreateShader(type);
	glShaderSource(shader, 1, sources, NULL);
	glCompileShader(shader);
	
	free(source);
	
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_ok);
	if(compile_ok == GL_FALSE) {
		fprintf(stderr, "Shader compile error: %s\n", filename);
		shader_print_log(shader);
		glDeleteShader(shader);
		return 0;
	}

	return shader;

}

GLuint shader_load_program(const char *vertex, const char *fragment) {

	GLint link_ok;
	GLuint program;

	GLuint vs = shader_load_file(vertex, GL_VERTEX_SHADER);
	GLuint fs = shader_load_file(fragment, GL_FRAGMENT_SHADER);

	if(vs == 0 || fs == 0) {
		return 0;
	}

	program = glCreateProgram();
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &link_ok);
	if(!link_ok) {
		fprintf(stderr, "Program Link Error: ");
		shader_print_log(program);
		return 0;
	}

	return program;

}

GLuint shader_load_texture(const char *filename) {

}


/**********************************************************************/
/** Vec2 Utilities                                                   **/	
/**********************************************************************/

void vec2_print(vec2 coord) {

	printf("vec2: %.2f, %.2f\n", coord[0], coord[1]);

}

/**********************************************************************/
/** Mat3 Utilities                                                   **/	
/**********************************************************************/

void mat3_identity(mat3 in) {

	in[M3_00] = 1.0f;
	in[M3_10] = 0.0f;
	in[M3_20] = 0.0f;

	in[M3_01] = 0.0f;
	in[M3_11] = 1.0f;
	in[M3_21] = 0.0f;

	in[M3_02] = 0.0f;
	in[M3_12] = 0.0f;
	in[M3_22] = 1.0f;

}

void mat3_copy(mat3 in, mat3 out) {

	out[M3_00] = in[M3_00];
	out[M3_10] = in[M3_10];
	out[M3_20] = in[M3_20];
	
	out[M3_01] = in[M3_00];
	out[M3_11] = in[M3_11];
	out[M3_21] = in[M3_22];

	out[M3_02] = in[M3_02];
	out[M3_12] = in[M3_12];
	out[M3_22] = in[M3_22];

}

void mat3_multiply(mat3 a, mat3 b, mat3 out) {

	mat3 tmp;

	tmp[M3_00] = a[M3_00]*b[M3_00]+a[M3_01]*b[M3_10]+a[M3_02]*b[M3_20];
	tmp[M3_10] = a[M3_10]*b[M3_00]+a[M3_11]*b[M3_10]+a[M3_12]*b[M3_20];
	tmp[M3_20] = a[M3_20]*b[M3_00]+a[M3_21]*b[M3_10]+a[M3_22]*b[M3_20];

	tmp[M3_01] = a[M3_00]*b[M3_01]+a[M3_01]*b[M3_11]+a[M3_02]*b[M3_21];
	tmp[M3_11] = a[M3_10]*b[M3_01]+a[M3_11]*b[M3_11]+a[M3_12]*b[M3_21];
	tmp[M3_21] = a[M3_20]*b[M3_01]+a[M3_21]*b[M3_11]+a[M3_22]*b[M3_21];

	tmp[M3_02] = a[M3_00]*b[M3_02]+a[M3_01]*b[M3_12]+a[M3_02]*b[M3_22];
	tmp[M3_12] = a[M3_10]*b[M3_02]+a[M3_11]*b[M3_12]+a[M3_12]*b[M3_22];
	tmp[M3_22] = a[M3_20]*b[M3_02]+a[M3_21]*b[M3_12]+a[M3_22]*b[M3_22];

	mat3_copy(tmp, out);

}


void mat3_translate(mat3 in, vec2 pos, mat3 out) {

	mat3 tmp;

	tmp[M3_00] = 1.0f;
	tmp[M3_10] = 0.0f;
	tmp[M3_20] = 0.0f;

	tmp[M3_01] = 0.0f;
	tmp[M3_11] = 1.0f;
	tmp[M3_21] = 0.0f;

	tmp[M3_02] = pos[0];
	tmp[M3_12] = pos[1];
	tmp[M3_22] = 1.0f;

	mat3_multiply(in, tmp, out);

}

/**********************************************************************/
/** Mat4 Utilities                                                   **/	
/**********************************************************************/


void mat4_identity(mat4 out) {

	out[M4_00] = 1.0f;
	out[M4_01] = 0.0f;
	out[M4_02] = 0.0f;
	out[M4_03] = 0.0f;
	out[M4_10] = 0.0f;
	out[M4_11] = 1.0f;
	out[M4_12] = 0.0f;
	out[M4_13] = 0.0f;
	out[M4_20] = 0.0f;
	out[M4_21] = 0.0f;
	out[M4_22] = 1.0f;
	out[M4_23] = 0.0f;
	out[M4_30] = 0.0f;
	out[M4_31] = 0.0f;
	out[M4_32] = 0.0f;
	out[M4_33] = 1.0f;

}

void mat4_copy(mat4 in, mat4 out) {

	out[M4_00] = in[M4_00];
	out[M4_01] = in[M4_01];
	out[M4_02] = in[M4_02];
	out[M4_03] = in[M4_03];
	out[M4_10] = in[M4_10];
	out[M4_11] = in[M4_11];
	out[M4_12] = in[M4_12];
	out[M4_13] = in[M4_13];
	out[M4_20] = in[M4_20];
	out[M4_21] = in[M4_21];
	out[M4_22] = in[M4_22];
	out[M4_23] = in[M4_23];
	out[M4_30] = in[M4_30];
	out[M4_31] = in[M4_31];
	out[M4_32] = in[M4_32];
	out[M4_33] = in[M4_33];

}

void mat4_multiply(mat4 a, mat4 b, mat4 out) {

	mat4 tmp;

	tmp[M4_00] = a[M4_00]*b[M4_00]+a[M4_01]*b[M4_10]+a[M4_02]*b[M4_20]+a[M4_03]*b[M4_30];
	tmp[M4_01] = a[M4_00]*b[M4_01]+a[M4_01]*b[M4_11]+a[M4_02]*b[M4_21]+a[M4_03]*b[M4_31];
	tmp[M4_02] = a[M4_00]*b[M4_02]+a[M4_01]*b[M4_12]+a[M4_02]*b[M4_22]+a[M4_03]*b[M4_32];
	tmp[M4_03] = a[M4_00]*b[M4_03]+a[M4_01]*b[M4_13]+a[M4_02]*b[M4_23]+a[M4_03]*b[M4_33];
   
	tmp[M4_10] = a[M4_10]*b[M4_00]+a[M4_11]*b[M4_10]+a[M4_12]*b[M4_20]+a[M4_13]*b[M4_30];
	tmp[M4_11] = a[M4_10]*b[M4_01]+a[M4_11]*b[M4_11]+a[M4_12]*b[M4_21]+a[M4_13]*b[M4_31];
	tmp[M4_12] = a[M4_10]*b[M4_02]+a[M4_11]*b[M4_12]+a[M4_12]*b[M4_22]+a[M4_13]*b[M4_32];
	tmp[M4_13] = a[M4_10]*b[M4_03]+a[M4_11]*b[M4_13]+a[M4_12]*b[M4_23]+a[M4_13]*b[M4_33];
 
	tmp[M4_20] = a[M4_20]*b[M4_00]+a[M4_21]*b[M4_10]+a[M4_22]*b[M4_20]+a[M4_23]*b[M4_30];
	tmp[M4_21] = a[M4_20]*b[M4_01]+a[M4_21]*b[M4_11]+a[M4_22]*b[M4_21]+a[M4_23]*b[M4_31];
	tmp[M4_22] = a[M4_20]*b[M4_02]+a[M4_21]*b[M4_12]+a[M4_22]*b[M4_22]+a[M4_23]*b[M4_32];
	tmp[M4_23] = a[M4_20]*b[M4_03]+a[M4_21]*b[M4_13]+a[M4_22]*b[M4_23]+a[M4_23]*b[M4_33];
 
	tmp[M4_30] = a[M4_30]*b[M4_00]+a[M4_31]*b[M4_10]+a[M4_32]*b[M4_20]+a[M4_33]*b[M4_30];
	tmp[M4_31] = a[M4_30]*b[M4_01]+a[M4_31]*b[M4_11]+a[M4_32]*b[M4_21]+a[M4_33]*b[M4_31];
	tmp[M4_32] = a[M4_30]*b[M4_02]+a[M4_31]*b[M4_12]+a[M4_32]*b[M4_22]+a[M4_33]*b[M4_32];
	tmp[M4_33] = a[M4_30]*b[M4_03]+a[M4_31]*b[M4_13]+a[M4_32]*b[M4_23]+a[M4_33]*b[M4_33];

	mat4_copy(tmp, out);

}


void mat4_translate(vec3 pos, mat4 out) {

	out[M4_00] = 1.0f;
	out[M4_10] = 0.0f;
	out[M4_20] = 0.0f;
	out[M4_30] = 0.0f;
	
	out[M4_01] = 0.0f;
	out[M4_11] = 1.0f;
	out[M4_21] = 0.0f;
	out[M4_31] = 0.0f;
	
	out[M4_02] = 0.0f;
	out[M4_12] = 0.0f;
	out[M4_22] = 1.0f;
	out[M4_32] = 0.0f;
	
	out[M4_03] = pos[0];
	out[M4_13] = pos[1];
	out[M4_23] = pos[2];
	out[M4_33] = 1.0f;

}

void mat4_orthagonal(float width, float height, mat4 out) {
	
	float right, left, top, bottom;
	bottom = 0;
	top = height;
	left = 0;
	right = width;

	float zNear = -0.1f;
	float zFar = 1.0f;
	float inv_z = 1.0f / (zFar - zNear);
	float inv_y = 1.0f / (top - bottom);
	float inv_x = 1.0f / (right - left);

	out[M4_00] = 2.0f * inv_x;
	out[M4_10] = 0.0f;
	out[M4_20] = 0.0f;
	out[M4_30] = 0.0f;

	out[M4_01] = 0.0f;
	out[M4_11] = 2.0f * inv_y;
	out[M4_21] = 0.0f;
	out[M4_31] = 0.0f;

	out[M4_02] = 0.0f;
	out[M4_12] = 0.0f;
	out[M4_22] = -2.0f * inv_z;
	out[M4_32] = 0.0f;

	out[M4_03] = -(right + left)*inv_x;
	out[M4_13] = -(top + bottom)*inv_y;
	out[M4_23] = -(zFar + zNear)*inv_z;
	out[M4_33] = 1.0f;

}

/**********************************************************************/
/** End Library Definitions                                          **/	
/**********************************************************************/


