build:
	g++ -o tetris -Wall main.cpp Map.cpp MapObject.cpp `pkg-config --cflags --libs --static allegro-5.0  allegro_color-5.0 allegro_image-5.0 allegro_main-5.0 allegro_memfile-5.0 allegro_physfs-5.0 allegro_primitives-5.0 allegro_ttf-5.0`
run: build
	./tetris

