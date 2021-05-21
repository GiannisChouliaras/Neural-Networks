#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define NUMBER_OF_EXAMPLES 900
#define NUMBER_OF_EXAMPLES_PER_AREA 150
#define FALSE 0
#define TRUE 1

struct Point {
	float x1;
	float x2;
};

struct Point points[NUMBER_OF_EXAMPLES];

void generateRandomPointsPerArea(float minX1, float maxX1, float minX2, float maxX2, int numberOfArea);
int alreadyInArray(float randomX1, float randomX2, int numberOfArea, int pointsCreatedInTheArea);
void exportExamplesToTxt(char *fileName);
