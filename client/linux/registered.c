#include<stdio.h>
#include<string.h>
#include"registered.h"

int register_func(struct information *info)
{  
	char tmp_sex;
	char ch, i;
	int tmp_age, flag;

	printf("==================注册输入阶段:===============\n");
	printf("创建账号(5位数):\n");
	while((ch = getchar() != '\n' && ch != EOF));
	i = 0;
	while(1)
	{		
		info->login_account[i] = getchar();
		if('\n' == info->login_account[i] && i < 5)
		{
			i = 0;
			printf("您输入的位数不够,请重新输入!\n");
			memset(info->login_account, 0, sizeof(info->login_account));
			continue;
		}
		else if(5 == i)
		{
			break;
		}
		i++;
	}
	info->login_account[i] = '\0';
	printf("login_account = %s\n", info->login_account);
	
		
	//用于创建密码
	while((ch = getchar() != '\n' && ch != EOF));
	printf("请输入需要创建的密码(5位数):\n");	
	i = 0;
	while(1)
	{
		system("stty -echo");
		info->password[i] = getchar();
		system("stty echo");
		if('\n' == info->password[i] && i < 5)
		{
			i = 0;
			printf("您输入的位数不够,请重新输入!\n");
			memset(info->password, 0, sizeof(info->password));
			continue;
		}
		else if(5 == i)
		{
			break;
		}
		i++;		
	}	
	info->password[i] = '\0';
	printf("password = %s\n", info->password);
	

	//创建昵称
	while((ch = getchar() != '\n' && ch != EOF));
	printf("请输入需要创建的呢称(10位英文字母):\n");	
	i = 0;
	while(1)
	{
		info->nickname[i] = getchar();
		if('\n' == info->nickname[i] && i < 10)
		{
			i = 0;
			printf("您输入的位数不够,请重新输入!\n");
			memset(info->nickname, 0, sizeof(info->nickname));
			continue;
		}
		else if(10 == i)
		{
			break;
		}
		i++;				
	}
	info->nickname[i] = '\0';
	printf("nickname = %s\n",info->nickname);  //打印输入的昵称


	//创建年龄
	while((ch = getchar() != '\n' && ch != EOF));
	printf("请输入你的年龄(3位数):\n");	
	i = 0;
	while(1)
	{
		info->age[i] = getchar();
		if('\n' == info->age[i] && i < 3)
		{
			i = 0;
			printf("您输入的位数不够,请重新输入!\n");
			memset(info->age, 0, sizeof(info->age));
			continue;
		}
		else if(3 == i)
		{
			break;
		}
		i++;
	}
	info->age[i] = '\0';
	printf("rgs_age = %s\n", info->age);

	
	//选择性别
	while(((ch = getchar()) != '\n') && (ch != EOF));
	printf("请选择性别:\n");
	printf("男(m/M)      or      女(f/F)\n");
	while(1)
	{
		
		info->sex[0] = getchar();
		
		printf("info->sex[0] = %c\n", info->sex[0]);
		
		if(('m' != info->sex[0]) && ('M' != info->sex[0]) 
			&& ('f' != info->sex[0]) && ('F' != info->sex[0]))
		{
			printf("输入错误，请重新输入!\n");
			while(((ch = getchar()) != '\n') && (ch != EOF));
			continue;
		}		
		break;
	}
	info->sex[1] = '\0';
	printf("rgs_sex = %s\n", info->sex);
	printf("==================================================\n");
		
	return 0;
}

