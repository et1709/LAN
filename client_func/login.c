#include"client_func.h"


void login_func(AGREEMENT *data, struct information *info, int flag)
{
	while((ch = getchar() != '\n' && ch != EOF)); //清空缓存
	
	if(flag == 3)
	{
		printf("请输入您的账号：\n");
		fgets(info->id, 6, stdin);
		stpcpy(data->mine_id, info->id);
		
		printf("请输入登陆密码：\n");
		fgets(info->password, 6, stdin);
		stpcpy(data->password, info->password);
	}
	else if(flag >= 0)
	{
		printf("请重新输入登陆密码(剩余%d次机会)\n", flag);
		fgets(info->password, 6, stdin);
		stpcpy(data->password, info->password);
	}
}
