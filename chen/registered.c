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

	//用于创建密码
	printf("请输入需要创建的密码:\n");
	system("stty -echo");
	gets(info->password);
	system("stty echo");     

	//创建昵称
	printf("请输入需要创建的呢称:\n");
	gets(info->nickname);
	printf("%s",info->nickname);  //打印输入的昵称
	
	//选择性别
	printf("请选择性别:\n");
	printf("m(男)     or      f(女)\n");
	while(1)
	{
		tmp_sex = getchar();
		while((ch = getchar() != '\n' && ch != EOF));
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
		if(scanf("%d", &tmp_age) <= 0)
		{
			printf("你的输入有误,请重新输入!\n");		
			while((ch=getchar()) != '\n' && ch!= EOF);
		}
        //成功输入
		info->age = tmp_age;
		break;
	}

	printf("%s,%s,%c,%d\n", info->password, info->nickname,
			info->sex, info->age);
	return 0;
}

