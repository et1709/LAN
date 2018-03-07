#include"registered.h"
#include<stdio.h>
#include<string.h>
int dengluxx(struct information *info)
{
	//char login_password[20];
	//char login_account
	int number;
	int account;
	char sh;

	printf("请输入您的账号\n");
	while(1)
	{
		if(scanf("%d", &account) <= 0)
		{
			printf("账号不得有数字以外的字符,请重新输入!\n");
			while((sh = getchar()) != '\n' && sh != EOF);
		}
		
		info -> login_account = account;
		printf("%d\n", info -> login_account);
		break;
	}
	
	printf("%c", info -> login_account);
	printf("请输入登陆密码(您有三次输入密码的机会!):\n");
	for(number = 3; number >= 0; number--)
	{
		system("stty -echo");
		gets(info ->login_password);
		system("stty echo");

		if(strcmp(info -> password,info -> login_password) == 0)
		{
			printf("密码正确!\n");
			break;
		}

		memset(&info -> login_password, 0 ,sizeof(info ->login_password));
		printf("您还有%d次机会:\n",number);

	}


	return 0;
}
