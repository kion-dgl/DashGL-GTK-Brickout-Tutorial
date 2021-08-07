# Draw a Triangle

![bricks_002](https://user-images.githubusercontent.com/25621780/128594499-2228a4cd-81ca-4e41-8bf7-c15653df645f.png)

Once we have a window open, the next step is going to be a triangle. This step is quite a jump from simply 
opening a window as we will need to initialize OpenGL. We'll start with the main function, where we add a
GTKGlArea widget to our window. 

File: main.c
```c
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
```

We first create our glArea widget. And then tell the widget to expand to the full width and height of our
640x480 window with _gtk_widget_set_vexpand_ and _gtk_widget_set_hexpand_. From there we need to add two
events, with callbacks. The first is "realize", which will be called when the GtkGLArea is ready to initialize
so we can set up OpenGL. We also need to add a "render" event, that will describe what we want to draw to the
screen when our OpenGL context is ready. And then we add our GtkGLArea widget to the window. 

From here let's take a look at the "realize" callback.

```c
static void on_realize(GtkGLArea *area) {

	// Debug Message

	g_print("on realize\n");

	gtk_gl_area_make_current(area);
	if(gtk_gl_area_get_error(area) != NULL) {
		fprintf(stderr, "Unknown error\n");
		return;
	}

	const GLubyte *renderer = glGetString(GL_RENDER);
	const GLubyte *version = glGetString(GL_VERSION);

	printf("Renderer: %s\n", renderer);
	printf("OpenGL version supported %s\n", version);

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLfloat triangle_vertices[] = {
		 0.0,  0.8,
		-0.8, -0.8,
		 0.8, -0.8
	};
	
	glGenBuffers(1, &vbo_triangle);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_triangle);
	glBufferData(
		GL_ARRAY_BUFFER,
		sizeof(triangle_vertices),
		triangle_vertices,
		GL_STATIC_DRAW
	);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	glDisableVertexAttribArray(0);
	
	GLint compile_ok = GL_FALSE;
	GLint link_ok = GL_FALSE;

	const char *vs_source = 
	"#version 120\n"
	"attribute vec2 coord2d; \n"
	"void main (void) {\n"
	"	gl_Position = vec4(coord2d, 0.0, 1.0);\n"
	"}";

	const char *fs_source =
	"#version 120\n"
	"void main (void) {\n"
	"	gl_FragColor[0] = 0.0;\n"
	"	gl_FragColor[1] = 0.0;\n"
	"	gl_FragColor[2] = 1.0;\n"
	"}";

	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fs_source, NULL);
	glCompileShader(fs);
	glGetShaderiv(fs, GL_COMPILE_STATUS, &compile_ok);
	if(!compile_ok) {
		fprintf(stderr, "Error in fragment shader\n");
		return;
	}

	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vs_source, NULL);
	glCompileShader(vs);
	glGetShaderiv(vs, GL_COMPILE_STATUS, &compile_ok);
	if(!compile_ok) {
		fprintf(stderr, "Error in vertex shader\n");
		return;
	}

	program = glCreateProgram();
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &link_ok);
	if(!link_ok) {
		fprintf(stderr, "Error when linking program\n");
		return;
	}

	const char *attribute_name = "coord2d";
	attribute_coord2d = glGetAttribLocation(program, attribute_name);
	if(attribute_coord2d == -1) {
		fprintf(stderr, "Could not bind attribute %s\n", attribute_name);
		return;
	}
}
```

And I'll try and quickly cover what's going here. This should be thought of more 
as a "Hello World" aspect of OpenGL to get it started, you don't need to completely
understand everything upfront, as the familiarity will come with more use. 

First we call _gtk_gl_area_make_current_ which will make sure the OpenGL 
context uses our GtkGLArea widget. Then for a check we see which versions of 
OpenGL are supported. From there we set the clearColor to white, so that 
when we render, something to the screen it will be on a white background.

From there we set up what are called "Vertex Array Object" or vao. These are
required from OpenGL 3.0 and onward, which is what GTKGLArea uses. And these 
are more of an optimization, when drawing more complex scenes. We will set one up, 
and then only use that one, so just think of this as syntax for now. 

From there we set up the coorinates for our triangle. And then we set up our shaders.
Shaders are small programs that run on the GPU and tell the GPU what to do with the 
information that we pass into it. We have vertex shaders, which desribe where something
should be rendered, and fragment shaders that describe what color something will be.
We will cover this information in more detail in later steps. 

File: main.c
```c
static void on_render(GtkGLArea *area, GdkGLContext *context) {

	g_print("on render\n");

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(program);

	glBindVertexArray(vao);
	glEnableVertexAttribArray(attribute_coord2d);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_triangle);
	glVertexAttribPointer(
		attribute_coord2d,
		2,
		GL_FLOAT,
		GL_FALSE,
		0,
		0
	);

	glDrawArrays(GL_TRIANGLES, 0, 3);
	glDisableVertexAttribArray(attribute_coord2d);

}
```

And last is the render callback. First we clear everything to a white background.
Then we call tell OpenGL we want to use our shader program. We tell the shader
program how to interpret the 2d coordinates that we pass into it. And then we
call _glDrawArrays_ to draw our triangle to the screen. 

We compile with:
```
$ gcc `pkg-config --cflags gtk+-3.0` main.c `pkg-config --libs gtk+-3.0` -lepoxy
```

And then run our program with:
```
$ ./a.out
```
