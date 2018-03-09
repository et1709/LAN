#include<stdio.h>
#include<string.h>
#include"registered.h"

int register_func(struct information *info)
{  
	char tmp_sex;
	char ch, i;
	int tmp_age, len;
	char tem_buf[15];

	// 1: 创建账号
	while((ch = getchar() != '\n') && (ch != EOF));
	//fflush(stdin);
	printf("==================注册输入阶段:===============\n");
	printf("创建账号(5位数):\n");
	while(1)
	{
		fgets(tem_buf, 8, stdin);			
		len = strlen(tem_buf);
		printf("tem_buf = %s\n", tem_buf);
		printf("len = %d\n", len);
		if(len > 6)
		{
			printf("您输入的位数太多,请按enter确定，再重新输入!\n");
			memset(tem_buf, 0, sizeof(tem_buf));
			while((ch = getchar() != '\n') && (ch != EOF));
			//fflush(stdin);
			continue;
		}
		else if('\n' == tem_buf[0])
		{
			memset(tem_buf, 0, sizeof(tem_buf));
			continue;
		}
		else if( len < 5 )
		{
			printf("您输入的位数太少,请重新输入!\n");
			memset(tem_buf, 0, sizeof(tem_buf));
			continue;
		}
		else
		{
			strcpy(info->id, tem_buf);
			break;
		}		
	}
	printf("login_account = %s\n", info->id);

			
	// 2: 创建密码
	//while((ch = getchar() != '\n') && (ch != EOF));
	printf("请输入需要创建的密码(3~5位数):\n");
	while(1)
	{		
		//system("stty -echo");
		fgets(tem_buf, 8, stdin);
		//system("stty echo");		
		len = strlen(tem_buf);
		
		printf("tem_buf = %s\n", tem_buf);
		printf("len = %d\n", len);
		if(len > 6)
		{
			printf("您输入的位数太多,请重新输入!\n");
			memset(tem_buf, 0, sizeof(tem_buf));
			while((ch = getchar() != '\n') && (ch != EOF));
			continue;
		}
		else if( len < 4 )
		{
			printf("您输入的位数太少,请重新输入!\n");
			memset(tem_buf, 0, sizeof(tem_buf));
			continue;
		}
		else
		{
			strcpy(info->password, tem_buf);
			break;
		}
	}		
	printf("password = %s\n", info->password);
	

	// 3: 创建昵称	
	printf("请输入需要创建的呢称(10位以内英文字母):\n");
	while(1)
	{
		
		fgets(tem_buf, 12, stdin);
		len = strlen(tem_buf);
		if(len > 11)
		{
			printf("您输入的位数太多,请重新输入!\n");
			memset(tem_buf, 0, sizeof(tem_buf));
			while((ch = getchar() != '\n') && (ch != EOF));
			continue;
		}
		else
		{
			strcpy(info->nickname, tem_buf);
			break;
		}				
	}
	printf("nickname = %s\n",info->nickname);  //打印输入的昵称


	//创建年龄
	printf("请输入你的年龄(3位数以内):\n");
	while(1)
	{
		fgets(tem_buf, 6, stdin);
		len = strlen(tem_buf);
		
		printf("tem_buf = %s\n", tem_buf);
		printf("len = %d\n", len);
		
		if(len > 4)
		{
			printf("您输入的位数太多,请重新输入!\n");
			memset(tem_buf, 0, sizeof(tem_buf));
			while((ch = getchar() != '\n') && (ch != EOF));
			fflush(stdin);
			continue;
		}
		else if('\n' != tem_buf[0])
		{
			strcpy(info->age, tem_buf);
			break;
		}
	}
	printf("rgs_age = %s\n", info->age);

	
	//选择性别	
	printf("请选择性别:\n");
	printf("男(m/M)      or      女(f/F)\n");
	while(1)
	{				
		info->sex[0] = getchar();	
		//printf("info->sex[0] = %c\n", info->sex[0]);		
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
	fflush(stdin);
	printf("rgs_sex = %s\n", info->sex);
	printf("==================================================\n");
		
	return 0;
}

