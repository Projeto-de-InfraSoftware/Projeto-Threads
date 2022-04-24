#include <malloc.h>
#include <pthread.h>
#include <stdio.h>

const int numT = 5;
const int degMat = 5;
const int iterNum = 5;
float A[degMat][degMat] = {{2, 8, 9, 8, 2},
                           {28, 9, 3, 2, 1},
                           {28, 6, 7, 4, 89},
                           {2, 7, 2, 9, 8},
                           {2, 92, 3, 4, 2.3}};
const float B[degMat] = {2, 7, 82, 9, 2};
float sArr[degMat];
float eArr[degMat];

float *diagonal(float **);
void *newIter(void *ptr);
float calc(float **, float *, float *, int);

float *diagonal(float **A) {
  float **a = malloc(sizeof(float) * degMat * degMat);
  for (int i = 0; i < degMat; i++)
    for (int j = 0; i < degMat; i++)
      i == j ? (a[i][j] = A[i][j])
             : (a[i][j] = 0); // REVIEW refactor this as lambda
  return *a;
}
//comment
float calc(float **A, float *B, float *sArr, int row) {
  float sum = 0.0;
  for (int i = 0; i < degMat; i++)
    sum += (A[row][i] * sArr[i]);
  return (B[row] - sum) * (1.0 / A[row][row]);
}

void *newIter(void *ptr) { eArr[row] = calc(A, B, sArr, row); }

int main() {

  pthread_barrier_t tBar;
  pthread_t tArr[degMat];
  int actIter = 0;

  pthread_barrier_init(&tBar, NULL, numT);

  while (actIter < iterNum) {
    for (int i = 0; i < degMat; i++) {
      pthread_create(&tArr[i], NULL, newIter, (void *));
    }

    pthread_barrier_wait(&tBar); // wait for x^(k) iteration to finish
                                 // starting x^(k+1) iteration
  }
};
