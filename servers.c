#include <t_stdio.h>
#include <unistd.h>
#include  "t_net.h"
#include <stdlib.h>
#include <sys/wait.h>
#include <signal.h>
int t_main(int);
void do_it(int n){
    wait(NULL);
    return;
}
int main(void){
    signal(SIGCHLD,do_it);
    int s_fd=s_listen(AF_INET,SOCK_STREAM,9090,5);
    if(s_fd==-1)return -1;
    while(1){
        int c_fd=h_accept(s_fd);
        if(c_fd==-1)return -1;
        pid_t pid=fork();
        if(pid==-1)E_MSG("fork",-1);
        if(pid==0){//子进程负责的任务
            close(s_fd);//关闭socket文件描述符
            t_main(c_fd);//业务处理
            //关闭本次连接
            close(c_fd);
            exit(0);
        }else close(c_fd);
    }
    return 0;
}
