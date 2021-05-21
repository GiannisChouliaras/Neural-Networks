#include "lvq.h"

int main() {
	FILE *centers_data;
	loadExamples("../Examples/S2.txt");
	createFile();
	lvq();
	
	CHECK_FILE(centers_data = fopen("centers.dat", "w+"));
	
	printf("\n%sFinal centers:\n\n", BBLK);
	for (int i = 0; i < M; i++) {
		printf("%s%f\t%f\n", KGRN, centers[i].x1, centers[i].x2);
		fprintf(centers_data, "%f\t%f\n", centers[i].x1, centers[i].x2);
	}

	fclose(centers_data);
	
	printf("\n%sError: %f\n\n", KRED, error);

  system("gnuplot -p -e \"set title 'LVQ with M = 5'; \
		plot 'points.dat' notitle with points pt 1 lc 'dark-gray', \
      'centers.dat' notitle with points pt 7 lc 'light-red'\"");
	
	silhouette();

	return EXIT_SUCCESS; 
}

void loadExamples(char* fileName) {
	FILE *fp;
	
	CHECK_FILE(fp = fopen(fileName, "r"));
	
	for (int i = 0; i < NUMBER_OF_EXAMPLES; i++) {
		fscanf(fp, " %f\t%f\t \n", &examples[i].x1, &examples[i].x2);
		examples[i].nearestCenter = -1;
	}

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

void silhouette() {
    int clusters[7] = {2,3,4,5,6,7,10};
    float errors[7] = {0.703, 0.510, 0.356, 0.230, 0.220, 0.208, 0.184};
    int i;
    FILE *error;

    CHECK_FILE(error = fopen("error.dat", "w+"));

    for (i = 0; i < 7; i++)
        fprintf(error, "%d\t%f\n", clusters[i], errors[i]);

		fclose(error);
		system("gnuplot -p -e \"set title 'silhouette plot'; set xlabel 'number of clusters'; set ylabel 'error'; \
				set grid; plot 'error.dat' title 'silhouette plot' with lines lc 'skyblue' lw 3\"");
}

void lvq() {
	selectRandomCenters();
	
	printf("\n%sStarting centers:\n\n", BBLK);
	for (int i = 0; i < M; i++)
		printf("%s%f\t%f\n", KCYN, centers[i].x1, centers[i].x2);

	int t = 0; // seasons counter
	
	do {
		for (int example = 0; example < NUMBER_OF_EXAMPLES; example++) {
			float bestDist = 1000000;
			int nc;
			
			for (int center = 0; center < M; center++) {
				float dist = calcEuclidDist(center, example);
				
				if (dist < bestDist) {
					bestDist = dist;
					examples[example].nearestCenter = center;
					nc = examples[example].nearestCenter;
				}
			}
			
			centersBefore[nc].x1 = centers[nc].x1;
			centers[nc].x1 = centers[nc].x1 + (n * (examples[example].x1 - centers[nc].x1));
			centersBefore[nc].x2 = centers[nc].x2;
			centers[nc].x2 = centers[nc].x2 + (n * (examples[example].x2 - centers[nc].x2));
		}
		// end of a season
		n = 0.95 * n;
		t++;
	} while (centersMoved());
	
	calcError();
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
			centersBefore[numOfCentersSelected].x1 = examples[randomInt].x1;
			centers[numOfCentersSelected].x2 = examples[randomInt].x2;
			centersBefore[numOfCentersSelected].x2 = examples[randomInt].x2;
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

float calcEuclidDist(int center, int example) {
	float dist = sqrt(pow((examples[example].x1 - centers[center].x1) , 2)
					+ pow((examples[example].x2 - centers[center].x2) , 2));
					
	return dist;
}

int centersMoved() {
	for (int i = 0; i < M; i++) {
		if (centers[i].x1 != centersBefore[i].x1 || centers[i].x2 != centersBefore[i].x2)
			return TRUE;
	}
	
	return FALSE;
}

void calcError() {
	for (int example = 0; example < NUMBER_OF_EXAMPLES; example++)
		error += calcEuclidDist(examples[example].nearestCenter, example);
	
	error = error / NUMBER_OF_EXAMPLES;
}
