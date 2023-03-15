#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void new_process(int fds[2]) 
{
  int n, prime;
  read(fds[0], &prime, 4);
  printf("prime %d\n", prime);
  if (read(fds[0], &n, 4) != 0) {
    int new_fds[2];
    pipe(new_fds);
    if (fork() == 0) {
      close(new_fds[1]);
      new_process(new_fds);
    } else {
      close(new_fds[0]);
      if (n % prime != 0) {
        write(new_fds[1], &n, 4);
      }
      while (read(fds[0], &n, 4) != 0) {
        if (n % prime != 0) {
          write(new_fds[1], &n, 4);
        }
      }
      close(new_fds[1]);
      wait(0);
      exit(0);
    }
  }
}

int
main(int argc, char *argv[])
{
  int fds[2];
  pipe(fds);
  if (fork() == 0) {
    close(fds[1]);
    new_process(fds);
    close(fds[0]);
  } else {
    close(fds[0]);
    for (int i = 2; i <= 35; i++) {
      write(fds[1], &i, 4);
    }
    close(fds[1]);
    wait(0);
    exit(0);
  }
  return 0;
}
