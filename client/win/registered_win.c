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
	gets_s(info->login_account, 5);
	
	//用于创建密码
	printf("请输入需要创建的密码:\n");
	system("stty -echo");
	while((ch = getchar() != '\n' && ch != EOF));
	gets_s(info->password, 5);	
	system("stty echo");     

	//创建昵称
	printf("请输入需要创建的呢称:\n");
	while((ch = getchar() != '\n' && ch != EOF));
	gets_s(info->nickname, 10);
	
	//printf("%s",info->nickname);  //打印输入的昵称
	
	//选择性别
	printf("请选择性别:\n");
	printf("m(男)     or      f(女)\n");
	while(1)
	{
		while((ch = getchar() != '\n' && ch != EOF));
		tmp_sex = getchar();
		if(tmp_sex == 'm' || tmp_sex == 'M' || tmp_sex == 'F' || tmp_sex == 'f')
		{
			info->sex = tmp_sex;
			break;
		}
		//while((ch = getchar() != '\n' && ch != EOF));
		printf("输入有误,请重新输入:\n");
	}

	//创建年龄
	printf("请输入你的年龄:\n");
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

	printf("%s,%s,%c,%d\n", info->password, info->nickname,
			info->sex, info->age);
	return 0;
}

