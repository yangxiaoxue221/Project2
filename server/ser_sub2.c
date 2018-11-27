#include "server.h"

char user_name[20];
int tmp;

/* 查找客户端传过来的用户名是否存在 */
void find_name(MYSQL *mysql,int confd)
{
    char name[20],msg[20];
    while (1)  // 判断用户名是否正确
    {
        tmp = recv(confd,name,sizeof(name),0); // 接收从服务端发送的用户名
        if (0 > tmp)
        {
            perror("recv name error!\n");
            exit(-1);
        }
        if (0 == strcmp("ok",name))
        {
            break;
        }

        strcpy(user_name,name);  // 把接收到的用户名存到另一个数组，判断密码需要使用
        bzero(&msg,sizeof(msg));
        tmp = find_table(mysql,user_name); // 查找表中是否有这个用户名
        if (-1 != tmp)
        {
            strcpy(msg,"exist");
        }
        tmp = send(confd,msg,sizeof(msg),0);
        if (0 > tmp)
        {
            perror("recv msg error!\n");
            exit(-1);
        }
    }
}

/* 注册账号 */
void reg_account(MYSQL *mysql,int confd)
{
    char msg[20],sign[20];
    find_name(mysql,confd);  // 查找客户端传过来的用户名是否存在 
    tmp = recv(confd,&user_msg,sizeof(user_msg),0);
    if (0 > tmp)
    {
        perror("recv msg error!\n");
        exit(-1);
    }
    insert_data(mysql); // 在表的最后插入新注册的用户信息
}

/* 登录账号  */
void log_account(MYSQL *mysql,int confd)
{
    char passwd[20],msg[20],ser_key[50];
    find_name(mysql,confd); // 查找客户端传过来的用户名是否存在 
    while (1)  // 判断密码是否正确
    {
        tmp =  recv(confd,passwd,sizeof(passwd),0);
        if (0 > tmp)
        {
            perror("recv passwd error!\n");
            exit(-1);
        }
        if (0 == strcmp("ok",passwd))
        {
            break;
        }
        if (0 == strcmp("error",passwd))
        {
            break;
        }
        bzero(&msg,sizeof(msg));
        tmp = find_table_passwd(mysql,user_name,passwd); // 查看这个密码是否正确
        if (-1 == tmp)
        {
            strcpy(msg,"error");
        }
        tmp = send(confd,msg,sizeof(msg),0);
        if (0  > tmp)
        {
            perror("send msg error!\n");
            exit(-1);
        }
    }
    if (0 == strcmp("error",passwd))  // 如果密码错误，判断验证信息是否正确
    {
        while (1)
        {
            tmp = recv(confd,ser_key,sizeof(ser_key),0);
            if (0 > tmp)
            {
                perror("send recv error!\n");
                exit(-1);
            }
            if (0 == strcmp("change",ser_key))
            {
                bzero(&user_msg,sizeof(user_msg));
                tmp = recv(confd,&user_msg,sizeof(user_message),0);
                if (0 > tmp)
                {
                    perror("recv user_msg\n");
                    exit(-1);
                }
                change_data(mysql);
                break;
            }

            bzero(&msg,sizeof(msg));
            tmp = find_table_key(mysql,user_name,ser_key);
            if (-1 == tmp)
            {
                strcpy(msg,"error");
            }
            tmp = send(confd,msg,sizeof(msg),0);
            if (0 > tmp)
            {
                perror("send msg error!\n");
                exit(-1);
            }
        }
    }
    char query[200] = "update message set confd= ";
    sprintf(query,"%s%d %s'%s'",query,confd,"where user_name=",user_name);
    printf("\n%s\n",query);
    tmp = mysql_real_query(mysql,query,strlen(query));
    if (tmp)
    {
        perror("mysql_real_query\n");
        exit(-1);
    }
    print_table(mysql);
}

/*  用户更改密码 */
void change_passwd(MYSQL *mysql,int confd)
{
    char msg[20],ser_key[50],passwd[20]; 
    find_name(mysql,confd);  // 查找客户端传过来的用户名是否存在
    while (1)  // 判断密码是否正确
    {
        tmp =  recv(confd,passwd,sizeof(passwd),0);
        if (0 > tmp)
        {
            perror("recv passwd error!\n");
            exit(-1);
        }
        if (0 == strcmp("ok",passwd))
        {
            break;
        }
        if (0 == strcmp("error",passwd))
        {
            break;
        }

        bzero(&msg,sizeof(msg));
        tmp = find_table_passwd(mysql,user_name,passwd); // 查看这个密码是否正确
        if (-1 == tmp)
        {
            strcpy(msg,"error");
        }
        tmp = send(confd,msg,sizeof(msg),0);
        if (0  > tmp)
        {
            perror("send msg error!\n");
            exit(-1);
        }
    }
    if (0 == strcmp("error",passwd))  // 如果密码错误，判断验证信息是否正确
    {
        while (1)
        {
            tmp = recv(confd,ser_key,sizeof(ser_key),0);
            if (0 > tmp)
            {
                perror("send recv error!\n");
                exit(-1);
            }
            if (0 == strcmp("change",ser_key))
            {
                break;
            }

            bzero(&msg,sizeof(msg));
            tmp = find_table_key(mysql,user_name,ser_key);
            if (-1 == tmp)
            {
                strcpy(msg,"error");
            }
            tmp = send(confd,msg,sizeof(msg),0);
            if (0 > tmp)
            {
                perror("send msg error!\n");
                exit(-1);
            }
        }
    }
    bzero(&user_msg,sizeof(user_msg));
    tmp = recv(confd,&user_msg,sizeof(user_message),0);
    if (0 > tmp)
    {
        perror("recv user_msg\n");
        exit(-1);
    }
    change_data(mysql);
}

/* 在线人数查询 */
int ser_online_num(MYSQL *mysql,int confd)
{
    int t,tmp,count = 0;
    char query[50] = "select * from message",message[100];
    MYSQL_RES *res;     //返回查询结果
    MYSQL_ROW row;      //返回行数据

    t = mysql_real_query(mysql,query,strlen(query));
    if (t)
    {
        perror("mysql_real_query error!\n");
        exit(-1);

    }
    res = mysql_store_result(mysql);
    while (row = mysql_fetch_row(res))
    {
        if (0 != strcmp(row[3],"0") && atoi(row[3]) != confd)
        {
            sprintf(message,"在线的人：%s",row[0]);
            tmp = send(confd,message,sizeof(message),0);
            if (0 > tmp)
            {
                perror("recv row[0] error!\n");
                exit(-1);
            }
            count++;
            sleep(1);
        }
    }
    sprintf(message,"共有%d个人在线",count);
    send(confd,message,sizeof(message),0);
    mysql_free_result(res);
}

/* 注销 */
void ser_logou(MYSQL *mysql,int confd)
{
    char name[20] = {0};
    char query[200] = "update message set confd=0 where user_name=";
    tmp = recv(confd,name,sizeof(name),0);
    if (0 > tmp)
    {
        perror("recv name error!\n");
        exit(-1);
    }
    sprintf(query,"%s'%s'",query,name);
    printf("\n%s\n",query);
    tmp = mysql_real_query(mysql,query,strlen(query));
    if (tmp)
    {
        perror("mysql_real_query\n");
        exit(-1);
    }
    print_table(mysql);
    send(confd,"exist",sizeof("exist"),0);
}

/* 私聊 */ 
void private_char(MYSQL *mysql,int confd)
{
    int cfd,t;
    char message[100],message1[100],name[20],msg[100];
    char query[50] = "select * from message";
    MYSQL_RES *res;     //返回查询结果
    MYSQL_ROW row;      //返回行数据
    tmp = recv(confd,name,sizeof(name),0); // 接收从服务端发送的用户名
    if (0 > tmp)
    {
        perror("recv name error!\n");
        exit(-1);
    }

    bzero(&msg,sizeof(msg));
    cfd = find_table(mysql,name); // 查找表中是否有这个用户名
    if (-1 == cfd || 0 == cfd)
    {
        strcpy(msg,"这个用户名不存在或者不在线！");
        tmp = send(confd,msg,sizeof(msg),0);
        if (0 > tmp)
        {
            perror("send msg error!\n");
        }
        return ;
    }

    tmp = recv(confd,message,sizeof(message),0);
    if (0 > tmp)
    {
        perror("recv message error!\n");
        exit(-1);
    }
    t = mysql_real_query(mysql,query,strlen(query));
    if (t)
    {
        perror("mysql_real_query error!\n");
        exit(-1);

    }
    res = mysql_store_result(mysql);
    while (row = mysql_fetch_row(res))
    {
        if (atoi(row[3]) == confd && atoi(row[4]) != 2 && atoi(row[4]) != 0)
        {
            break;
        }
        else if (atoi(row[3]) == confd && atoi(row[4]) == 2)
        {
            send(confd,"你被群主禁言了!",sizeof("你被群主禁言了!"),0);
            return ;
        }
        else if (atoi(row[3]) == confd && atoi(row[4]) == 0)
        {
            send(confd,"你已被踢，不能发送和接收消息!",sizeof("你已被踢，不能发送和接收消息!"),0);
            return ;
        }
    }
    mysql_free_result(res);

    sprintf(message1,"接收到%s的消息为:%s",row[0],message);
    mysql_real_query(mysql,query,strlen(query));
    res = mysql_store_result(mysql);
    while (row = mysql_fetch_row(res))
    {   
        // 找到我要发送消息的那个人的confd,并且他没有被禁言,也没有被踢
        if (atoi(row[3]) == cfd && atoi(row[4]) == 1)  
        {
            tmp = send(cfd,message1,sizeof(message1),0);
            if (0 > tmp)
            {
                perror("send message error!\n");
                exit(-1);
            }
            break;  
        }
    }
    mysql_free_result(res);
}

/* 群聊 */
void group_char(MYSQL *mysql,int confd)
{
    int cfd,t;
    char message[100],message1[100],name[20],msg[100];
    char query[50] = "select * from message";
    MYSQL_RES *res;     //返回查询结果
    MYSQL_ROW row;  

    tmp = recv(confd,message,sizeof(message),0);
    if (0 > tmp)
    {
        perror("recv message error!\n");
        exit(-1);
    }

    t = mysql_real_query(mysql,query,strlen(query));
    if (t)
    {
        perror("mysql_real_query error!\n");
        exit(-1);

    }
    res = mysql_store_result(mysql);
    while (row = mysql_fetch_row(res))
    {
        if (atoi(row[3]) == confd && atoi(row[4]) != 2 && atoi(row[4]) != 0)
        {
            break;
        }
        else if (atoi(row[3]) == confd && atoi(row[4]) == 2)
        {
            send(confd,"你被群主禁言了!",sizeof("你被群主禁言了!"),0);
            return ;
        }
        else if (atoi(row[3]) == confd && atoi(row[4]) == 0)
        {
            send(confd,"你已被踢，不能发送和接收消息!",sizeof("你已被踢，不能发送和接收消息!"),0);
            return ;
        }
    }
    mysql_free_result(res);

    sprintf(message1,"接收到%s的群聊消息为:%s",row[0],message);
    mysql_real_query(mysql,query,strlen(query));
    res = mysql_store_result(mysql);
    while (row = mysql_fetch_row(res))
    {
        if (atoi(row[3]) != 0 && atoi(row[3]) != confd && atoi(row[4]) != 0)
        {
            tmp = send(atoi(row[3]),message1,sizeof(message1),0);
            if (0 > tmp)
            {
                perror("send message error!\n");
                exit(-1);
            } 
        }
    }
    mysql_free_result(res);
}

/* 踢人 */
void kick_player(MYSQL *mysql,int confd)
{
    int cfd;
    char message[100],name[20],msg[100];
    char query[50] = "select * from message";
    MYSQL_RES *res;     //返回查询结果
    MYSQL_ROW row;      //返回行数据
    if (5 != confd)  return ;// 如果不是群主，直接return;
    tmp = recv(confd,name,sizeof(name),0); // 接收从服务端发送的用户名
    if (0 > tmp)
    {
        perror("recv name error!\n");
        exit(-1);
    }

    bzero(&msg,sizeof(msg));
    cfd = find_table(mysql,name); // 查找表中是否有这个用户名
    if (-1 == cfd)
    {
        sprintf(msg,"用户%s不存在！",name);
        tmp = send(confd,msg,sizeof(msg),0);
        return ;
    }

    mysql_real_query(mysql,query,strlen(query));
    res = mysql_store_result(mysql);
    while (row = mysql_fetch_row(res))
    {
        if (0 == strcmp(row[0],name))
        {
            if (0 == atoi(row[4]))
            {
                sprintf(msg,"%s不在群里!",name);
                send(confd,msg,sizeof(msg),0);
            }
            else
            {
                sprintf(message,"update message set other='0' where user_name='%s'",name);
                printf("\n%s\n",message);
                mysql_real_query(mysql,message,strlen(message));
                print_table(mysql);
                sprintf(msg,"%s被踢！",name);
                send(confd,msg,sizeof(msg),0);
            }
            break;
        }
    }
} 

/* 禁言 */
void banned_post(MYSQL *mysql,int confd)
{
    int cfd;
    char message[100],name[20],msg[100];
    char query[50] = "select * from message";
    MYSQL_RES *res;     //返回查询结果
    MYSQL_ROW row;      //返回行数据
    if (5 != confd)  return ;// 如果不是群主，直接return;
    tmp = recv(confd,name,sizeof(name),0); // 接收从客户端发送的用户名
    if (0 > tmp)
    {
        perror("recv name error!\n");
        exit(-1);
    }

    bzero(&msg,sizeof(msg));
    cfd = find_table(mysql,name); // 查找表中是否有这个用户名
    if (-1 == cfd)
    {
        sprintf(msg,"用户%s不存在！",name);
        tmp = send(confd,msg,sizeof(msg),0);
        return ;
    }

    mysql_real_query(mysql,query,strlen(query));
    res = mysql_store_result(mysql);
    while (row = mysql_fetch_row(res))
    {
        if (2 == atoi(row[4]) && 0 == strcmp(row[0],name))
        {
            sprintf(msg,"%s已经被禁言了!",name);
            send(confd,msg,sizeof(msg),0);
        }
        else if (0 == strcmp(row[0],name))
        {
            sprintf(message,"update message set other='2' where user_name='%s'",name);
            printf("\n%s\n",message);
            mysql_real_query(mysql,message,strlen(message));
            print_table(mysql);
            sprintf(msg,"%s被禁言！",name);
            send(confd,msg,sizeof(msg),0);
            break;
        }
    }
}

/* 解禁 */
void lift_ban(MYSQL *mysql,int confd)
{
    int cfd;
    char message[100],name[20],msg[100] = {0};
    char query[50] = "select * from message";
    MYSQL_RES *res;     //返回查询结果
    MYSQL_ROW row;      //返回行数据
    if (5 != confd)  return ;// 如果不是群主，直接return;
    tmp = recv(confd,name,sizeof(name),0); // 接收从客户端发送的用户名
    if (0 > tmp)
    {
        perror("recv name error!\n");
        exit(-1);
    }

    bzero(&msg,sizeof(msg));
    cfd = find_table(mysql,name); // 查找表中是否有这个用户名
    if (-1 == cfd)
    {
        sprintf(msg,"用户%s不存在！",name);
        tmp = send(confd,msg,sizeof(msg),0);
        return ;
    }

    mysql_real_query(mysql,query,strlen(query));
    res = mysql_store_result(mysql);
    while (row = mysql_fetch_row(res))
    {
        if (2 != atoi(row[4]) && 0 == strcmp(row[0],name))
        {
            sprintf(msg,"%s没有被禁言!",name);
            send(confd,msg,sizeof(msg),0);
        }
        else if (0 == strcmp(row[0],name))
        {
            sprintf(message,"update message set other='1' where user_name='%s'",name);
            printf("\n%s\n",message);
            mysql_real_query(mysql,message,strlen(message));
            print_table(mysql);
            sprintf(msg,"%s被解禁！",name);
            send(confd,msg,sizeof(msg),0);
            break;
        }
    }
}

/* 传输文件 */
void transfer_files(MYSQL *mysql,int confd)
{
    int cfd,flag = 0;
    char name[20],msg[1025] = {0},file_name[20];
    char query[50] = "select * from message";
    MYSQL_RES *res;     //返回查询结果
    MYSQL_ROW row;      //返回行数据
    tmp = recv(confd,name,sizeof(name),0); // 接收从客户端发送的用户名
    if (0 > tmp)
    {
        perror("recv name error!\n");
        exit(-1);
    }

    bzero(&msg,sizeof(msg));
    cfd = find_table(mysql,name); // 查找表中是否有这个用户名
    if (-1 == cfd)
    {
        sprintf(msg,"用户%s不存在！",name);
        tmp = send(confd,msg,sizeof(msg),0);
        return ;
    }

    mysql_real_query(mysql,query,strlen(query));
    res = mysql_store_result(mysql);
    while (row = mysql_fetch_row(res))
    {
        if (atoi(row[3]) == confd && atoi(row[4]) != 2 && atoi(row[4]) != 0)
        {
            break;
        }
        else if (atoi(row[3]) == confd && atoi(row[4]) == 2)
        {
            send(confd,"你被群主禁言了!",sizeof("你被群主禁言了!"),0);
            return ;
        }
        else if (atoi(row[3]) == confd && atoi(row[4]) == 0)
        {
            send(confd,"你已被踢，不能发送和接收消息!",sizeof("你已被踢，不能发送和接收消息!"),0);
            return ;
        }
    }
    mysql_free_result(res);

    mysql_real_query(mysql,query,strlen(query));
    res = mysql_store_result(mysql);
    while (row = mysql_fetch_row(res))
    {   
        // 找到我要发送消息的那个人的confd,并且他没有被禁言,也没有被踢      
        if (atoi(row[3]) == cfd && 0 == strcmp("1",row[4]))  
        {
            flag = 1;
            break;  
        }
    }
    mysql_free_result(res);
    if (0 == flag)
    {
        return ;
    }

    send(cfd,"ok",sizeof("ok"),0);
    while (1)
    {
        bzero(&msg,sizeof(msg));
        recv(confd,msg,sizeof(msg),0);
        send(cfd,msg,sizeof(msg),0);
        if (0 == strcmp("exit",msg))
        {
            break;
        }
    }
}
