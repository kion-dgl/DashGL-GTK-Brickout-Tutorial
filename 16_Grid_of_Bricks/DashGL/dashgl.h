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

#ifndef DASHGL_UTILS
#define DASHGL_UTILS
	

	/**********************************************************************/
	/** Type Definitions                                                 **/	
	/**********************************************************************/

	typedef float vec2[2];
	typedef float vec3[3];
	typedef float mat3[9];
	typedef float mat4[16];

	#define M3_00 0
	#define M3_01 1
	#define M3_02 2
	#define M3_10 3
	#define M3_11 4
	#define M3_12 5
	#define M3_20 6
	#define M3_21 7
	#define M3_22 8

	#define M4_00 0
	#define M4_10 1
	#define M4_20 2
	#define M4_30 3
	#define M4_01 4
	#define M4_11 5
	#define M4_21 6
	#define M4_31 7
	#define M4_02 8
	#define M4_12 9
	#define M4_22 10
	#define M4_32 11
	#define M4_03 12
	#define M4_13 13
	#define M4_23 14
	#define M4_33 15

	/**********************************************************************/
	/** Shader Utilities                                                 **/	
	/**********************************************************************/
	
	void shader_print_log(GLuint object);
	GLuint shader_load_file(const char *filename, GLenum type);
	GLuint shader_load_program(const char *vertex, const char *fragment);
	GLuint shader_load_texture(const char *filename);
	
	/**********************************************************************/
	/** Vector2 Utilities                                                **/	
	/**********************************************************************/

	void vec2_print(vec2 coord);

	/**********************************************************************/
	/** Matrix3 Utilities                                                **/	
	/**********************************************************************/
	
	void mat3_identity(mat3 in);
	void mat3_copy(mat3 in, mat3 out);
	void mat3_multiply(mat3 a, mat3 b, mat3 out);
	void mat3_translate(mat3 in, vec2 pos, mat3 out);

	/**********************************************************************/
	/** Matrix4 Utilities                                                **/	
	/**********************************************************************/
	
	// Basic Utilities

	void mat4_identity(mat4 out);
	void mat4_copy(mat4 in, mat4 out);
	void mat4_multiply(mat4 a, mat4 b, mat4 out);
	
	// Transformation Utilities

	void mat4_translate(vec3 pos, mat4 out);

	// Projection Utilities

	void mat4_orthagonal(float width, float height, mat4 out);

	/**********************************************************************/
	/** End Library Definitions                                          **/	
	/**********************************************************************/

#endif
