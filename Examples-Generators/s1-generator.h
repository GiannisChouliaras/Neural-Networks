#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define NUMBER_OF_EXAMPLES 6000
#define MIN -1
#define MAX 1
#define FALSE 0
#define TRUE 1

struct Point {
	float x1;
	float x2;
	int category;
};

struct Point examples[NUMBER_OF_EXAMPLES];
enum catagories{C1 = 1, C2, C3, C4};

void generateRandomPoints();
int alreadyInArray(float randomX1, float randomX2, int positionsFilled);
void divideIntoCategories();
void addNoise();
float pointOneFloat(float floatBefore);
void exportExamplesToTxt(char *fileName1, char *fileName2);
