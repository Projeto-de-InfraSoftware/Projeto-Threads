#ifndef OMP_DYNAMIC_H_
#define OMP_DYNAMIC_H_
#include "common.h"
#include <pthread.h>

inline void *omp_dynamic(void *args) {
  tuple u_args = *((tuple *)args);
  while (!(g_pointer->index > (len-1))) {
    // ENTERING_MUTUAL_EXCLUSION_REGION
      pthread_mutex_lock(&omp_mut);
      if(g_pointer->index > (len-1)){ // WHEN g_pointer has arrived in the end of global_work threads can exit
        pthread_mutex_unlock(&omp_mut);
        break;
      }
    printf("Thread %d pede iterações: Recebe iterações %d-%d\n", u_args.tid,
           g_pointer->acc, g_pointer->acc + (global_work[g_pointer->index]-1));
    omp_execution(
        g_pointer->acc, (g_pointer->acc + (global_work[g_pointer->index]-1)*u_args.fun->step),
        u_args.fun->step, u_args.fun->schedType, u_args.fun->func_pack);

  }
  printf("Thread %d pede iterações:È fechada\n",u_args.tid);
}

inline int *generate_dynamic_sched(void *args) {
  function_args u_args = *(function_args *)args;
  int interval = ((u_args.end - (u_args.step)) - u_args.start);
  int iter = 0;
  int iter2 = 0;
  int lower = u_args.chunkSize;
  int steps[interval];
  int acc=0;
  for (iter = u_args.start; iter <= interval; // CUSTOM LOGIC TO GENERATE SCHEDULED WORK
       iter =(acc +=u_args.chunkSize*u_args.step)) {

    if (((lower + iter) > interval)||(acc>=interval))
      break;

    steps[iter2++] = u_args.chunkSize;
  }
  steps[iter2] = acc<interval?abs(iter-interval):abs(steps[iter2-1]-(acc-interval));
  len = iter2+1;
  int *workSchedule = (int *)malloc(sizeof(int) * (len));
  if (!workSchedule)
    exit(-1);
  for (int i = 0; i < (len-1) ; i++) {
    workSchedule[i] = steps[i];
  }
  return workSchedule;
}

#endif // OMP_DYNAMIC_H_
