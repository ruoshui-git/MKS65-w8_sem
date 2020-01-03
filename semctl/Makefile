CC = gcc
EXE = semctl

semctl: semctl.h semctl.c
	$(CC) -o semctl -ggdb3 semctl.c

.PHONY: clean

clean:
	rm -f $(EXE) story.txt