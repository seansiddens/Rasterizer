rasterizer: main.c rasterizer.c util.c
	gcc main.c rasterizer.c util.c -o bin/rasterizer -lm