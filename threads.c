#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int avg;
int min;
int max;
void *max_fn(void *arg) {
  int *nums = (int *)arg;
  for (int i = 1; i <= nums[0]; i++) {
    if (nums[i] > max) {
      max = nums[i];
    }
  }

  return NULL;
}
void *min_fn(void *arg) {
  int *nums = (int *)arg;
  for (int i = 1; i <= nums[0]; i++) {
    if (nums[i] < min) {
      min = nums[i];
    }
  }

  return NULL;
}
void *avg_fn(void *arg) {

  int *nums = (int *)arg;
  for (int i = 1; i <= nums[0]; i++) {
    avg += nums[i];
  }
  avg /= nums[0];

  return NULL;
}
int main(int argc, char *argv[]) {
  int *data = (int *)malloc(argc * sizeof(int));
  data[0] = argc - 1;
  for (int i = 1; i < argc; i++) {
    data[i] = atoi(argv[i]);
  }
  min = max = data[1];
  pthread_t avg_thread;
  pthread_t min_thread;
  pthread_t max_thread;
  pthread_create(&avg_thread, NULL, avg_fn, data);
  pthread_create(&max_thread, NULL, max_fn, data);
  pthread_create(&min_thread, NULL, min_fn, data);

  pthread_join(avg_thread, NULL);
  pthread_join(max_thread, NULL);
  pthread_join(min_thread, NULL);
  printf("The average value is %d\n", avg);
  printf("The minimum value is %d\n", min);
  printf("The maximum value is %d\n", max);

  free(data);

  return 0;
}
