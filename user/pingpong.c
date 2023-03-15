#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int pid;
  int fds[2];
  char buf[64];

  pipe(fds);

  pid = fork();

  if(pid == 0) {
    read(fds[0], buf, sizeof(buf));
    printf("%d: received ping\n", getpid());
    write(fds[1], "ping", 4);
    exit(0);
  } else {
    write(fds[1], "pong", 4);
    read(fds[0], buf, sizeof(buf));
    printf("%d: received pong\n", getpid());
    exit(0);
  }

}
