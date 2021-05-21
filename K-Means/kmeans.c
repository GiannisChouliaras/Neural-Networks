#include "kmeans.h"

int main() {
	FILE *centers_data;
	loadExamples("../Examples/S2.txt");
    createFile();
	kmeans();
	
	CHECK_FILE(centers_data = fopen("centers.dat", "w+"));

	printf("\n%sFinal centers:\n\n", BBLK);

	for (int i = 0; i < M; i++) {
		printf("%s%f\t%f\n", KGRN, centers[i].x1, centers[i].x2);
		fprintf(centers_data, "%lf %lf\n", centers[i].x1, centers[i].x2);
	}
	fclose(centers_data);

	printf("\n%sError: %f\n\n%s", KRED, error, BBLK);


     system("gnuplot -p -e \"set title 'Kmeans with M = 5'; \
			 plot 'points.dat' notitle with points pt 1 lc 'dark-gray', \
            'centers.dat' notitle with points pt 7 lc 'light-red'\"");
	
	return EXIT_SUCCESS;
}

void loadExamples(char* fileName) {
	FILE *fp;

	CHECK_FILE(fp = fopen(fileName, "r"));

	for (int i = 0; i < NUMBER_OF_EXAMPLES; i++)
		fscanf(fp, " %f\t%f\t \n", &examples[i].x1, &examples[i].x2);

	fclose(fp);
}

void createFile() {
	FILE * points_data;
	int i;

	CHECK_FILE(points_data = fopen("points.dat", "w+"));

	for (i = 0; i < NUMBER_OF_EXAMPLES; i++) 
		fprintf(points_data, "%lf %lf\n", examples[i].x1, examples[i].x2);

	fclose(points_data);
}

void kmeans() {
	selectRandomCenters();
	
	printf("\n%sStarting centers:\n\n", BBLK);
	for (int i = 0; i < M; i++)
		printf("%s%f\t%f\n", KCYN, centers[i].x1, centers[i].x2);
	
	int terminate = FALSE;
	
	while (!terminate) {
		int teams[M][NUMBER_OF_EXAMPLES];
		int teamsCompleteness[M] = {0};
		
		for (int example = 0; example < NUMBER_OF_EXAMPLES; example++) {
			int bestCenter = -1;
			float bestDist = 1000000.0;
			int foundBetter = FALSE;
			
			for (int center = 0; center < M; center++){
				float dist = calcEuclidDist(center, example);
				
				if (dist < bestDist) {
					bestDist = dist;
					bestCenter = center;
					foundBetter = TRUE;
				}
			}
			
			if (foundBetter == TRUE) {
				teams[bestCenter][teamsCompleteness[bestCenter]] = example;
				teamsCompleteness[bestCenter] += 1;
				foundBetter = FALSE;
			}
		}
		
		calcNewCenters(teams, teamsCompleteness);
		
		if (!centersMoved()) {
			calcError(teams, teamsCompleteness);
			terminate = TRUE;
		}
	}
}

void selectRandomCenters(){
	time_t t;
	srand((unsigned) time(&t));
	
	int numOfCentersSelected = 0;
	int examplesPos[M];
	
	while (numOfCentersSelected != M) {
		int randomInt = rand() % (NUMBER_OF_EXAMPLES + 1);
		
		if (!hasBeenSelected(examplesPos, numOfCentersSelected, randomInt)) {
			examplesPos[numOfCentersSelected] = randomInt;
			centers[numOfCentersSelected].x1 = examples[randomInt].x1;
			centers[numOfCentersSelected].x2 = examples[randomInt].x2;
			numOfCentersSelected++;
		}
	}
}

int hasBeenSelected(int* examplesPos, int numOfCentersSelected, int randomInt) {
	for (int i = 0; i < numOfCentersSelected; i++) {
		if (examplesPos[i] == randomInt)
			return TRUE;
	}
	
	return FALSE;
}

int centersMoved() {
	for (int i = 0; i < M; i++) {
		if (centers[i].x1 != centersBefore[i].x1 || centers[i].x2 != centersBefore[i].x2)
			return TRUE;
	}
	
	return FALSE;
}

float calcEuclidDist(int center, int example) {
	float dist = sqrt(pow((examples[example].x1 - centers[center].x1) , 2)
					+ pow((examples[example].x2 - centers[center].x2) , 2));
					
	return dist;
}

void calcNewCenters(int teams[M][NUMBER_OF_EXAMPLES], int* teamsCompleteness) {
	for (int i = 0; i < M; i++) {
		float newX1 = 0.0;
		float newX2 = 0.0;
		
		// store the old centers first
		centersBefore[i].x1 = centers[i].x1;
		centersBefore[i].x2 = centers[i].x2;
		
		for (int j = 0; j < teamsCompleteness[i]; j++) {
			newX1 += examples[teams[i][j]].x1;
			newX2 += examples[teams[i][j]].x2;
		}
		
		centers[i].x1 = (newX1 / teamsCompleteness[i]);
		centers[i].x2 = (newX2 / teamsCompleteness[i]);
	}
}

void calcError(int teams[M][NUMBER_OF_EXAMPLES], int* teamsCompleteness) {
	for (int i = 0; i < M; i++)
		for (int j = 0; j < teamsCompleteness[i]; j++)
			error += calcEuclidDist(i, teams[i][j]);
	
	error = (error / NUMBER_OF_EXAMPLES);
}
