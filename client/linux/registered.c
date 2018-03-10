#include<stdio.h>
#include<string.h>
#include"registered.h"

//从键盘获取一个整数
int get_integer(void)
{
	int num;
	char ch;
		
	while(scanf("%d", &num) < 0)
	{
		printf("Input error, Please enter an integer!\n");
		while((ch = getchar() != '\n') && (ch != EOF));			
	}
	while((ch = getchar() != '\n') && (ch != EOF));

	return num;
}

//从键盘获取size以内个字符
void get_string(char *buf, int size)
{
	int i = 0, len = size;
	char ch;
	while(1)
	{
		//printf("Please enter %d characters: ", len);
		ch = getchar();
		if('\n' == ch)
		{
			continue;
		}
		while(('\n' != ch) && (EOF != ch) && (0 != len))
		{			
			buf[i] = ch;
			ch = getchar();
			i++;
			len--;
		}
		buf[i] = '\0';
		if('\n' != ch)
		{
			printf("输入字符超出 %d 个, 已截取前 %d 个!\n", size, size);
			while(((ch = getchar()) != '\n') && (ch != EOF));
		}
		break;
	}	
}

//从键盘获取size个字符
void get_size_string(char *buf, int size)
{
	int i = 0, len = size;
	char ch;
	while(1)
	{
		//printf("Please enter %d characters: ", len);
		ch = getchar();
		if('\n' == ch)
		{
			continue;
		}
		while(('\n' != ch) && (EOF != ch) && (0 != len))
		{			
			buf[i] = ch;
			ch = getchar();
			i++;
			len--;
		}
		if(0 != len)
		{
			printf("The number of input characters is not enough. Please re-enter!\n");
			memset(buf, 0, sizeof(char) * size);
			len = size;
			i = 0;
			continue;
		}
		buf[i] = '\0';
		if('\n' != ch)
		{
			printf("输入字符超出 %d 个, 已截取前 %d 个!\n", size, size);
			while(((ch = getchar()) != '\n') && (ch != EOF));
		}
		break;
	}	
}

//从键盘获取一个字符 
char get_char(void)
{
	char ch, temp;
	printf("Please enter one characters: ");
	while(ch = getchar())
	{
		if('\n' == ch)
		{
			continue;
		}
		while(('\n' != (temp = getchar())) && (EOF != temp));
		break;
	}
	return ch;
}

//输入注册信息
int register_func(struct information *info)
{  
	int i;
	int flag = 0;
/*
	// 1: 创建账号
	printf("==================注册输入阶段:===============\n");
	printf("创建账号(5位数):\n");
	get_size_string(info->id, 5);	
*/

	// 3: 创建昵称	
	printf("请输入需要创建的呢称:\n");
	get_string(info->nickname, 5);


	// 2: 创建密码
	printf("请输入需要创建的密码(5位数):\n");
	get_size_string(info->password, 5);


	//创建年龄
	printf("请输入你的年龄(3位数以内):\n");
	while(1)
	{
		flag = 0;
		get_string(info->age, 3);
		//printf("info->age = %s\n", info->age);
		for(i = 0; i < 3; i++)
		{
			if('\0' == info->age[i])
			{
				break;
			}
			if(info->age[i] < '0' || info->age[i] > '9')
			{
				printf("info->age[%d] = %c\n", i, info->age[i]);
				printf("Input error, Please enter the number!\n");
				memset(info->age, 0, sizeof(info->age));
				flag = 1;
				break;
			}
		}
		if(!flag)
		{
			break;
		}
	}
	//printf("info->age = %s\n", info->age);
		
	
	//选择性别	
	printf("请选择性别:\n");
	printf("男(m/M)      or      女(f/F)\n");
	while(1)
	{	
		info->sex[0] = get_char();	
		if(('m' != info->sex[0]) && ('M' != info->sex[0])
			&& ('f' != info->sex[0]) && ('F' != info->sex[0]))
		{
			printf("Input error，Please re-enter!\n");
			continue;
		}
		break;
	}
	info->sex[1] = '\0';
		
	return 0;
}

//输入登陆消息
void login_func(struct information *info)
{
	printf("请输入您的账号(5位数):\n");
	get_size_string(info->id, 5);
	//printf("%c", info->id);

	printf("请输入登陆密码(5)位数:\n");
	//system("stty -echo");
	get_size_string(info->password, 5);
	//system("stty echo");	
}


