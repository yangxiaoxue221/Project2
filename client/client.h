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

/* 菜单 */
int menu_one(int sockfd);  

/* 设置菜单 */                                     
int menu_two();      

/* 设置密码 */                                
void cli_passwd(char *passwd);  

/* 判断用户名 */
void cli_name(int sockfd);

/* 判断密码 */
int client_passwd(int sockfd);

/* 查找用户名 */                     
void find_name(int sockfd);   

/* 用户注册模块 */                        
void reg_account(int sockfd);    

/* 用户登录模块 */              
void log_account(int sockfd);

/* 用户更改密码模块 */                         
void change_passwd(int sockfd);

/* 在线人数查询 */                       
void cli_online_num(int sockfd); 

/* 注销 */
void logout(int sockfd,char *name);    

/* 私聊 */  
void private_char(int sockfd);                      

/* 群聊 */  
void group_chat(int sockfd);

/* 踢人 */
void kick_player(int sockfd);

/* 禁言 */
void banned_post(int sockfd);

/* 解禁 */
void lift_ban(int sockfd);

/* 传输文件 */                           
void transfer_files(int sockfd);     

#endif
