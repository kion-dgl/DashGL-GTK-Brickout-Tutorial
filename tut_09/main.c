/*
 *  This file is part of DashGL.com - Gtk - Brickout Tutorial
 *  Copyright (C) 2017 Benjamin Collins - MIT License
 */

#include <GL/glew.h>
#include <gtk/gtk.h>
#include <math.h>
#include "DashGL/dashgl.h"

#define WIDTH 640.0f
#define HEIGHT 480.0f

static void on_realize(GtkGLArea *area);
static void on_render(GtkGLArea *area, GdkGLContext *context);
static gboolean on_idle(gpointer data);

float dx, dy;
vec3 pos = { 50.0f, 50.0f, 0.0f };
mat4 mvp;

GLuint program;
GLuint vao, vbo_circle;
GLint attribute_coord2d;
GLint uniform_mvp;

int main(int argc, char *argv[]) {

	GtkWidget *window;
	GtkWidget *glArea;

	gtk_init(&argc, &argv);

	// Initialize Window

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "Brickout Tutorial");
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_window_set_default_size(GTK_WINDOW(window), 640, 480);
	gtk_window_set_type_hint(GTK_WINDOW(window), GDK_WINDOW_TYPE_HINT_UTILITY);
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

	// Initialize GTK GL Area

	glArea = gtk_gl_area_new();
	gtk_widget_set_vexpand(glArea, TRUE);
	gtk_widget_set_hexpand(glArea, TRUE);
	g_signal_connect(glArea, "realize", G_CALLBACK(on_realize), NULL);
	g_signal_connect(glArea, "render", G_CALLBACK(on_render), NULL);
	gtk_container_add(GTK_CONTAINER(window), glArea);

	// Show widgets

	gtk_widget_show_all(window);
	gtk_main();

	return 0;

}

static void on_realize(GtkGLArea *area) {

	// Debug Message

	g_print("on realize\n");

	gtk_gl_area_make_current(area);
	if(gtk_gl_area_get_error(area) != NULL) {
		fprintf(stderr, "Unknown error\n");
		return;
	}

	glewExperimental = GL_TRUE;
	glewInit();

	const GLubyte *renderer = glGetString(GL_RENDER);
	const GLubyte *version = glGetString(GL_VERSION);
	const GLubyte *shader = glGetString(GL_SHADING_LANGUAGE_VERSION);

	printf("Shader %s\n", shader);
	printf("Renderer: %s\n", renderer);
	printf("OpenGL version supported %s\n", version);

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	int i;
	float angle, nextAngle;
	int num_segments = 100;

	GLfloat circle_vertices[6 * 100];
	
	for(i = 0; i < num_segments; i++) {

		angle = i * 2.0f * M_PI / (num_segments - 1);
		nextAngle = (i+1) * 2.0f * M_PI / (num_segments - 1);

		circle_vertices[i*6 + 0] = cos(angle) * 20;
		circle_vertices[i*6 + 1] = sin(angle) * 20;

		circle_vertices[i*6 + 2] = cos(nextAngle) * 20;
		circle_vertices[i*6 + 3] = sin(nextAngle) * 20;

		circle_vertices[i*6 + 4] = 0.0f;
		circle_vertices[i*6 + 5] = 0.0f;

	}

	glGenBuffers(1, &vbo_circle);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_circle);
	glBufferData(
		GL_ARRAY_BUFFER,
		sizeof(circle_vertices),
		circle_vertices,
		GL_STATIC_DRAW
	);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	glDisableVertexAttribArray(0);
	
	const char *vs = "shader/vertex.glsl";
	const char *fs = "shader/fragment.glsl";

	program = shader_load_program(vs, fs);

	const char *attribute_name = "coord2d";
	attribute_coord2d = glGetAttribLocation(program, attribute_name);
	if(attribute_coord2d == -1) {
		fprintf(stderr, "Could not bind attribute %s\n", attribute_name);
		return;
	}

	const char *uniform_name = "orthograph";
	GLint uniform_ortho = glGetUniformLocation(program, uniform_name);
	if(uniform_ortho == -1) {
		fprintf(stderr, "Could not bind uniform %s\n", uniform_name);
		return;
	}
	
	uniform_name = "mvp";
	uniform_mvp = glGetUniformLocation(program, uniform_name);
	if(uniform_mvp == -1) {
		fprintf(stderr, "Could not bind uniform %s\n", uniform_name);
		return;
	}

	glUseProgram(program);

	mat4 orthograph;
	mat4_orthagonal(WIDTH, HEIGHT, orthograph);

	glUniformMatrix4fv(uniform_ortho, 1, GL_FALSE, orthograph);
	g_timeout_add(20, on_idle, (void*)area);
	
	dx = 2.0f;
	dy = 3.0f;

}

static void on_render(GtkGLArea *area, GdkGLContext *context) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	mat4_translate(pos, mvp);
	glUniformMatrix4fv(uniform_mvp, 1, GL_FALSE, mvp);

	glBindVertexArray(vao);
	glEnableVertexAttribArray(attribute_coord2d);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_circle);
	glVertexAttribPointer(
		attribute_coord2d,
		2,
		GL_FLOAT,
		GL_FALSE,
		0,
		0
	);

	glDrawArrays(GL_TRIANGLES, 0, 3 * 100);
	glDisableVertexAttribArray(attribute_coord2d);

}

static gboolean on_idle(gpointer data) {

	pos[0] += dx;
	pos[1] += dy;

	if(pos[0] > WIDTH) {
		pos[0] = WIDTH;
		dx *= -1;
	} else if(pos[0] < 0) {
		pos[0] = 0;
		dx *= -1;
	}

	if(pos[1] > HEIGHT) {
		pos[1] = HEIGHT;
		dy *= -1;
	} else if(pos[1] < 0) {
		pos[1] = 0;
		dy *= -1;
	}

	gtk_widget_queue_draw(GTK_WIDGET(data));
	return TRUE;

}
