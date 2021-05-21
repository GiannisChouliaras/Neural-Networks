#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <errno.h>
#include <string.h>

/* Macro for checking file opening */
#define CHECK_FILE(X) ((X) == NULL ? ({fprintf(stderr, "ERROR: (" __FILE__ ":%d)\
 -- %s\n",__LINE__,strerror(errno)); exit(-1);}) : (printf("")))

#define BBLK "\e[1;30m"
#define KRED "\x1B[31m"
#define KGRN "\x1B[32m"
#define KCYN "\x1B[36m"

#define NUMBER_OF_EXAMPLES 900
#define FALSE 0
#define TRUE 1
#define M 5	// number of teams

struct Point {
	float x1;
	float x2;
	int nearestCenter;
};

struct Point examples[NUMBER_OF_EXAMPLES];
struct Point centers[M];
struct Point centersBefore[M];
float error = 0.0;
float n = 0.1;	// learning rate

void loadExamples(char* fileName);
void lvq();
void createFile();
void silhouette();
void selectRandomCenters();
void calcError();

int hasBeenSelected(int* examplesPos, int numOfCentersSelected, int randomInt);
int centersMoved();

float calcEuclidDist(int center, int example);
