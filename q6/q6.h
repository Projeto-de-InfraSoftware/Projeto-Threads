#include <bits/types.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/sysinfo.h>
#include <sys/types.h>

#define OMP_NUM_THREADS 4
#define NEXT_STEP(chunk_size, inicio, j, passo, c, i)                          \
  (((chunk_size * OMP_NUM_THREADS * i) + inicio) + (chunk_size * j) +          \
   (passo + c))

enum scheduleOpt { STATIC, DYNAMIC, GUIDELINE };

typedef struct threadArr {
  pthread_t thread;
  u_int8_t tID;
} threadArr;

typedef struct function_args {
  __uint64_t start;
  __uint64_t step;
  __uint64_t end;
  __uint8_t schedType;
  __uint32_t chunkSize;
  void (*func_pack)(int);
  threadArr *tArr;
} function_args;


void *omp_execution(void *args);

void* omp_static(void *args);

void* omp_dynamic(void *args);

void* omp_guideline(void *args);

void omp_for(int start, int step, int final, int schedule, int chunk_size,
             void (*f)(int));
