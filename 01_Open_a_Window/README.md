# Open a Window

![bricks_001](https://user-images.githubusercontent.com/25621780/128594082-788b466f-cde6-486f-837c-a88d94dd6fde.png)

 First we make a quick 'Hello, World!' program. To make sure our environment is set up and working. 
 
 File: main.c
 ```
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

Compile
```
$ gcc `pkg-config --cflags gtk+-3.0` main.c `pkg-config --libs gtk+-3.0`
```

Run:
```
./a.out
```
