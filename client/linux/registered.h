#ifndef _REGISTERED_H_
#define _REGISTERED_H_

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"protocol.h"


//*******结构体定义***********
struct information
{
	char id[6];					//用户创建的账号        
	char nickname[11];         //用户昵称
	char password[6];          //用户创建的密码
	char sex[2];               //用户创建的性别
	char age[4];               //用户创建的年龄
};

//***********函数声明***********
extern int register_func(struct information *info);
extern void login_func(struct information *info);

//从键盘获取size以内个字符
void get_string(char *buf, int size);

//从键盘获取size个字符
void get_size_string(char *buf, int size);

//从键盘获取一个字符 
char get_char(void);

//从键盘获取一个整数
int get_integer(void);





#endif
