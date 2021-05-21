#include "s2-generator.h"

int main () {
	generateRandomPointsPerArea(0.75, 1.25, 0.75, 1.25, 0);	// [0.75,1.25]x[0.75,1.25]
	generateRandomPointsPerArea(0.0, 0.5, 0.0, 0.5, 1);		// [0, 0.5]x[0,0.5]
	generateRandomPointsPerArea(0.0, 0.5, 1.5, 2.0, 2);		// [0,0.5] x [1.5,2]
	generateRandomPointsPerArea(1.5, 2.0, 0.0, 0.5, 3);		// [1.5,2]x[0,0.5]
	generateRandomPointsPerArea(1.5, 2.0, 1.5, 2.0, 4);		// [1.5,2] x [1.5,2]
	generateRandomPointsPerArea(0.0, 2.0, 0.0, 2.0, 5);		// [0,2]x[0,2]
	
	exportExamplesToTxt("../Examples/S2.txt");

	return(0);
}

void generateRandomPointsPerArea(float minX1, float maxX1, float minX2, float maxX2, int numberOfArea) {
	int pointsCreatedInTheArea = 0;
	
	time_t t;
	srand((unsigned) time(&t));
	
	while (pointsCreatedInTheArea != NUMBER_OF_EXAMPLES_PER_AREA) {
		float randomX1 = rand() / (float) RAND_MAX;		// [0, 1.0]
		randomX1 = minX1 + randomX1 * (maxX1 - minX1);	// [min, max]
		float randomX2 = rand() / (float) RAND_MAX;		// [0, 1.0]
		randomX2 = minX2 + randomX2 * (maxX2 - minX2);	// [min, max]

		if (!alreadyInArray(randomX1, randomX2, numberOfArea, pointsCreatedInTheArea)) {
			points[(numberOfArea * 150) + pointsCreatedInTheArea].x1 = randomX1;
			points[(numberOfArea * 150) + pointsCreatedInTheArea].x2 = randomX2;
			pointsCreatedInTheArea += 1;
		}
	}
}

int alreadyInArray(float randomX1, float randomX2, int numberOfArea, int pointsCreatedInTheArea) {
	for (int i = (numberOfArea * 150); i < (numberOfArea * 150) + pointsCreatedInTheArea; i++)
		if ((points[i].x1 == randomX1) && (points[i].x2 == randomX2))
			return TRUE;

	return FALSE;
}

void exportExamplesToTxt(char *fileName) {
	FILE *fp;
	
	if ((fp = fopen(fileName, "w")) == NULL) {
		printf("Could not open file, check the path!\n");
		exit(1);
	}

	for (int i = 0; i < NUMBER_OF_EXAMPLES; i++)
		fprintf(fp, " %f\t%f\t \n", points[i].x1, points[i].x2);
	
	fclose(fp);
}
