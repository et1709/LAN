#include"registered.h"
#include<stdio.h>
#include<string.h>
int dengluxx(struct information *info)
{
	int number;
	int account;
	char sh, ch;

	while((ch = getchar() != '\n' && ch != EOF));
	printf("�����������˺�\n");
	gets_s(info->login_account, 5);
		
	//printf("%c", info->login_account);
	printf("�������½����(����������������Ļ���!):\n");
	for(number = 3; number >= 0; number--)
	{
		system("stty -echo");
		while((ch = getchar() != '\n' && ch != EOF));
		gets_s(info->login_password, 5);
		system("stty echo");

		if(strcmp(info->password, info->login_password) == 0)
		{
			//printf("������ȷ!\n");
			break;
		}

		memset(&info->login_password, 0, sizeof(info ->login_password));
		printf("������%d�λ���:\n", number);

	}
	
	return 0;
}
