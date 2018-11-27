#ifndef _MIAN_H
#define _MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <mysql/mysql.h>
#include <pthread.h>

#define STAT 6688

typedef struct node
{
    char user_name[20];
    char passwd[20];
    char ser_key[50];
}user_message;
user_message user_msg;

void *pthread(void *fd);

/* 打印表中的数据 */
void print_table(MYSQL *mysql); 

/* 查找表中是否有这个用户名 */                 
int find_table(MYSQL *mysql,char *name);    

/* 在表的最后插入新注册的用户信息 */                  
void insert_data(MYSQL *mysql);         

/* 查找表中是否有这个密码 */                     
int find_table_passwd(MYSQL *mysql,char *name,char *passwd);

/* 查找表中是否有这个验证信息 */
int find_table_key(MYSQL *mysql,char *name,char *ser_key);

/* 更改用户密码 */          
void change_data(MYSQL *mysql);           

/* 查找客户端传过来的用户名是否存在 */                    
void find_name(MYSQL *mysql,int confd);   

/* 登录账号 */              
void log_account(MYSQL *mysql,int confd);

/* 注册账号 */                     
void reg_account(MYSQL *mysql,int confd);      

/* 用户更改密码 */  
void change_passwd(MYSQL *mysql,int confd); 

/* 在线人数查询 */   
int ser_online_num(MYSQL *mysql,int confd); 

/* 注销 */
void ser_logou(MYSQL *mysql,int confd);

/* 私聊 */ 
void private_char(MYSQL *mysql,int confd);

/* 群聊 */
void group_char(MYSQL *mysql,int confd);

/* 踢人 */
void kick_player(MYSQL *mysql,int confd); 

/* 禁言 */
void banned_post(MYSQL *mysql,int confd); 

/* 解禁 */
void lift_ban(MYSQL *mysql,int confd);

/* 传输文件 */
void transfer_files(MYSQL *mysql,int confd);  

#endif
