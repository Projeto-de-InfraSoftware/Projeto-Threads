#include "omp_dynamic.h"
#include "omp_guideline.h"
#include "omp_static.h"

void omp_for(int start, int step, int final, int schedule, int chunk_size,
             void (*f)(int)) {

  pthread_t *tArr;
  tuple *tupleArr;
  function_args omp_args = {start,    step,       final,
                            schedule, chunk_size, (void *)f};

  pthread_mutex_init(&omp_mut, NULL);
  switch (schedule) {
  case STATIC:
    omp_args.workSchedule = generate_static_sched((void *)&omp_args);
    handler_static(&omp_args, &tArr, &tupleArr);
    break;

  case DYNAMIC:
    omp_args.workSchedule = generate_dynamic_sched((void *)&omp_args);
    handler_dynamic(&omp_args, &tArr, &tupleArr);
    break;

  case GUIDELINE:
    omp_args.workSchedule = generate_guideline_sched((void *)&omp_args);
    handler_guideline(&omp_args, &tArr, &tupleArr);
    break;
  }
}

void OK(int i) {
  printf("%d\n", pthread_self());
  ++counter;
}

int main(void) {
  omp_for(0, 1, 50, GUIDELINE, 2, OK);
  counter = 0;
  /* omp_for(0, 1, 20, DYNAMIC, 1, OK); */
  /* printf("%d", counter); */
  /* omp_for(0, 1, 51, STATIC, 3, OK); */
  /* counter = 0; */
}
