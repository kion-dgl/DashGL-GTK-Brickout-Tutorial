all:
	gcc -c -o DashGL/dashgl.o DashGL/dashgl.c -lGL -lepoxy -lpng
	gcc `pkg-config --cflags gtk+-3.0` main.c DashGL/dashgl.o `pkg-config --libs gtk+-3.0` -lepoxy -lGL
