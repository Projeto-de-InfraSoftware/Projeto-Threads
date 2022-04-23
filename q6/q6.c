#include "q6.h"
#include <assert.h>
#include <bits/types.h>
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
  function_args u_args = *(function_args *)args; // unpacked_args
  while (pos < u_args.end) {
    // ENTERING MUTUAL EXCLUSION REGION
    pthread_mutex_lock(&omp_mut);
    u_args.start = pos;
    if ((pos + u_args.chunkSize + u_args.step) > u_args.end)
      pos += (pos + u_args.chunkSize + u_args.step) % u_args.end;
    else
      pos += (u_args.chunkSize + u_args.step);
    printf("Thread %d pede iterações: Recebe iterações %d-%d\n",
           &u_args.tArr->tID, u_args.start, (u_args.start + u_args.chunkSize));
    pthread_create(&u_args.tArr->thread, NULL, omp_execution, (void *)&u_args);
    // EXITING MUTUAL EXCLUSION REGION
  }
  pthread_join(u_args.tArr->thread, NULL);
}

void *omp_guideline(void *args) {
  __uint64_t pos = 0;
  pthread_mutex_init(&omp_mut, NULL);
  threadArr tArr[OMP_NUM_THREADS]; // TODO refactor as unpack macro
  function_args u_args = *(function_args *)args; // unpacked_args
  u_int8_t defaultChunkSize = u_args.chunkSize;
  while (pos < u_args.end) {
    // ENTERING MUTUAL EXCLUSION REGION
    pthread_mutex_lock(&omp_mut);
    u_args.start = pos;
    int newChunkSize = ceil((u_args.end - pos) / ((float)(OMP_NUM_THREADS)));
    u_args.chunkSize =
        (newChunkSize > defaultChunkSize) ? defaultChunkSize : newChunkSize;
    if ((pos + u_args.chunkSize + u_args.step) > u_args.end)
      pos += (pos + u_args.chunkSize + u_args.step) % u_args.end;
    else
      pos += (u_args.chunkSize + u_args.step);
    pos = u_args.chunkSize;
    printf("Thread %d pede iterações: Recebe iterações %d-%d (teto de %d/%d)",
           tArr->tID, u_args.start, u_args.chunkSize, (u_args.end - pos),
           OMP_NUM_THREADS);

    pthread_create(&(tArr->thread), NULL, omp_execution, (void *)&u_args);
    // EXITING MUTUAL EXCLUSION REGION
  }
  pthread_join(tArr->thread, NULL);
}

void *omp_execution(void *args) {
  function_args u_args = *(function_args *)args;
  if (u_args.schedType != STATIC)
    pthread_mutex_unlock(&omp_mut);

  for (int k = u_args.start; k < (u_args.chunkSize + u_args.start);
       k += u_args.step) {
    u_args.func_pack(k);
  }
}

void omp_for(int start, int step, int final, int schedule, int chunk_size,
             void (*f)(int)) {

  threadArr tArr[OMP_NUM_THREADS];
  function_args omp_args[OMP_NUM_THREADS] = {start,    step,       final,
                                             schedule, chunk_size, (void *)f};
  pthread_mutex_init(&omp_mut, NULL);
  pos = start;

  switch (schedule) {
    /* case STATIC: */
    /*   omp_static((void *)&omp_args); */
    /*   break; */

  case DYNAMIC:
    for (int i = 0; i < OMP_NUM_THREADS; i++) {
      omp_args[i].tArr = &tArr[i];
      pthread_create(&(tArr[i].thread), NULL, omp_dynamic,
                     (void *)&omp_args[i]);
    }
    break;

    /* case GUIDELINE: */
    /*   for(int i = 0; i < OMP_NUM_THREADS;i++){ */
    /*     omp_args.tArr = &tArr[i]; */
    /*     pthread_create(&tArr[i].thread, NULL, omp_guideline, (void
     * *)&omp_args); */
    /*   } */
    /*   break; */
    /* } */
  }
}

void OK(int i) {
  printf("This is the %d OK!\n", i);
  counter++;
}

int main(void) {

  omp_for(0, 1, 50, DYNAMIC, 1, OK);
  /* omp_for(0, 1, 50, GUIDELINE, 1, OK); */
  /* counter=0; */
  /* omp_for(0, 1, 50, STATIC, 1, OK); */
  /* counter=0; */
}
