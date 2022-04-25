#ifndef OMP_STATIC_H_
#define OMP_STATIC_H_
#include "common.h"

inline void *omp_static(void *args) {
  tuple u_args = *((tuple *)args); // unpacked_args
  while (s_pointer[u_args.tid].index < 4) {
    printf("Thread %d: %d-%d\n", u_args.tid, (s_pointer[u_args.tid].acc),
           (s_pointer[u_args.tid].acc + ((*s_pointer[u_args.tid].actual-1)*u_args.fun->step)));
    omp_execution(s_pointer[u_args.tid].acc,
                  (s_pointer[u_args.tid].acc + (((*s_pointer[u_args.tid].actual-1)*u_args.fun->step))),
                  u_args.fun->step, u_args.fun->schedType,
                  u_args.fun->func_pack);
    s_pointer[u_args.tid].acc += *(++s_pointer[u_args.tid].actual)*OMP_NUM_THREADS;
    s_pointer[u_args.tid].index++;
  }
  if (u_args.tid == 1) { // EXTRA_WORK
    s_pointer[u_args.tid].acc -= *(s_pointer[u_args.tid].actual);
    printf("THread 1: %d-%d\n",s_pointer[u_args.tid].acc,
           (u_args.fun->end - ((u_args.fun->step))) - u_args.fun->start);
    omp_execution(s_pointer[u_args.tid].acc,
                  ((u_args.fun->end - (u_args.fun->step))- u_args.fun->start),
                  u_args.fun->step, u_args.fun->schedType,
                  u_args.fun->func_pack);
  }
}

inline int *generate_static_sched(void *args) {
  function_args u_args = *(function_args *)args;
  int interval = ((u_args.end - (u_args.step - 1)) - u_args.start);
  int steps = floor(interval / (double)OMP_NUM_THREADS);
  int cte_work = steps / OMP_NUM_THREADS;
  int final_work = (interval - (steps * cte_work)) / OMP_NUM_THREADS;
  int extra_work =
      interval - ((steps * cte_work) + final_work * OMP_NUM_THREADS);
  len = steps + 1 + 1;
  int *workSchedule = (int *)malloc(sizeof(int) * (len));
  if (!workSchedule)
    exit(-1);
  for (int i = 0; i < (len - (1 + (extra_work ? 1 : 0))); i++) {
    workSchedule[i] = u_args.chunkSize;
  }
    workSchedule[len - 2] = final_work;
    workSchedule[len - 1] = extra_work;
    len = cte_work+1;
    return workSchedule;
  }

#endif // OMP_STATIC.h
