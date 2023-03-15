#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

char buf[512];

/********************************************************
* Description: split input which from pipe into args
* Inuput：empty args array to store args
* Return：how many args are parsed
**********************************************************/
int 
parse_args(char** args)
{
  int args_num = 0;
  int n, m;
  char *p, *q;
  m = 0;
  while((n = read(0, buf+m, sizeof(buf)-m-1)) > 0){
    m += n;
    buf[m] = '\0';
    p = buf;
    while((q = strchr(p, '\n')) != 0){
      *q = 0;
      args[args_num]=malloc(512);
      memmove(args[args_num++], p, strlen(p)+1);
      p = q+1;
    }
    if(m > 0){
      m -= p - buf;
      memmove(buf, p, m);
    }
  }
  return args_num;
}

int
main(int argc, char *argv[])
{
  int args_num;
  char* xargs[MAXARG];
  char* new_argv[MAXARG]; // new argv for exec
  args_num = parse_args(xargs);
  for (int i = 0; i < args_num; i++) {
    if (fork() == 0) {
      int j=0;
      if (argc == 1) {
        new_argv[j++] = "echo";
      }
      for (; j < argc - 1; j++) { // exclude xargs which is argv[0]
        new_argv[j] = argv[j+1];
      }
      new_argv[j] = xargs[i];
      exec(new_argv[0], new_argv);
      fprintf(2, "exec %s failed\n", new_argv[0]);
    } else {
      wait(0);
      continue;
    }
  }
  exit(0);
}