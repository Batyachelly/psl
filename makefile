all:
	# gcc -o psl1 main.c -lncurses -lcurl -lSDL2 -lSDL2_mixer -lpthread
	gcc -o psl.out main.c view.c connect.c audioplay.c -lcurl -lncurses -lpthread -lSDL2 -lSDL2_mixer
	./psl.out