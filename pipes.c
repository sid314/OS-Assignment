#include <stdio.h>
#include <unistd.h>

void changeCase(char *inp, int size) {
  for (int i = 0; i < size; i++) {
    if (inp[i] >= 65 && inp[i] <= 90) {
      inp[i] += 32;

    } else if (inp[i] >= 97 && inp[i] <= 122) {
      inp[i] -= 32;
    }
  }
}

int main(int argc, char *argv[]) {
  int upPipe[2];
  int downPipe[2];
  char buffer[100];
  const char message[] = "Hello\n";
  if (pipe(upPipe) != 0) {
    printf("Error in creating pipe");
    return -1;
  }
  if (pipe(downPipe) != 0) {
    printf("Error in creating pipe");
    return -1;
  }
  int pid = fork();
  if (pid == -1) {

    printf("Error in creating fork");
    return -1;
  }
  if (pid == 0) {
    close(upPipe[1]);
    close(downPipe[0]);
    if (read(upPipe[0], buffer, sizeof(buffer)) == -1) {
      printf("Error in reading");
      return -1;
    }
    printf("Received at child process: %s", buffer);
    changeCase(buffer, sizeof(buffer));
    if (write(downPipe[1], buffer, sizeof(message)) == -1) {
      printf("Error in writing");
      return -1;
    }
    printf("Sent from child process: %s", buffer);
  } else {
    close(upPipe[0]);
    close(downPipe[1]);
    if (write(upPipe[1], message, sizeof(message)) == -1) {
      printf("Error in writing");
      return -1;
    }
    if (read(downPipe[0], buffer, sizeof(buffer)) == -1) {
      printf("Error in reading");
      return -1;
    }
    printf("Received at parent process: %s", buffer);
  }
  return 0;
}
