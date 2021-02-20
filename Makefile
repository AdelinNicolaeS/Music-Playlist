build:
	gcc -Wall -Wextra -std=gnu99 playlist.c tema1.c -o tema1
clean:
	rm tema1
	rm *.o
