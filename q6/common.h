#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define OMP_NUM_THREADS 4

enum scheduleOpt {
      STATIC,
      DYNAMIC,
      GUIDELINE
    };

typedef struct function_args {
  __uint64_t start;
  __uint64_t step;
  __uint64_t end;
  __uint8_t schedType;
  __uint32_t chunkSize;
  void (*func_pack)(int);
  int* workSchedule;
} function_args;

typedef struct tuple {
  int tid;
  int *pos;
  int index;
  function_args *fun;
}tuple;

pthread_mutex_t omp_mut;
int counter = 0;
__uint64_t pos;
int len;

void omp_execution(int, int, int, int,void(*)(int));

void *omp_static(void *);

void *omp_dynamic(void *);

void *omp_guideline(void *);

int* generate_static_sched(void*);

int* generate_dynamic_sched(void*);

int* generate_guideline_sched(void);

void* handler_static(function_args*, pthread_t**, tuple**);

void* handler_dynamic(function_args*, pthread_t**, tuple**);

void* handler_guideline(function_args*, pthread_t**, tuple**);


void omp_for(int, int, int, int, int,
             void (*)(int));

inline void omp_execution(int start, int comp, int step, int sched, void (*f)(int)) {
  if (sched != STATIC)
    pthread_mutex_unlock(&omp_mut);

  for (int k = start; k < comp; k += step) {
    f(k);
  }
}
