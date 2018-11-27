#include "client.h"

int main(int argc,char *argv[])
{
    if (2 > argc)
    {
        perror("argc error!\n");
        exit(-1);
    }

    struct sockaddr_in seraddr;
    int tmp,sockfd,choice,ret_pth;
    pthread_t pth;

    sockfd = socket(AF_INET,SOCK_STREAM,0);
    if (0 > sockfd)
    {
        perror("socket error!\n");
        exit(-1);
    }

    bzero(&seraddr,sizeof(seraddr));
    seraddr.sin_family = AF_INET;
    seraddr.sin_port = htons(STAT);
    seraddr.sin_addr.s_addr = inet_addr(argv[1]);

    tmp = connect(sockfd,(struct sockaddr *)&seraddr,sizeof(seraddr));
    if (0 > tmp)
    {
        perror("connect error!\n");
        exit(-1);
    }

    while ((choice = menu_one(sockfd)) != 4)
    {
        switch(choice)
        {
            case 1: reg_account(sockfd);break;     //用户注册模块
            case 2: log_account(sockfd);break;     //用户登录模块
            case 3: change_passwd(sockfd);break;   //用户更改密码
            default:
                    printf("输入错误，请重新输入\n");
        }
    } 
    close(sockfd);
    return 0;
}

