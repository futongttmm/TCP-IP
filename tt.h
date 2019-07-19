#ifndef TT_H_
#define TT_H_
typedef struct{
    char method[32];
    char path[256];
    char proto[32];
}req_t;//客户端请求信息头
typedef struct{
    int code;
    char *f_type;
}res_t;
#endif
