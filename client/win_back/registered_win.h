#ifndef _REGISTERED_H_
#define _REGISTERED_H_

#include<stdio.h>
#include<string.h>
#include<stdlib.h>


//*******结构体定义***********
struct information
{
	char nickname[11];         //用户创建的名字
	char password[6];          //用户创建的密码
	char sex[2];               //用户创建的性别
	char age[4];               //用户创建的年龄

	char login_password[6];    //密码
	char login_account[6];     //账户

};

//***********函数声明***********
extern int register_func(struct information *info);
extern int login_information(struct information *info);

#endif
