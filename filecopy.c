#include <stdio.h>
#include <unistd.h>
int main(int argc, char *argv[]) {
  int p[2];
  if (pipe(p) != 0) {
    printf("Error in creating pipe");
    return -1;
  }
  int pid = fork();
  if (pid == -1) {
    printf("Error in creating fork");
    return -1;
  }
  if (pid == 0) {
    close(p[1]);
    FILE *file = NULL;
    int bytesread = 0;
    char buff[1024];
    file = fopen(argv[2], "wb");
    if (file != NULL) {
      while ((bytesread = read(p[0], buff, sizeof(buff))) > 0) {
        fwrite(buff, bytesread, 1, file);
      }
    }
    fclose(file);
    close(p[0]);
  } else {
    close(p[0]);
    FILE *file = NULL;
    int bytesread = 0;
    char buff[1024];
    file = fopen(argv[1], "rb");
    if (file != NULL) {
      while ((bytesread = fread(buff, 1, sizeof(buff), file)) > 0) {
        write(p[1], buff, bytesread);
      }
    }
    fclose(file);
    close(p[1]);
  }
  return 0;
}
