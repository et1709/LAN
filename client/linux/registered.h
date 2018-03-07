#ifndef _REGISTERED_H_
#define _REGISTERED_H_

#include<stdio.h>
#include<string.h>
#include<stdlib.h>


//*******结构体定义***********
struct information
{
	char nickname[20];//用户创建的名字
	char password[20];//用户创建的密码
	char sex;//用户创建的性别
	int age;//用户创建的年龄

	char login_password[20];
	int login_account;

};

//***********函数声明***********
extern int register_func(struct information *info);
extern int dengluxx(struct information *info);

#endif
