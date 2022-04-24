#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/sysinfo.h>

#define OMP_NUM_THREADS 4
#define NEXT_STEP(chunk_size, inicio, j, passo, c, i)                          \
  (((chunk_size * OMP_NUM_THREADS * i) + inicio) + (chunk_size * j) +          \
   (passo + c))

    enum scheduleOpt {
      STATIC,
      DYNAMIC,
      GUIDELINE
    };

typedef struct function_args {
  __uint64_t start;
  __uint64_t step;
  __uint64_t end;
  __uint8_t schedType;
  __uint32_t chunkSize;
  void (*func_pack)(int);
} function_args;

void omp_execution(int, int, int, int,void(*)(int));

void *omp_static(void *);

void *omp_dynamic(void *);

void *omp_guideline(void *);

void omp_for(int, int, int, int, int,
             void (*)(int));
