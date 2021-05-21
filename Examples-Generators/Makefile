# -*- MakeFile -*-

all: generateS1 generateS2

generateS1: s1-generator.c s1-generator.h
	gcc -o generateS1 s1-generator.c -lm

generateS2: s2-generator.c s2-generator.h
	gcc -o generateS2 s2-generator.c -lm

clean:
	rm -f generateS1 generateS2
