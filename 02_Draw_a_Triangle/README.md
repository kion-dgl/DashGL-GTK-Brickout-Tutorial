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
