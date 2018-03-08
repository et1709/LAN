#include<stdio.h>
#include<string.h>
#include"registered.h"

/*void safe_flush(FILE *fp)
  {
  int ch;
  while((ch = fagetc(fp)) != EOF && ch != '\n');

  }*/


int register_func(struct information *info)
{  
	char tmp_sex;
	char ch;
	int tmp_age;

	while((ch = getchar() != '\n' && ch != EOF));
	printf("创建账号:\n");	
	fgets(info->login_account, 6, stdin);
	
	//用于创建密码
	printf("请输入需要创建的密码:\n");
	system("stty -echo");
	while((ch = getchar() != '\n' && ch != EOF));
	fgets(info->password, 6, stdin);
	system("stty echo");     

	//创建昵称
	printf("请输入需要创建的呢称:\n");
	while((ch = getchar() != '\n' && ch != EOF));
	fgets(info->nickname, 6, stdin);
	
	//printf("%s",info->nickname);  //打印输入的昵称

	//创建年龄
	printf("请输入你的年龄:\n");
	fgets(info->age, 4, stdin);
	/*
	while(1)
	{
		while((ch = getchar() != '\n' && ch != EOF));
		
		if(scanf("%s", info->age) <= 0)
		{
			printf("你的输入有误,请重新输入!\n");		
			while((ch=getchar()) != '\n' && ch!= EOF);
		}       
		break;
	}
	//printf("%s,%s,%c,%d\n", info->password, info->nickname,
			//info->sex, info->age);
	*/
	
	//选择性别
	printf("请选择性别:\n");
	printf("男(m/M)    or      女(f/F)\n");
	while(1)
	{			
		while((ch = getchar() != '\n' && ch != EOF));
		fgets(info->sex, 2, stdin);
		if('m' == info->sex[0] || 'M' == info->sex[0]
			|| 'f' == info->sex[0] || 'F' == info->sex[0])
		{			
			break;
		}
		printf("输入错误，请重新输入:\n");
	}

		
	return 0;
}

