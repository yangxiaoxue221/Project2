#include "server.h"

MYSQL *mysql;

void *pthread(void *fd)
{
    char sign[20];
    int tmp,confd = *(int *)fd;

    while (1)
    {
        tmp = recv(confd,sign,sizeof(sign),0);  // 接受从客户端发来的信号，以确定做哪一步工作
        if (0 > tmp)
        {
            perror("recv name error!\n");
            exit(-1);
        }
        if (0 == strcmp("1",sign))   // 注册账号
        {
            reg_account(mysql,confd);
        }
        if (0 == strcmp("2",sign))   // 账户登录
        {
            log_account(mysql,confd);   
        }
        if (0 == strcmp("3",sign))   // 用户更改密码
        {
            change_passwd(mysql,confd); 
        }
        if (0 == strcmp("4",sign))   // 在线人数查询
        {
            ser_online_num(mysql,confd); 
        }
        if (0 == strcmp("5",sign))   // 私聊
        {
            private_char(mysql,confd);
        }
        if (0 == strcmp("6",sign))   // 群聊
        {
            group_char(mysql,confd);
        }
        if (0 == strcmp("7",sign))   // 注销
        {
            ser_logou(mysql,confd); 
        } 
        if (0 == strcmp("8",sign))   // 踢人
        {
            kick_player(mysql,confd); 
        }
        if (0 == strcmp("9",sign))   // 禁言
        {
            banned_post(mysql,confd); 
        }
        if (0 == strcmp("10",sign))   // 解禁
        {
            lift_ban(mysql,confd); 
        }
        if (0 == strcmp("11",sign))   // 传输文件
        {
            transfer_files(mysql,confd); 
        }
    }
    pthread_exit(NULL);
}

int main()
{
    struct sockaddr_in seraddr;
    pthread_t pth;
    int tmp,ret_pth;
    int confd,sockfd;

    mysql = mysql_init(NULL); //创建一个MYSQL句柄并初始化
    if (!mysql)
    {
        perror("mysql_init error!\n");
        exit(-1);
    }

    if (!mysql_real_connect(mysql,"localhost","root","yxx2211227","Msg",0,NULL,0))    // 链接MYSQL数据库
    {
        perror("mysql_real_connect error!\n");
        exit(-1);
    }

    // 打印表中的数据
    print_table(mysql);

    sockfd = socket(AF_INET,SOCK_STREAM,0);
    if (0 > sockfd)
    {
        perror("socket error!\n");
        exit(-1);
    }

    bzero(&seraddr,sizeof(seraddr));
    seraddr.sin_family = AF_INET;
    seraddr.sin_port = htons(STAT);
    seraddr.sin_addr.s_addr = htonl(INADDR_ANY);

    tmp = bind(sockfd,(struct sockaddr *)&seraddr,sizeof(seraddr));
    if (0 > tmp)
    {
        perror("bind error!\n");
        exit(-1);
    }

    tmp = listen(sockfd,5);
    if (0 > tmp)
    {
        perror("bind error!\n");
        exit(-1);
    }

    while(1)
    {
        confd = accept(sockfd,NULL,NULL);
        if (0 > confd)
        {
            perror("accept error!\n");
            exit(-1);
        }

        ret_pth = pthread_create(&pth, NULL, pthread, (void *)&confd);
        if (0 != ret_pth)
        {
            perror("pthread_create error!\n");
            exit(-1);
        }
    }

    close(sockfd);
    close(confd);
    pthread_join(pth,NULL);
    mysql_close(mysql);
    return 0;
}
