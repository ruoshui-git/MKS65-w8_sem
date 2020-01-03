CC = gcc

all: write semctl

DFLAGS = -ggdb3

write: const.h semctl.h write.c lock.o
	$(CC) -o write $(DFLAGS) lock.o write.c

semctl: semctl.h const.h semctl.c lock.o
	$(CC) -o semctl $(DFLAGS) lock.o semctl.c
 
lock.o: lock.c lock.h
	$(CC) -c $(DFLAGS) lock.c

.PHONY: clean

clean:
	rm -f write semctl story.txt *.o