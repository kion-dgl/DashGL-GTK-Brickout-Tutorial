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
/** End Library Definitions                                          **/	
/**********************************************************************/


