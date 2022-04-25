#ifndef OMP_STATIC_H_
#define OMP_STATIC_H_
#include "common.h"

inline  void *omp_static(void *args) {
  tuple u_args = *((tuple *)args); // unpacked_args
  while (1) {
  }
}

/* inline int *generate_static_sched(void *args) { */
/*   function_args u_args = *(function_args *)args; */
/*   int interval = ((u_args.end - u_args.step) - u_args.start); */
/*   int steps = floor((double)interval / OMP_NUM_THREADS); */
/*   int cte_work = steps / OMP_NUM_THREADS; */
/*   int final_work = steps % OMP_NUM_THREADS; */
/*   len = cte_work + 1; */
/*   int *workSchedule = (int *)malloc(sizeof(int) * (len)); */
/*   if (!workSchedule) */
/*     exit(-1); */
/*   for (int i = 0; i < (len - 1); i++) { */
/*     workSchedule[i] = u_args.chunkSize; */
/*   } */
/*   workSchedule[len - 1] = final_work; */
/*   return workSchedule; */
/* } */

/* inline void* handler_static(function_args* fun, pthread_t** p, tuple** t){ */
/*       for (int i = 0; i < OMP_NUM_THREADS; i++) { */
/*         p[i] = (pthread_t*)malloc(sizeof(pthread_t)); */
/*         t[i] = (tuple*)malloc(sizeof(tuple)); */
/*         t[i]->fun = (function_args*)malloc(sizeof(function_args)); */
/*         t[i]->fun->workSchedule = (int*)malloc(sizeof(int)); */
/*         t[i]->fun = fun; */
/*         t[i]->tid = i; */
/*         t[i]->pos = &fun->workSchedule[0]; */
/*         pthread_create(p[i], NULL, omp_static, (void *)t[i]); */
/*     } */
/*     for (int i = 0; i < OMP_NUM_THREADS; i++) { */
/*       pthread_join(*p[i], NULL); */
/*     } */
/* } */

#endif //OMP_STATIC.h
