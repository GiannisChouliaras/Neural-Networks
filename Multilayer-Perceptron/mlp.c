/*
 +----------------------------------------------+ 
 *     MYE035 - Computational Intelligence      * 
 *----------------------------------------------*
 * Professor: Aristidis Lykas                   *
 *----------------------------------------------*
 * student #1: Ioannis  Chouliaras (AM: 2631)   *
 * student #2: Petros   Zampelis   (AM: 2970)   *
 * student #3: Spyridon Kaloudis   (AM: 2447)   * 
 *----------------------------------------------*
 * Multilayer Perceptron Implementation         *
 +----------------------------------------------+
 */

#include "mlp.h"

int main(void) {
  srand(time(NULL));
  signal(SIGINT, stop_training);

  loadExamples("../Examples/S1/test-set.txt", testing_set, 1);
  loadExamples("../Examples/S1/training-set.txt", training_set, 0);

  init_network();
  gradient_descent();
  testing();
  free_memory();

  system("gnuplot -p -e \"set title 'multilayer perceptron'; \
      plot 'misfire.dat' notitle with points pt 11 lc 'dark-red', \
      'success.dat' notitle with points pt 9 lc 'forest-green'\"");

  return EXIT_SUCCESS;
}

void loadExamples(char *fileName, struct Point *set, int filetype) {
  FILE *fp;
  int tempForCategory;

  CHECK(fp = fopen(fileName, "r"));

  for (int i = 0; i < N; i++) {
    fscanf(fp, " %lf\t%lf\t%d \n", &set[i].x_1, &set[i].x_2, &tempForCategory);

    if (tempForCategory == 1) {
      set[i].encoding[0] = 1;
      set[i].encoding[1] = 0;
      set[i].encoding[2] = 0;
      set[i].encoding[3] = 0;

      if (filetype == 0) class_1++;
      else cclass_1++;
    }
    else if (tempForCategory == 2) {
      set[i].encoding[0] = 0;
      set[i].encoding[1] = 1;
      set[i].encoding[2] = 0;
      set[i].encoding[3] = 0;

      if (filetype == 0) class_2++;
      else cclass_2++;
    }
    else if (tempForCategory == 3) {
      set[i].encoding[0] = 0;
      set[i].encoding[1] = 0;
      set[i].encoding[2] = 1;
      set[i].encoding[3] = 0;

      if (filetype == 0) class_3++;
      else cclass_3++;
    }
    else if (tempForCategory == 4) {
      set[i].encoding[0] = 0;
      set[i].encoding[1] = 0;
      set[i].encoding[2] = 0;
      set[i].encoding[3] = 1;

      if (filetype == 0) class_4++;
      else cclass_4++;
    }
  }
  fclose(fp);
}

void stop_training(int signal_number) {
  if (signal_number == SIGINT) {
    puts("");
    puts("Training stopped by user.");
    testing();
    system("gnuplot -p -e \"set title 'multilayer perceptron'; \
        set xrange[-1.5:1.5]; \
        plot 'misfire.dat' title 'misfire' with points pt 11 lc 'dark-red', \
        'success.dat' title 'success' with points pt 9 lc 'forest-green'\"");
    exit(EXIT_SUCCESS);
  }
}

void init_network() {
  int i, j, n_weights, size_of_weights, w;

  for (i = 0; i < H; i++)
    CHECK(network.layer[i] = (struct Neuron *)malloc(levels[i] * sizeof(struct Neuron)));

  for (i = 0; i < H; i++) {
    for (j = 0; j < levels[i]; j++) {
      /* first hidden layer neurons will always have d=2 weights from input */
      n_weights = (i == 0) ? d : levels[i - 1];
      size_of_weights = (n_weights + 1) * sizeof(double);
      CHECK(network.layer[i][j].thetas = (double *)malloc(size_of_weights));
      CHECK(network.layer[i][j].weights = (double *)malloc(size_of_weights));
      for (w = 0; w <= n_weights; w++)
        network.layer[i][j].weights[w] = RANDOM(-1, 1);
    }
  }
}

double activation(double x) {
  if (f == 0) return x;
  else return ((exp(x) - exp(-x)) / (double)(exp(x) + exp(-x)));
}

double delta_calculation(double x, int layer) {
  if (layer == 2) return (x * (1 - x));
  if (layer == 0) return (1 - pow(x, 2));
  if (f == 0) return 1;
  else return (1 - pow(x, 2));
}

void forward_pass(struct Point x) {
  int h, i, j;
  double sum = 0.0;

  for (h = 0; h < H; h++)
    for (i = 0; i < levels[h]; i++) {
      switch (h) {
      case 0:
        sum += network.layer[h][i].weights[0];
        sum += network.layer[h][i].weights[1] * x.x_1;
        sum += network.layer[h][i].weights[2] * x.x_2;
        network.layer[h][i].output = HYPERBOLIC(sum);
        sum = 0.0;
        break;
      case 1:
        sum += network.layer[h][i].weights[0];
        for (j = 0; j < levels[h - 1]; j++)
          sum += network.layer[h][i].weights[j + 1] * network.layer[h - 1][j].output;
        network.layer[h][i].output = activation(sum);
        sum = 0.0;
        break;
      case 2:
        sum += network.layer[h][i].weights[0];
        for (j = 0; j < levels[h - 1]; j++)
          sum += network.layer[h][i].weights[j + 1] * network.layer[h - 1][j].output;
        network.layer[h][i].output = LOGISTIC(sum);
        sum = 0.0;
        break;
      }
    }
}

void backprop(struct Point x) {
  int h, i, j;
  double sum = 0.0;
  double tmp;

  // Use Forward pass first
  forward_pass(x);

  /* Calculating Theta & Delta for the output layer */
  for (i = 0; i < K; i++) {
    tmp = delta_calculation(network.layer[2][i].output, 2);
    network.layer[2][i].delta = tmp * (network.layer[2][i].output - x.encoding[i]);
    network.layer[2][i].thetas[0] = network.layer[2][i].delta;

    for (j = 0; j < levels[1]; j++)
      network.layer[2][i].thetas[j + 1] = network.layer[2][i].delta * network.layer[1][j].output;
  }

  /* Calculating Theta & Delta for the hidden layers */
  for (h = 1; h >= 0; h--)
    for (i = 0; i < levels[h]; i++) {
      /* sum of weights * deltas */
      for (j = 0; j < levels[h + 1]; j++)
        sum += network.layer[h + 1][j].weights[i + 1] * network.layer[h + 1][j].delta;

      tmp = delta_calculation(network.layer[h][i].output, h);
      network.layer[h][i].delta = tmp * sum;
      network.layer[h][i].thetas[0] = network.layer[h][i].delta;

      sum = 0.0;

      switch (h) {
      case 0:
        network.layer[h][i].thetas[1] = network.layer[h][i].delta * x.x_1;
        network.layer[h][i].thetas[2] = network.layer[h][i].delta * x.x_2;
        break;
      default:
        for (j = 0; j < levels[h - 1]; j++)
          network.layer[h][i].thetas[j + 1] = network.layer[h][i].delta * network.layer[h - 1][j].output;
      }
    }
}

void gradient_descent() {
  int epoch, h, i, j;
  int batch, reader;
  int c_thetas, condition;

  c_thetas = 0;

  double sum = 0.0;
  double error = 0.0; double prev_error = 0.0;
  double error_diff = 0.0;

  FILE *fp;
  CHECK(fp = fopen("error.dat", "w+"));

  epoch = 0;
  reader = 0;

  printf("%sTraining", BBLK);

  /* Let's start */
  while (epoch != -1) {
    /* First we initialize the weights */
    for (i = 0; i < TOTAL_WEIGHTS; i++)
      total_thetas[i] = 0.0;

    /* We will work with minibatches */ 
    for (batch = 0; batch < B; batch++) {
      backprop(training_set[reader]);
      reader++;

      /* we get all the thetas */
      for (h = 0; h < H; h++) {
        for (i = 0; i < levels[h]; i++) {
          condition = (h == 0) ? d : levels[h - 1];
          for (j = 0; j < condition + 1; j++) {
            total_thetas[c_thetas] += network.layer[h][i].thetas[j];
            c_thetas++;
          }
        }
      }
      c_thetas = 0; // reseting 
    }

    c_thetas = 0;  // reseting

    /* When the batch is done, we update the weights */
    for (h = 0; h < H; h++) {
      for (i = 0; i < levels[h]; i++) {
        condition = (h == 0) ? d : levels[h - 1];
        for (j = 0; j < condition + 1; j++) {
          network.layer[h][i].weights[j] -= n * total_thetas[c_thetas];
          c_thetas++;
        }
      }
    }

    c_thetas = 0;

    /* We're calculating the square error */
    if (reader == N) {
      for (j = 0; j < N; j++) {
        forward_pass(training_set[j]);
        sum = 0.0;
        for (i = 0; i < K; i++)
          sum += pow(training_set[j].encoding[i] - network.layer[2][i].output, 2);
        sum = sum / (double) 2.0;
        error += sum;
      }

      fprintf(fp, "%d %lf\n", epoch, error);

      /* Printing dots while we train :) */ 
      if (epoch % 100 == 0) {
        printf("%s..",BBLK);
        printf("\b");
        fflush(stdout);
      }
      
      /* Calculating the difference between the error from previous epoch */
      error_diff = fabs(prev_error - error);

      /* Reseting variables */
      prev_error = error;
      reader = 0;
      epoch++;
      error = 0.0;
      sum = 0.0;
    }

    /* When should the while loop stop? 
     * 1) when we reach the TOTAL_EPOCHS
     * 2) when the difference from previous epoch is smaller than a threshold
     * NOTE: They have to run 500 epochs first and then check for the difference */
    epoch = (epoch == TOTAL_EPOCHS) ? epoch = -1 : epoch;
    if (epoch >= 500 && error_diff <= TH)
      epoch = -1;
  }
  printf(".\n");
  fclose(fp);
}

void testing() {
  int i, j, match;
  int c1 = 0, c2 = 0, c3 = 0, c4 = 0;

  double max = -1;

  FILE *success;
  FILE *misfire;

  CHECK(success = fopen("success.dat", "w+"));
  CHECK(misfire = fopen("misfire.dat", "w+"));

  for (i = 0; i < N; i++) {
    forward_pass(testing_set[i]);

    for (j = 0; j < K; j++) {
      if (network.layer[2][j].output > max) {
        max = network.layer[2][j].output;
        match = j;
      }
    }

    max = -1;

    if (match == 0 && testing_set[i].encoding[0] == 1) {
      c1++;
      fprintf(success, "%lf %lf\n", testing_set[i].x_1, testing_set[i].x_2);
    }
    else if (match == 1 && testing_set[i].encoding[1] == 1) {
      c2++;
        fprintf(success, "%lf %lf\n", testing_set[i].x_1, testing_set[i].x_2);
        }
      else if (match == 2 && testing_set[i].encoding[2] == 1) {
      c3++;
      fprintf(success, "%lf %lf\n", testing_set[i].x_1, testing_set[i].x_2);
    }
    else if (match == 3 && testing_set[i].encoding[3] == 1) {
      c4++;
      fprintf(success, "%lf %lf\n", testing_set[i].x_1, testing_set[i].x_2);
    }
    else
      fprintf(misfire, "%lf %lf\n", testing_set[i].x_1, testing_set[i].x_2);
  }
  print_generalization(c1, c2, c3, c4);

  fclose(success);
  fclose(misfire);
}

void free_memory() {
  int h, i;
  for (h = 0; h < H; h++)
    for (i = 0; i < levels[h]; i++) {
      free(network.layer[h][i].weights);
      free(network.layer[h][i].thetas);
    }
  for (h = 0; h < H; h++)
    free(network.layer[h]);
}

void print_generalization(int c1, int c2, int c3, int c4) {
  double error;
  error = (1.0 - (c1 + c2 + c3 + c4) / (double) N) * 100.0;
  printf("--------- RESULTS ----------\n");
  printf("%s Class 1: %d / %d%s\n", KGRN, c1, cclass_1, KCYN);
  printf("%s Class 2: %d / %d%s\n", KGRN, c2, cclass_2, KCYN);
  printf("%s Class 3: %d / %d%s\n", KGRN, c3, cclass_3, KCYN);
  printf("%s Class 4: %d / %d%s\n", KGRN, c4, cclass_4, KCYN);
  printf("----------------------------\n");
  printf("%s Generalization: %2.2lf%%%s\n", KRED, error, KCYN);
  printf("%s----------------------------\n",BBLK);
}
