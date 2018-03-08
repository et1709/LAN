#include"registered.h"
#include"function.h"

void login_func(AGREEMENT *data, struct information *info, int flag)
{
	char ch;
	printf("进入login_func\n");
	setbuf(stdin, NULL);
	printf("=====flag = %d\n", flag);
	if(flag == 3)
	{
		printf("请输入您的账号：\n");
		fgets(info->id, 6, stdin);
		strcpy(data->mine_id, info->id);
		strcpy(buffer, info->id);
		setbuf(stdin, NULL);
		
		printf("请输入登陆密码：\n");
		fgets(info->password, 6, stdin);
		strcpy(data->password, info->password);
		printf("=======data->password %s\n", data->password);
		setbuf(stdin, NULL);
	}
	else if(flag >= 0)
	{
		printf("请重新输入登陆密码(剩余%d次机会)\n", flag);
		fgets(info->password, 6, stdin);
		strcpy(data->password, info->password);
		strcpy(data->mine_id, buffer);
		
	}
	else {
		printf("请重新注册或者登陆");
		login_mark = 3;
	}
}
