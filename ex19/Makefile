CFLAGS=-Wall -g

all: ex19

ex19: object.o hall_of_the_minotaur.o

clean:
	rm -f ex19 object.o hall_of_the_minotaur.o

test:
	valgrind ./ex19 < test_script1.txt
