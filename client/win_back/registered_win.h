#ifndef _REGISTERED_H_
#define _REGISTERED_H_

#include<stdio.h>
#include<string.h>
#include<stdlib.h>


//*******�ṹ�嶨��***********
struct information
{
	char nickname[11];         //�û�����������
	char password[6];          //�û�����������
	char sex[2];               //�û��������Ա�
	char age[4];               //�û�����������

	char login_password[6];    //����
	char login_account[6];     //�˻�

};

//***********��������***********
extern int register_func(struct information *info);
extern int login_information(struct information *info);

#endif
