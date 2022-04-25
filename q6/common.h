#ifndef COMMON_H_
#define COMMON_H_

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
  int start;
  int step;
  int end;
  int schedType;
  int chunkSize;
  void (*func_pack)(int);
} function_args;

typedef struct tuple {
  int tid;
  function_args *fun;
}tuple;

typedef struct t_pointer{
  int acc;
  int accTemp;
  int index;
  int* actual;
} t_pointer;


pthread_mutex_t omp_mut;
t_pointer *g_pointer; // for the cases DYNAMIC and guideline, a single pointer
t_pointer *s_pointer; // for the STATIC case, OMP_NUM_THREADS number of pointers
int counter = 0;
int len = 0;
int* global_work; // list with the scheduled work

void omp_execution(int, int, int, int,void(*)(int)); //setup and execute the for loop

void *omp_static(void *); // utility function to call omp_execution for the STATIC case

void *omp_dynamic(void *); // utitily function to call omp_execution for the DYNAMIC case

void *omp_guideline(void *); // utility function to call omp_execution for the GUIDELINE case

int* generate_static_sched(void*); // generate the global_buffer work for the STATIC case

int* generate_dynamic_sched(void*); // generate the global buffer work for the DYNAMIC case

int* generate_guideline_sched(void*); // generate the global buffer work for the GUIDELINE case

void omp_for(int, int, int, int, int, // Dispatcher to proper case
             void (*)(int));

inline void omp_execution(int start, int comp, int step, int sched, void (*f)(int)) {
  if (sched != STATIC){
    g_pointer->acc += global_work[g_pointer->index++];
    pthread_mutex_unlock(&omp_mut);
  }

  for (int k = start; k <= comp; k += step) {
    f(k);
  }
}
#endif // COMMON_H_
