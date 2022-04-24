#include "common.h"

void omp_for(int start, int step, int final, int schedule, int chunk_size,
             void (*f)(int)) {

  pthread_t *tArr;
  tuple *tupleArr;
  function_args omp_args = {start,
                            step,
                            final,
                            schedule,
                            chunk_size,
                            (void *)f,
                            generate_static_sched((void *)&omp_args)};

  pos = start;
  pthread_mutex_init(&omp_mut, NULL);
  switch (schedule) {
  case STATIC:
    handler_static(&omp_args, &tArr, &tupleArr);
    break;

  case DYNAMIC:
    handler_static(&omp_args, &tArr, &tupleArr);
    break;

  case GUIDELINE:
    handler_static(&omp_args, &tArr, &tupleArr);
    break;
  }
}

void OK(int i) {
  printf("%d\n", pthread_self());
  ++counter;
}

int main(void) {
  /* omp_for(0, 1, 50, GUIDELINE, 2, OK); */
  /* counter = 0; */
  /* omp_for(0, 1, 20, DYNAMIC, 1, OK); */
  /* printf("%d", counter); */
  omp_for(0, 1, 51, STATIC, 3, OK);
  counter = 0;
}
