#include "server.h"

/* 打印表中的数据 */
void print_table(MYSQL *mysql)
{
    int t;
    char query[50] = "select * from message";
    MYSQL_RES *res;     //返回查询结果
    MYSQL_ROW row;      //返回行数据

    t = mysql_real_query(mysql,query,strlen(query));
    if (t)
    {
        perror("mysql_real_query error!\n");
        exit(-1);

    }
    printf("\n-------------------------------------------------------------------------------------\n");
    printf("%-20s   %-20s    %-20s %-20s %-20s","用户名","用户密码","验证内容","confd","other");
    printf("\n-------------------------------------------------------------------------------------\n");
    res = mysql_store_result(mysql);
    while (row = mysql_fetch_row(res))
    {
        for (t=0; t<mysql_num_fields(res); t++)
        {
            printf("%-20s",row[t]);
        }
        printf("\n");
    }
    mysql_free_result(res);
}

/* 查找表中是否有这个用户名 */
int find_table(MYSQL *mysql,char *name)
{
    int t;
    char query[50] = "select * from message";
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
        if (0 == strcmp(row[0],name))
        {
            mysql_free_result(res);
            return atoi(row[3]);
        }
    }
    mysql_free_result(res);
    return -1;
}

/* 在表的最后插入新注册的用户信息 */
void insert_data(MYSQL *mysql)
{
    int t;
    char *head = "insert into message values(";
    char query[200];

    sprintf(query,"%s'%s','%s','%s','%d','%s'%s",head,user_msg.user_name,user_msg.passwd,user_msg.ser_key,0,"1",")");

    t = mysql_real_query(mysql,query,strlen(query));
    if (t)
    {
        perror("mysql_real_query\n");
        exit(-1);
    }
    print_table(mysql);
}


/* 查找表中是否有这个密码 */
int find_table_passwd(MYSQL *mysql,char *name,char *passwd)
{
    int t;
    char query[50] = "select * from message";
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
        if (0 == strcmp(row[1],passwd) && 0 == strcmp(row[0],name))
        {
            mysql_free_result(res);
            return 1;
        }
    }
    mysql_free_result(res);
    return -1;
}

/* 查找表中是否有这个验证信息 */
int find_table_key(MYSQL *mysql,char *name,char *ser_key)
{
    int t;
    char query[50] = "select * from message";
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
        if (0 == strcmp(row[2],ser_key) && 0 == strcmp(row[0],name))
        {
            mysql_free_result(res);
            return 1;
        }
    }
    mysql_free_result(res);
    return -1;
}

/* 更改用户密码 */
void change_data(MYSQL *mysql)
{
    int t;
    char *head = "update message set passwd=";
    char query[200];

    sprintf(query,"%s'%s' where user_name='%s'",head,user_msg.passwd,user_msg.user_name);

    t = mysql_real_query(mysql,query,strlen(query));
    if (t)
    {
        perror("mysql_real_query\n");
        exit(-1);
    }
    print_table(mysql);
}


