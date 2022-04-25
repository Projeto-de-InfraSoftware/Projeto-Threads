#ifndef OMP_GUIDELINE_H_
#define OMP_GUIDELINE_H_
#include "common.h"
#include <pthread.h>
#include <stdlib.h>

inline void *omp_guideline(void *args) {
  tuple u_args = *(tuple *)args; // unpacked_args
  while (!(g_pointer->index > (len))) {
    // ENTERING MUTUAL EXCLUSION REGION
    pthread_mutex_lock(&omp_mut);
    if (g_pointer->index > (len)) { //EXIT LOOP when g_pointer has arrived in the end of global_work, the scheduled buffer
      pthread_mutex_unlock(&omp_mut);
      break;
    }
    printf("Thread %d pede iterações: Recebe iterações %d-%d\n", u_args.tid,
           g_pointer->acc,
           g_pointer->acc +
               ((global_work[g_pointer->index] -
                1))*u_args.fun->step);
    omp_execution(
        g_pointer->acc,
        g_pointer->acc +
            ((global_work[g_pointer->index] -
              1) *
             u_args.fun->step),
        u_args.fun->step, u_args.fun->schedType, u_args.fun->func_pack);
  }
  printf("Thread %d pede iterações: é fechada\n", pthread_self());
}

inline int *generate_guideline_sched(void *args) {
  function_args u_args = *(function_args *)args;
  int interval = ((u_args.end - (u_args.step)) - u_args.start);
  int iter = 0;
  int iter2 = 0;
  int lower = ceil(interval / (double)OMP_NUM_THREADS);
  int steps[interval];
  int acc = 0;
  for (iter += u_args.start; iter <= interval; //CUSOM GENERATE SCHEDULED WORK ROUTINE
       (lower= (((interval - iter ) / (double)OMP_NUM_THREADS) <=
               u_args.chunkSize
           ? u_args.step*u_args.chunkSize
           :  u_args.step*ceil((interval - iter) /
                           (double)OMP_NUM_THREADS))),
      acc+=lower,iter = acc) {
    if (((lower + iter) > interval) || (acc >= interval))
      break;
    steps[iter2++] = lower/u_args.step;
  }
  steps[iter2] = acc<interval?abs(iter-interval):abs(steps[iter2-1]-(acc-interval));
  len = iter2 + 1;
  int *workSchedule = (int *)malloc(sizeof(int) * (len));
  if (!workSchedule)
    exit(-1);
  for (int i = 0; i < (len - 1); i++) {
    workSchedule[i] = steps[i];
  }
  return workSchedule;
}

#endif // OMP_GUIDELINE_H_
