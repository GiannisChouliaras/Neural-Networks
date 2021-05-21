#include "s1-generator.h"

int main () {
	generateRandomPoints();
	divideIntoCategories();
	addNoise();
	exportExamplesToTxt("../Examples/S1/training-set.txt", "../Examples/S1/test-set.txt");

	return(0);
}

void generateRandomPoints() {
	int positionsFilled = 0;
	
	time_t t;
	srand((unsigned) time(&t));

	while (positionsFilled != NUMBER_OF_EXAMPLES) {
		float randomX1 = rand() / (float) RAND_MAX;	// [0, 1.0]
		randomX1 = MIN + randomX1 * (MAX - MIN);	// [MIN, MAX]
		float randomX2 = rand() / (float) RAND_MAX;	// [0, 1.0]
		randomX2 = MIN + randomX2 * (MAX - MIN);	// [MIN, MAX]

		if (!alreadyInArray(randomX1, randomX2, positionsFilled)) {
			examples[positionsFilled].x1 = randomX1;
			examples[positionsFilled].x2 = randomX2;
			positionsFilled++;
		}
	}
}

int alreadyInArray(float randomX1, float randomX2, int positionsFilled) {
	for (int i = 0; i < positionsFilled; i++)
		if ((examples[i].x1 == randomX1) && (examples[i].x2 == randomX2))
			return TRUE;

	return FALSE;
}

void divideIntoCategories() {
	for (int i = 0; i < NUMBER_OF_EXAMPLES; i ++) {
		if (powf(examples[i].x1, 2) + (powf(examples[i].x2, 2)) < 0.25)
			examples[i].category = C1;
		else if ((examples[i].x1 < -0.4 && examples[i].x2 < -0.4) ||
				 (examples[i].x1 > 0.4 && examples[i].x2 > 0.4))
			examples[i].category = C2;
		else if ((examples[i].x1 < -0.4 && examples[i].x2 > 0.4) ||
				 (examples[i].x1 > 0.4 && examples[i].x2 < -0.4))
			examples[i].category = C3;
		else
			examples[i].category = C4;
	}
}

void addNoise() {
	time_t t;
	srand((unsigned) time(&t));
	
	for (int i = 0; i < NUMBER_OF_EXAMPLES; i++) {
		float randomNumber = (float) rand() / (RAND_MAX);
		randomNumber = pointOneFloat(randomNumber);

		if ((randomNumber * 10) == 1)
			examples[i].category = C4;
	}
}

float pointOneFloat(float floatBefore) { 
    float floatAfter = (int)(floatBefore * 10 + 0.5);
    return (float)floatAfter / 10;
}

void exportExamplesToTxt(char *fileName1, char *fileName2) {
	FILE *fp;
	
	if ((fp = fopen(fileName1, "w")) == NULL) {
		printf("Could not open training-set file, check the path!\n");
		exit(1);
	}
	
	for (int i = 0; i < (NUMBER_OF_EXAMPLES / 2); i++)
		fprintf(fp, " %f\t%f\t%d \n", examples[i].x1, examples[i].x2, examples[i].category);
	
	fclose(fp);
	
	if ((fp = fopen(fileName2, "w")) == NULL) {
		printf("Could not open test-set file, check the path!\n");
		exit(1);
	}

	for (int i = (NUMBER_OF_EXAMPLES / 2); i < NUMBER_OF_EXAMPLES; i++)
		fprintf(fp, " %f\t%f\t%d \n", examples[i].x1, examples[i].x2, examples[i].category);
	
	fclose(fp);
}
