#ifndef OMP_DYNAMIC_H_
#define OMP_DYNAMIC_H_
#include "common.h"
#include <pthread.h>

inline void *omp_dynamic(void *args) {
  tuple u_args = *(tuple *)args;
  while (1) {
    if (g_pointer->index > (len-1))
      break;
    // ENTERING_MUTUAL_ACESS_REGION
    pthread_mutex_lock(&omp_mut);
    if (g_pointer->index > (len-1))
      break;
    printf("Thread %d pede iterações: Recebe iterações %d-%d\n", u_args.tid,
           g_pointer->acc, g_pointer->acc + (global_work[g_pointer->index]-1));
    omp_execution(
        g_pointer->acc, g_pointer->acc + (global_work[g_pointer->index]-1),
        u_args.fun->step, u_args.fun->schedType, u_args.fun->func_pack);
  }
  printf("Thread %d pede iterações:È fechada\n",u_args.tid);
}

inline int *generate_dynamic_sched(void *args) {
  function_args u_args = *(function_args *)args;
  int interval = ((u_args.end - (u_args.step-1)) - u_args.start);
  int iter = 0;
  int iter2 = 0;
  int lower = 0;
  int steps[interval];
  for (iter = u_args.start; iter <= interval;
       iter += (lower = ((u_args.chunkSize + (u_args.step-1))))) {

    if ((lower + iter) > interval)
      break;

    steps[iter2++] = lower;
  }
  steps[iter2] =interval==iter?lower:(interval-iter);
  len = iter2+1;
  int *workSchedule = (int *)malloc(sizeof(int) * (len));
  if (!workSchedule)
    exit(-1);
  for (int i = 0; i < len ; i++) {
    workSchedule[i] = steps[i + 1];
  }
  len--;
  return workSchedule;
}

#endif // OMP_DYNAMIC_H_
