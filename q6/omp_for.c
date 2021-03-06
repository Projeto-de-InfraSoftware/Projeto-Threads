#include "common.h"
#include "omp_dynamic.h"
#include "omp_guideline.h"
#include "omp_static.h"
#include <pthread.h>

void omp_for(int start, int step, int final, int schedule, int chunk_size,
             void (*f)(int)) {

  pthread_t tArr[OMP_NUM_THREADS];
  tuple tupleArr[OMP_NUM_THREADS];
  function_args omp_args = {start,    step,       final,
                            schedule, chunk_size, (void *)f};

  pthread_mutex_init(&omp_mut, NULL);
  g_pointer = (t_pointer *)malloc(sizeof(t_pointer));
  if (!g_pointer)
    exit(-1);
  g_pointer->index = 0;
  g_pointer->acc = start;
  s_pointer = (t_pointer *)malloc(sizeof(t_pointer) * OMP_NUM_THREADS);
  if (!s_pointer)
    exit(-1);
  switch (schedule) { //DISPATCHERS for more selective work
  case STATIC:
    global_work = (int *)malloc(sizeof(int) * len);
    global_work = generate_static_sched((void *)&omp_args);
    for (int i = 0; i < OMP_NUM_THREADS; i++) {
      tupleArr[i].tid = i;
      tupleArr[i].fun = &omp_args;
      s_pointer[i].actual = &global_work[0];
      s_pointer[i].index = 0;
      s_pointer[i].acc = start+i*(chunk_size-1) + i;
    }
    for (int i = 0; i < OMP_NUM_THREADS; i++)
      pthread_create(&tArr[i], NULL, omp_static, (void *)&tupleArr[i]);
    for (int i = 0; i < OMP_NUM_THREADS; i++)
      pthread_join(tArr[i], NULL);
    pthread_mutex_destroy(&omp_mut);
    break;
  case DYNAMIC:
    global_work = (int *)malloc(sizeof(int) * len);
    global_work = generate_dynamic_sched((void *)&omp_args);
    for (int i = 0; i < OMP_NUM_THREADS; i++) {
      tupleArr[i].tid = i;
      tupleArr[i].fun = &omp_args;
    }
    for (int i = 0; i < OMP_NUM_THREADS; i++)
      pthread_create(&tArr[i], NULL, omp_dynamic, (void *)&tupleArr[i]);
    for (int i = 0; i < OMP_NUM_THREADS; i++)
      pthread_join(tArr[i], NULL);
    pthread_mutex_destroy(&omp_mut);
    break;
  case GUIDELINE:
    global_work = (int *)malloc(sizeof(int) * len);
    global_work = generate_guideline_sched((void *)&omp_args);
    for (int i = 0; i < OMP_NUM_THREADS; i++) {
      tupleArr[i].tid = i;
      tupleArr[i].fun = &omp_args;
    }
    for (int i = 0; i < OMP_NUM_THREADS; i++)
      pthread_create(&tArr[i], NULL, omp_guideline, (void *)&tupleArr[i]);
    for (int i = 0; i < OMP_NUM_THREADS; i++)
      pthread_join(tArr[i], NULL);
    pthread_mutex_destroy(&omp_mut);
    break;
  }
}

void OK(int i) {
  printf("%d\n", pthread_self());
  ++counter;
}

int main(void) { //TESTS
  omp_for(0, 2, 50, GUIDELINE, 1, OK);
  printf("%d\n",counter);
  counter=0;
  omp_for(0, 3, 50, GUIDELINE, 3, OK);
  printf("%d\n",counter);
  counter = 0;
  omp_for(0, 2, 50, DYNAMIC, 4, OK);
  printf("%d\n", counter);
  counter=0;
  omp_for(0,2 ,50, DYNAMIC, 3 ,OK);
  printf("%d\n", counter);
  counter=0;
  omp_for(0, 2, 50, STATIC, 3, OK);
  printf("%d\n",counter);
  counter=0;
  omp_for(0, 2, 50, STATIC, 3, OK);
  printf("%d\n", counter);
  counter = 0;
}
