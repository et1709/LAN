#include"registered.h"
#include"function.h"

void login_func(struct information *info)
{
	char ch;
	while(((ch = getchar()) != '\n') && (ch != EOF));
	printf("请输入您的账号(5位数):\n");
	gets(info->id);	
	//printf("%c", info->id);

	while(((ch = getchar()) != '\n') && (ch != EOF));
	printf("请输入登陆密码(5)位数:\n");
	//system("stty -echo");
	gets(info->password);
	//system("stty echo");
	
}
