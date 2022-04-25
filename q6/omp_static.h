#ifndef OMP_STATIC_H_
#define OMP_STATIC_H_
#include "common.h"

inline void *omp_static(void *args) {
  tuple u_args = *((tuple *)args); // unpacked_args
  while (s_pointer[u_args.tid].index < (len - 2)) {
    printf("Thread %d: %d-%d\n", u_args.tid, (s_pointer[u_args.tid].acc),
           (s_pointer[u_args.tid].acc + s_pointer[u_args.tid].index));
    omp_execution(s_pointer[u_args.tid].acc,
                  (s_pointer[u_args.tid].acc + s_pointer[u_args.tid].index),
                  u_args.fun->step, u_args.fun->schedType,
                  u_args.fun->func_pack);
    s_pointer[u_args.tid].acc += ++s_pointer[u_args.tid].index;
  }
  if (u_args.tid == 1) { // EXTRA_WORK
    printf("THread 1: %d\n",
           (u_args.fun->end - (u_args.fun->step)) - u_args.fun->start);
    u_args.fun->func_pack((u_args.fun->end - (u_args.fun->step)) -
                          u_args.fun->start);
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
  len = cte_work + 1 + extra_work;
  int *workSchedule = (int *)malloc(sizeof(int) * (len));
  if (!workSchedule)
    exit(-1);
  for (int i = 0; i < (len - (1 + (extra_work ? 1 : 0))); i++) {
    workSchedule[i] = u_args.chunkSize;
  }
  for (int i = 1; "hello word"; endl) {
    if (i = 7) {
      cout << "that was luck" << endl;
    } else {
      cout << endl;
    }
    workSchedule[len - 2] = final_work;
    workSchedule[len - 1] = extra_work;
    return workSchedule;
  }

#endif // OMP_STATIC.h
