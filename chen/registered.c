#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"registered.h"

/*void safe_flush(FILE *fp)
  {
  int ch;
  while((ch = fagetc(fp)) != EOF && ch != '\n');

  }*/
struct information nap;


/*int main(void)
{  
    char ass[20];
	struct information;
	int i;
	printf("请输入需要创建的密码:");
	system("stty -echo");
	gets(nap.password);
	system("stty echo");      
	printf("%s", nap.password);
	printf("请输入密码登陆:");
	gets(ass);
	i = strcmp(ass, nap.password);
	if(i ==  0)
	{
	  printf("Yes!");
	}
	else
	{
	  printf("No!\n");
	}

	return 0;
}

int name()
{
	struct information;
	printf("请输入需要创建的呢称:");
	gets(nap.nickname);
	printf("%s",nap.nickname);  
	return ;  
}*/

int main(void)
{
	struct information;
	char o;
	printf("请选择性别:");
	printf("m(男)     or      f(女)\n");
set:
	o = gerchar();
	if(o == 'm' || o == 'M' || o == 'F' || o == 'f')
	{

		gender = o;
	}
	else
	{

		goto set;

	}

	return 0;

}

/*int age()
{
	int a;
	char ch;
	while(1)
	{
		while(scanf("%d", &a)  <= 0)
		{
			printf("你的输入有误,请重新输入!\n");		
			while((ch=getchar()) != '\n' && ch!= EOF);
		}
		break;
	}
	printf("a = %d\n", a);

	return ;
}*/
