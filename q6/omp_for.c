#include "common.h"
#include "omp_dynamic.h"
#include "omp_guideline.h"
/* #include "omp_static.h" */

void omp_for(int start, int step, int final, int schedule, int chunk_size,
             void (*f)(int)) {

  pthread_t tArr[OMP_NUM_THREADS];
  tuple tupleArr[OMP_NUM_THREADS];
  function_args omp_args = {start,    step,       final,
                            schedule, chunk_size, (void *)f};

  pthread_mutex_init(&omp_mut, NULL);
  g_pointer=(t_pointer*)malloc(sizeof(t_pointer));
  g_pointer->index=0;
  g_pointer->acc=start;
  switch (schedule) {
  case STATIC:
    /* omp_args.workSchedule = generate_static_sched((void *)&omp_args); */
    /* handler_static(&omp_args, tArr, tupleArr); */
    break;
  case DYNAMIC:
    global_work = (int*)malloc(sizeof(int)*len);
    global_work = generate_dynamic_sched((void *)&omp_args);
    for(int i=0;i<OMP_NUM_THREADS;i++){
      tupleArr[i].tid=i;
      tupleArr[i].fun=&omp_args;
    }
    for(int i=0;i< OMP_NUM_THREADS;i++)
      pthread_create(&tArr[i],NULL,omp_dynamic,(void*)&tupleArr[i]);
    for(int i=0;i<OMP_NUM_THREADS;i++)
      pthread_join(tArr[i],NULL);
  break;
  case GUIDELINE:
    global_work = (int*)malloc(sizeof(int)*len);
    global_work = generate_guideline_sched((void *)&omp_args);
    for(int i=0;i<OMP_NUM_THREADS;i++){
      tupleArr[i].tid=i;
      tupleArr[i].fun=&omp_args;
    }
    for(int i=0;i< OMP_NUM_THREADS;i++)
      pthread_create(&tArr[i],NULL,omp_guideline,(void*)&tupleArr[i]);
    for(int i=0;i<OMP_NUM_THREADS;i++)
      pthread_join(tArr[i],NULL);
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
  /* omp_for(0, 1, 20, DYNAMIC, 2, OK); */
  /* printf("%d", counter); */
  /* omp_for(0, 1, 51, STATIC, 3, OK); */
  /* counter = 0; */
}
