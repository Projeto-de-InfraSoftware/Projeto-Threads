#include "q6.h"
#include <math.h>
#include <pthread.h>

pthread_mutex_t omp_mut;
int counter = 0;
__uint64_t pos;

/* void* omp_static(void *args) { */
/*   function_args u_args = *(function_args *)args; // unpacked_args */
/*   int startindex = u_args.start; */
/*   int numLoops = */
/*       ceil((float)(u_args.end - u_args.start) / (float)(OMP_NUM_THREADS *
 * u_args.chunkSize *u_args.step)); */

/*   for (int i = 0; i <= numLoops; i++) { */
/*     if ((i * (u_args.step + u_args.chunkSize) + u_args.start) > u_args.end)
 */
/*       u_args.chunkSize = (u_args.chunkSize - u_args.step)/ OMP_NUM_THREADS;
 */

/*     for (int j = 0; j < OMP_NUM_THREADS; j++) { */
/*       u_args.start = startindex; */
/*       printf("Thread %d: %d-%d", tArr[j].tID, (u_args.start + i * j), */
/*              (u_args.start + i * j + u_args.chunkSize)); */
/*       u_args.start += (i * j); */
/*       pthread_create(&(tArr->thread), NULL, omp_execution, args); */
/*     } */
/*   } */
/*   pthread_join(tArr->thread, NULL); */
/* } */

void *omp_dynamic(void *args) {
  function_args u_args = *(function_args *)args;
  while (pos < (u_args.end - u_args.step)) {
    // ENTERING MUTUAL EXCLUSION REGION
    pthread_mutex_lock(&omp_mut);
    u_args.start = pos;
    pos = (u_args.chunkSize + u_args.step + pos) > u_args.end
              ? (u_args.end - u_args.step)
              : (u_args.chunkSize + u_args.step + pos);
    printf("Thread %d pede iterações: Recebe iterações %d-%d\n", pthread_self(),
           u_args.start, (u_args.start + u_args.chunkSize));
    omp_execution(u_args.start, (u_args.chunkSize + u_args.start), u_args.step,
                  u_args.schedType, u_args.func_pack);
  }
  printf("Thread %d pede iterações: é fechada\n", pthread_self());
}

/* void *omp_guideline(void *args) { */
/*   function_args u_args = *(function_args *)args; // unpacked_args */
/*   u_int8_t defaultChunkSize = u_args.chunkSize; */
/*   while (pos < u_args.end) { */
/*     // ENTERING MUTUAL EXCLUSION REGION */
/*     pthread_mutex_lock(&omp_mut); */
/*     u_args.start = pos; */
/*     int newChunkSize = ceil((u_args.end - pos) / ((float)(OMP_NUM_THREADS)));
 */
/*     u_args.chunkSize = */
/*         (newChunkSize > defaultChunkSize) ? defaultChunkSize : newChunkSize;
 */
/*     if ((pos + u_args.chunkSize + u_args.step) > u_args.end) */
/*       pos += u_args.end - u_args.step; */
/*     else */
/*       pos += (u_args.chunkSize + u_args.step); */
/*     pos = u_args.chunkSize; */
/*     printf("Thread %d pede iterações: Recebe iterações %d-%d (teto de
 * %d/%d)", */
/*            pthread_self(), u_args.start, u_args.chunkSize, (u_args.end -
 * pos), */
/*            OMP_NUM_THREADS); */
/*       omp_execution((void *)&u_args); */
/*   } */
/* } */

void *omp_execution(int start, int comp, int step, int sched, void (*f)(int)) {
  if (sched != STATIC)
    pthread_mutex_unlock(&omp_mut);

  for (int k = start; k < comp; k += step) {
    f(k);
  }
}

void omp_for(int start, int step, int final, int schedule, int chunk_size,
             void (*f)(int)) {

  pthread_t *tArr[OMP_NUM_THREADS];
  function_args omp_args = {start,    step,       final,
                            schedule, chunk_size, (void *)f};
  pos = start;
  pthread_mutex_init(&omp_mut, NULL);

  switch (schedule) {
    /* case STATIC: */
    /*   omp_static((void *)&omp_args); */
    /*   break; */

  case DYNAMIC:
    for (int i = 0; i < OMP_NUM_THREADS; i++) {
      tArr[i] = (pthread_t *)malloc(sizeof(pthread_t));
      pthread_create(tArr[i], NULL, omp_dynamic, (void *)&omp_args);
    }
    for (int i = 0; i < OMP_NUM_THREADS; i++)
      pthread_join(*tArr[i], NULL);

    break;

    /* case GUIDELINE: */
    /* for (int i = 0; i < OMP_NUM_THREADS; i++) { */
    /*   tArr[i] = (pthread_t *)malloc(sizeof(pthread_t)); */
    /*   pthread_create(tArr[i], NULL, omp_dynamic, (void *)&omp_args); */
    /*   for(int i = 0; i < OMP_NUM_THREADS;i++){ */
    /* *)&omp_args); */
    /*   } */
  }
}

void OK(int i) {

  printf("%d\n", pthread_self());
  counter++;
}

int main(void) {
  /* omp_for(0, 1, 50, GUIDELINE, 1, OK); */
  /* counter=0; */
  omp_for(0, 2, 80, DYNAMIC, 1, OK);
  printf("%d", counter);
  /* omp_for(0, 1, 50, STATIC, 1, OK); */
  /* counter=0; */
}
