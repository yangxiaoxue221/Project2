#include "client.h"

/* 菜单  */
int menu_one(int sockfd)
{
    int result,tmp;
    printf("\n-------------------------------------------------\n\n");
    printf("\t       欢 迎 进 入 聊 天 室\n\n");
    printf("\t\t1、用户注册模块\n");
    printf("\t\t2、用户登录模块\n");
    printf("\t\t3、用户更改密码模块\n");
    printf("\t\t4、用户退出\n");
    printf("\n-------------------------------------------------\n\n");
    printf("请输入你的选择:\n");
    scanf("%d",&result);
    if (4 == result)
    {
        tmp = send(sockfd,"0",sizeof("0"),0);
        if (0 > tmp)
        {
            perror("send 0 error!\n");
            exit(-1);
        }
    }
    return result;
}

/* 聊天菜单 */
int menu_two()
{
    int result;
    printf("\n-------------------------------------------------\n\n");
    printf("\t       欢 迎 登 录 聊 天 室\n\n");
    printf("\t\t1、在线人数查询\n");
    printf("\t\t2、私聊\n");
    printf("\t\t3、群聊\n");
    printf("\t\t4、踢人\n");
    printf("\t\t5、禁言\n");
    printf("\t\t6、解禁\n");
    printf("\t\t7、文件传输\n");
    printf("\t\t8、注销\n");
    printf("\n-------------------------------------------------\n\n");
    printf("请输入你的选择:\n");
    scanf("%d",&result);
    return result;
}

/* 设置密码 */
void cli_passwd(char *passwd)
{
    char passwd1[20];
    int p2 = 1;        
    printf("请输入新的用户密码,仅包含字母和数字(6 <= len < 20):\n");
    while (1 == p2)
    {
        scanf("%s",passwd);
        int i,zm = 0,sz = 0;
        for (i=0; passwd[i]; i++)
        {
            if (passwd[i] >= '0' && passwd[i] <= '9')
            {
                sz++;
            }
            else if (passwd[i] >= 'a' && passwd[i] <= 'z')
            {
                zm++;
            }
            else if (passwd[i] >= 'A' && passwd[i] <= 'Z')
            {
                zm++;
            }
            else
            {
                printf("密码只包含字母和数字，请重新输入:\n");
                scanf("%s",passwd);
                i = -1;
            }
        }
        if (sz+zm < 6 || sz+zm >= 20)
        {
            printf("密码长度不符合要求，请重新输入:\n");
            continue;
        }
        printf("请再输入一次密码(和第一次密码相同):\n");
        scanf("%s",passwd1);
        if (0 != strcmp(passwd,passwd1))
        {
            printf("两次密码不一致，请重新输入(len < 20):\n");
            continue;
        }
        p2 = 0;
    }
}

char name[20],name1[20];
/* 判断用户名 */
void  cli_name(int sockfd)
{
    char msg[20];
    int tmp;
    printf("请输入用户名(len < 20):\n");
    while (1)
    {
        scanf("%s",name);
        tmp = send(sockfd,name,sizeof(name),0);
        if (0 > tmp)
        {
            perror("send name error!\n");
            exit(-1);
        }
        bzero(&msg,sizeof(msg));
        tmp = recv(sockfd,msg,sizeof(msg),0);
        if (0 > tmp)
        {
            perror("recv msg error!\n");
            exit(-1);
        }
        if (0 == strcmp("exist",msg))
        {
            tmp = send(sockfd,"ok",sizeof("ok"),0);
            if (0 > tmp)
            {
                perror("send ok error!\n");
                exit(-1);
            }
            break;
        }
        printf("用户名输入错误，请重新输入:\n");
    }

}
/* 判断密码 */
int client_passwd(int sockfd)
{
    char msg[20],passwd[20];
    int count = 0,tmp;
    printf("请输入登录的用户密码(len < 20):\n");
    while (1)
    {
        scanf("%s",passwd);
        tmp = send(sockfd,passwd,sizeof(passwd),0);
        if (0 > tmp)
        {
            perror("send passwd error!\n");
            exit(-1);
        }
        bzero(&msg,sizeof(msg));
        tmp = recv(sockfd,msg,sizeof(msg),0);
        if (0 > tmp)
        {
            perror("recv msg error!\n");
            exit(-1);
        }
        if (0 == strcmp("error",msg))
        {
            count++;
            if (3 <= count)
            {
                tmp = send(sockfd,"error",sizeof("error"),0);
                if (0 > tmp)
                {
                    perror("send enter error!\n");
                    exit(-1);
                }
                break;
            }
            printf("密码输入错误，还可以输入%d次：\n",3-count);
            continue;
        }
        tmp = send(sockfd,"ok",sizeof("ok"),0);
        if (0 > tmp)
        {
            perror("send ok error!\n");
            exit(-1);
        }
        break;
    }
    return count;
}

/* 用户注册模块 */
void reg_account(int sockfd)
{
    int choice,tmp,i = 0;
    char msg[20],passwd[20];
    int p1 = 1,p2 = 1;
    tmp = send(sockfd,"1",sizeof("1"),0);
    if (0 > tmp)
    {
        perror("send 1 error!\n");
        exit(-1);
    }

    printf("请输入要注册的用户名(len < 20):\n");
    while (1 == p1)
    {
        scanf("%s",user_msg.user_name);
        tmp = send(sockfd,user_msg.user_name,sizeof(user_msg.user_name),0);
        if (0 > tmp)
        {
            perror("send name error!\n");
            exit(-1);
        }

        bzero(&msg,sizeof(msg));
        tmp = recv(sockfd,msg,sizeof(msg),0);
        if (0 > tmp)
        {
            perror("recv msg error!\n");
            exit(-1);
        }
        if (0 == strcmp("exist",msg))
        {
            printf("该用户名已存在，请重新输入(len < 20):\n");
            continue;
        }
        p1 = 0;

        tmp = send(sockfd,"ok",sizeof("ok"),0);
        if (0 > tmp)
        {
            perror("send ok error!\n");
            exit(-1);
        }

        cli_passwd(passwd);
        strcpy(user_msg.passwd,passwd); // 输入密码

        printf("请输入验证内容，忘记密码时，可输入验证内容重新修改密码:\n");
        scanf("%s",user_msg.ser_key);

        tmp = send(sockfd,&user_msg,sizeof(user_msg),0);
        if (0 > tmp)
        {
            perror("send msg error!\n");
            exit(-1);
        }

        printf("\n注册成功，可以登录！\n");
        break;
    }
}

/* 用户更改密码模块 */
void change_passwd(int sockfd)
{
    char passwd[20],ser_key[50],msg[20];
    int tmp,count = 0;
    tmp = send(sockfd,"3",sizeof("3"),0); 
    if (0 > tmp)
    {
        perror("send 3 error!\n");
        exit(-1);
    }

    cli_name(sockfd); // 判断用户名
    count = client_passwd(sockfd); // 判断密码
    if (3 == count)
    {
        printf("密码输入错误次数过多，请输入验证消息，更改密码：\n");
        int i=0;
        while (3 > i)
        {
            scanf("%s",ser_key);
            tmp = send(sockfd,ser_key,sizeof(ser_key),0);
            if (0 > tmp)
            {
                perror("send ser_key error!\n");
                exit(-1);
            }

            bzero(&msg,sizeof(msg));
            tmp = recv(sockfd,msg,sizeof(msg),0);
            if (0 > tmp)
            {
                perror("recv msg error!\n");
                exit(-1);
            }
            if (0 == strcmp("error",msg))
            {
                if (2-i > 0)
                {
                    printf("验证消息错误，还可以再输入%d次：\n",2-i);
                }
                i++;
            }
            else
            {
                break;
            }
        }
        if (3 == i)
        {
            printf("验证消息和密码都输入错误码，强制退出！\n");
            return ;
        }
        tmp = send(sockfd,"change",sizeof("change"),0);
        if (0 > tmp)
        {
            perror("send change error!\n");
            exit(-1);
        }

    }
    bzero(&user_msg,sizeof(user_msg));
    strcpy(user_msg.user_name,name);
    cli_passwd(passwd);  // 输入密码
    strcpy(user_msg.passwd,passwd);

    tmp = send(sockfd,&user_msg,sizeof(user_message),0);
    if (-1 == tmp)
    {
        perror("send user_msg error!\n");
        exit(-1);
    }
}

void *pthread(void *fd)
{
    int tmp,sockfd = *(int *)fd;
    FILE *fp;
    char message[1025],file_name[20];
    while(1)
    {
        bzero(&message,sizeof(message));
        tmp = recv(sockfd,message,sizeof(message),0);
        if (0 > tmp)
        {
            perror("recv message error!\n");
            exit(-1);
        }
        if (0 == strcmp("ok",message))
        {
            fp = fopen("proj.txt","w");
            if (NULL == fp)
            {
                perror("fopen file_name error!\n");
                exit(-1);
            }
            while (1)
            {
                bzero(&message,sizeof(message));
                recv(sockfd,message,sizeof(message),0);
                if (0 == strcmp("exit",message))
                {
                    break;
                }
                fwrite(message,1,strlen(message),fp);
            }
            fclose(fp);
        }
        if (0 == strcmp("exist",message))
        {
            break;
        }
        printf("\n------------------------------------\n");
        printf("%s",message);
        printf("\n------------------------------------\n");
    }
    pthread_exit(NULL);
}

/* 在线人数查询 */
void cli_online_num(int sockfd)
{
    int tmp;
    tmp = send(sockfd,"4",sizeof("4"),0);  
    if (0 > tmp)
    {
        perror("send 4 error!\n");
        exit(-1);
    }
}

/* 注销 */
void cli_logout(int sockfd,char *name)
{
    int tmp;
    tmp = send(sockfd,"7",sizeof("7"),0);
    if (0 > tmp)
    {
        perror("send 7 error!\n");
        exit(-1);
    }
    sleep(2);
    tmp = send(sockfd,name,sizeof(name),0);
    if (0 > tmp)
    {
        perror("send name error!\n");
        exit(-1);
    }
}

/* 私聊 */
void private_char(int sockfd)
{
    int tmp;
    char message[100] = {0},user_name[20];
    tmp = send(sockfd,"5",sizeof("5"),0);
    if (0 > tmp)
    {
        perror("send 5 error!\n");
        exit(-1);
    }
    printf("输入聊天对象的用户名:\n");
    scanf("%s",user_name);
    tmp = send(sockfd,user_name,sizeof(user_name),0);
    if (0 > tmp)
    {
        perror("send name error!\n");
        exit(-1);
    }
    printf("请输入你要发送的内容：\n");
    scanf("%s",message);
    tmp = send(sockfd,message,sizeof(message),0);
    if (0 > tmp)
    {
        perror("send message error!\n");
        exit(-1);
    }
}

/* 群聊 */
void group_chat(int sockfd)
{
    int tmp;
    char message[100];
    tmp = send(sockfd,"6",sizeof("6"),0);
    if (0 > tmp)
    {
        perror("send 6 error!\n");
        exit(-1);
    }
    printf("请输入你要发送的内容：\n");
    scanf("%s",message);
    tmp = send(sockfd,message,sizeof(message),0);
    if (0 > tmp)
    {
        perror("send message error!\n");
        exit(-1);
    }
}

/* 踢人 */
void kick_player(int sockfd)
{
    int tmp;
    char user_name[20] = {0};
    tmp = send(sockfd,"8",sizeof("8"),0);
    if (0 > tmp)
    {
        perror("send 8 error!\n");
        exit(-1);
    }
    printf("输入想要踢的用户名:\n");
    scanf("%s",user_name);
    tmp = send(sockfd,user_name,sizeof(user_name),0);
    if (0 > tmp)
    {
        perror("send name error!\n");
        exit(-1);
    }
}

/* 禁言 */
void banned_post(int sockfd)
{
    int tmp;
    char user_name[20] = {0};
    tmp = send(sockfd,"9",sizeof("9"),0);
    if (0 > tmp)
    {
        perror("send 9 error!\n");
        exit(-1);
    }
    printf("输入想禁言的用户名:\n");
    scanf("%s",user_name);
    tmp = send(sockfd,user_name,sizeof(user_name),0);
    if (0 > tmp)
    {
        perror("send name error!\n");
        exit(-1);
    }
}

/* 解禁 */
void lift_ban(int sockfd)
{
    int tmp;
    char user_name[20] = {0};
    tmp = send(sockfd,"10",sizeof("10"),0);
    if (0 > tmp)
    {
        perror("send 10 error!\n");
        exit(-1);
    }
    printf("输入想解禁的用户名:\n");
    scanf("%s",user_name);
    tmp = send(sockfd,user_name,sizeof(user_name),0);
    if (0 > tmp)
    {
        perror("send name error!\n");
        exit(-1);
    }
}

/* 传输文件 */
void transfer_files(int sockfd)
{
    int tmp;
    char user_name[20],file_name[20],msg[1025],file[20];
    FILE *fp;
    tmp = send(sockfd,"11",sizeof("11"),0);
    if (0 > tmp)
    {
        perror("send 11 error!\n");
        exit(-1);
    }
    printf("输入接收文件的用户名:\n");
    scanf("%s",user_name);
    send(sockfd,user_name,sizeof(user_name),0);

    printf("输入要读取文件的路径名:\n");
    scanf("%s",file_name);
    fp = fopen(file_name,"r");
    if (NULL == fp)
    {
        perror("open error!\n");
        exit(-1);
    }

    while (1)
    {
        bzero(&msg,sizeof(msg));
        fread(msg,1,1024,fp);
        send(sockfd,msg,sizeof(msg),0);
        if (feof(fp))
        {
            break;
        }
        usleep(100);
    }
    send(sockfd,"exit",sizeof("exit"),0);
}

/* 用户登录模块 */
void log_account(int sockfd)
{
    int tmp,count = 0,choice,res_pth;
    pthread_t pth;
    char ser_key[50],msg[20],passwd[20];
    tmp = send(sockfd,"2",sizeof("2"),0);  
    if (0 > tmp)
    {
        perror("send sign error!\n");
        exit(-1);
    }

    cli_name(sockfd); // 判断用户名
    count = client_passwd(sockfd); // 判断密码
    if (3 == count)
    {
        printf("密码输入错误次数过多，请输入验证消息，更改密码：\n");
        int i=0;
        while (3 > i)
        {
            scanf("%s",ser_key);
            tmp = send(sockfd,ser_key,sizeof(ser_key),0);
            if (0 > tmp)
            {
                perror("send ser_key error!\n");
                exit(-1);
            }

            bzero(&msg,sizeof(msg));
            tmp = recv(sockfd,msg,sizeof(msg),0);
            if (0 > tmp)
            {
                perror("recv msg error!\n");
                exit(-1);
            }
            if (0 == strcmp("error",msg))
            {
                if (2-i > 0)
                {
                    printf("验证消息错误，还可以再输入%d次：\n",2-i);
                }
                i++;
            }
            else
            {
                break;
            }
        }
        if (3 == i)
        {
            printf("验证消息和密码都输入错误码，强制退出！\n");
            return ;
        }
        else
        {
            tmp = send(sockfd,"change",sizeof("change"),0);
            if (0 > tmp)
            {
                perror("send change error!\n");
                exit(-1);
            }

            bzero(&user_msg,sizeof(user_msg));
            strcpy(user_msg.user_name,name);
            cli_passwd(passwd);  // 输入密码
            strcpy(user_msg.passwd,passwd);

            tmp = send(sockfd,&user_msg,sizeof(user_message),0);
            if (-1 == tmp)
            {
                perror("send user_msg error!\n");
                exit(-1);
            }
        }
    }
    printf("\n登 录 成 功!\n");
    strcpy(name1,name);
    res_pth = pthread_create(&pth,NULL,pthread,(void *)&sockfd);
    if (0 != res_pth)
    {
        perror("pthread_create error!\n");
        exit(-1);
    }
    while ((choice = menu_two()))
    {
        if (8 == choice)
        {  
            cli_logout(sockfd,name1); 		   
            break;
        }
        switch (choice)
        {
            case 1: cli_online_num(sockfd); break;  // 在线人数查询
            case 2: private_char(sockfd); break;    // 私聊
            case 3: group_chat(sockfd); break;      // 群聊
            case 4: kick_player(sockfd); break;     // 踢人
            case 5: banned_post(sockfd); break;     // 禁言
            case 6: lift_ban(sockfd); break;        // 解禁
            case 7: transfer_files(sockfd); break;  // 传输文件
            default:
                    printf("输入错误，请重新输入\n");
        }
    }
}
