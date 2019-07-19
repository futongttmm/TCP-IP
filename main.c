#include <unistd.h>
#include <stdio.h>
#include "tt.h"
#include <string.h>
char *work_dir="/home/tarena/CSD1812/uc/web/html";

static char *trans_path(req_t *rq){
    char path[258];
    strcpy(path,work_dir);
    strcat(path,rq->path);
    strcpy(rq->path,path);
    return rq->path;
}
char *get_file_type(const char *file){
    char *f_t;
    f_t=strrchr(file,'.');
    if(f_t){
        if(strcmp(f_t,".png")==0)return "image/png";
        if(strcmp(f_t,".jpg")==0)return "image/jpg";
        return "text/html";
    }
    return "text/html"; 
}

//根据浏览器的请求信息组织响应信息
//HTTP1.1/  200
//Content-Type:text/html(image/jpg  image/png)
void orgn_res_info(req_t *rq,res_t *rs){
    //获取code,文件可访问200,不可访问404
    rs->code=access(rq->path,R_OK)?404:200;
    //获取文件的类型
    rs->f_type=get_file_type(rq->path);
    return;
}
static void get_request(int fd,req_t *rq){
    char buf[1024]={0};
    int r=read(fd,buf,1024);
    sscanf(buf,"%s %s %s\r\n",\
            rq->method,rq->path,rq->proto);
    if(strcmp(rq->path,"/")==0)
        strcpy(rq->path,"/index.html");
    return;
}
void response_c(int fd,req_t *rq,res_t *rs){
    char f_line[64],s_line[64];
    sprintf(f_line,"%s %d\r\n",rq->proto,rs->code);
    sprintf(s_line,"Content-Type: %s\r\n\r\n",rs->f_type);
    if(rs->code==404)strcpy(rq->path,\
            "/home/tarena/CSD1812/uc/web/html/error.html");
    write(fd,f_line,strlen(f_line));
    write(fd,s_line,strlen(s_line));
    //将文件的内容输出到浏览器
    dup2(fd,1);//将浏览器重定向到标准输出
    execlp("cat","cat",rq->path,NULL);
    return;
}
//处理客户端的通讯
int t_main(int fd){
    req_t req;
    res_t res;
    //获取浏览器的请求信息
    get_request(fd,&req);
    char *file=trans_path(&req);
    //处理请求信息,获取相关信息
    orgn_res_info(&req,&res);
    //根据获取到的相关信息,
    //组织客户端的响应头.响应客户端
    response_c(fd,&req,&res);
    return 0;
}
