# Open a Window

![bricks_001](https://user-images.githubusercontent.com/25621780/128594082-788b466f-cde6-486f-837c-a88d94dd6fde.png)

 First we make a quick 'Hello, World!' program. To make sure our environment is set up and working. 
 
 File: main.c
 ```c
 #include <gtk/gtk.h>

int main(int argc, char *argv[]) {

	GtkWidget *window;

	gtk_init(&argc, &argv);

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "Brickout Tutorial");
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_window_set_default_size(GTK_WINDOW(window), 640, 480);
	gtk_window_set_type_hint(GTK_WINDOW(window), GDK_WINDOW_TYPE_HINT_UTILITY);
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

	gtk_widget_show_all(window);

	gtk_main();

	return 0;

}
```

While there's not much going here, let's try to break down and identify what is happening. I would imagine that most of the syntax
here is self-explanitory, but I will try none-the-less. We create a new window with _gtk_window_new_. We then give the window a
title with _gtk_window_set_title_. We then define where we want the window to appear on the screen initialially. Where we are using
GTK_WIN_POS_CENTER, but we will list the other options below. 

| Name  | Description  |
|---|---|
| GTK_WIN_POS_NONE | No influence is made on placement.  |
| GTK_WIN_POS_CENTER  | Windows should be placed in the center of the screen. |
| GTK_WIN_POS_MOUSE  |  Windows should be placed at the current mouse position. |
| GTK_WIN_POS_CENTER_ALWAYS | Keep window centered as it changes size, etc. |
| GTK_WIN_POS_CENTER_ON_PARENT | Center the window on its transient parent (see gtk_window_set_transient_for()) |

So basically for an application like this, center makes the most sense. After that we have a function to 
set the hint type. In most cases, this isn't actually needed. This is for added for tiling desktop environments such
as i3 to keep our application from stretching.

We call _g_signal_connect_ to completely exit out of the application once the window has been closed. Since GTK doesn't
completely exit the process when the window is closed. We call _gtk_widget_show_all_ to show the window to the user, and
then we call _gtk_main_ to start the application. 

Compile
```
$ gcc `pkg-config --cflags gtk+-3.0` main.c `pkg-config --libs gtk+-3.0`
```

Run:
```
./a.out
```
