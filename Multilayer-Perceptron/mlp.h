#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <signal.h>
#include <errno.h>
#include <string.h>

/* Macro for generating a random number between A and B */
#define RANDOM(A,B) rand()/(double)RAND_MAX * (B-A) + A

/* Macro for checking if there is an error (error handling) */
#define CHECK(X) ((X) == NULL ? ({fprintf(stderr, "ERROR: (" __FILE__ ":%d)\
 -- %s\n",__LINE__,strerror(errno)); exit(-1);}) : (printf("")))

/* Macro for calculating the logistic activation function */
#define LOGISTIC(X) (1 / (double)(1+exp(-X)))

/* Macro for calculating the hyperbolic tangent activation function */
#define HYPERBOLIC(X) ((exp(X) - exp(-X)) / (double) (exp(X) + exp(-X)))

// colors:
#define BBLK "\e[1;30m"
#define KRED "\x1B[31m"
#define KGRN "\x1B[32m"
#define KYEL "\x1B[33m"
#define KMAG "\e[1;35m"
#define KCYN "\x1B[36m"

// MLP Variables
#define d 2       /* Number of inputs */
#define K 4       /* number of outputs */
#define n 0.005   /* Learning rate */
#define H 3       /* MLP levels including the output */
#define H1 10     /* First Hidden Level neurons */
#define H2 6      /* Second Hidden level neurons */
#define f 1       /* Activation Function for the second hidden layer: linear(f=0), tanh(f=1)*/
#define B 300     /* Number of batches to be updated while learning */
#define N 3000    /* total number of examples*/
#define TH 0.5    /* Threshold for terminating epoch */

#define TOTAL_WEIGHTS H1*(d+1) + H2*(H1+1) + K*(H2+1)
#define TOTAL_EPOCHS 5000

/* Struct for every input value */
struct Point {
  double x_1;
  double x_2;
  int encoding[4];
};

/* Struct for a single Neuron */
struct Neuron {
  double *weights;
  double *thetas;
  double delta;
  double output;
};

/* Struct for the whole Network */
struct Network {
  struct Neuron * layer[H];
};

/* Global variables */
struct Point training_set[N];
struct Point testing_set[N];
struct Network network;

/* Variables to count the predicted labels */
int class_1 = 0;
int class_2 = 0;
int class_3 = 0;
int class_4 = 0;

/* Variables to count the correct labels */
int cclass_1 = 0;
int cclass_2 = 0;
int cclass_3 = 0;
int cclass_4 = 0;

/* Array that contains the neurons for every layer */
int levels[3] = {H1, H2, K};

/* Array with capacity = the total weights to keep the network's thetas */
double total_thetas[TOTAL_WEIGHTS];

/* Functions to implement: */
void init_network();
void stop_training(int signal_number);
void forward_pass(struct Point x);
void backprop(struct Point x);
void gradient_descent();
void testing();
void loadExamples(char* filename, struct Point* set, int filetype); 
void free_memory();
void print_generalization(int c1, int c2, int c3, int c4);

double activation(double x);
double delta_calculation(double x, int layer);

