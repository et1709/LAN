#ifndef _REGISTERED_H_
#define _REGISTERED_H_

#include<stdio.h>
#include<string.h>
#include<stdlib.h>

//***********宏定义***********
#define NICKNAME_LEN	11
#define PASSWORD_LEN	6
#define SEX_LEN			2
#define AGE_LEN			4
#define ID_LEN			6

//*******结构体定义***********
struct information
{
	char id[ID_LEN]
	char nickname[NICKNAME_LEN];		//用户创建的名字
	char password[PASSWORD_LEN];		//用户创建的密码
	char sex[SEX_LEN];               	//用户创建的性别
	char age[AGE_LEN];					//用户创建的年龄
};

//***********函数声明***********
int register_func(struct information *info);
void login_func(AGREEMENT *data, struct information *info, int flag)；

#endif
