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
	printf("�����˺�:\n");	
	gets_s(info->login_account, 5);
	
	//���ڴ�������
	printf("��������Ҫ����������:\n");
	system("stty -echo");
	while((ch = getchar() != '\n' && ch != EOF));
	gets_s(info->password, 5);	
	system("stty echo");     

	//�����ǳ�
	printf("��������Ҫ�������س�:\n");
	while((ch = getchar() != '\n' && ch != EOF));
	gets_s(info->nickname, 10);
	
	//printf("%s",info->nickname);  //��ӡ������ǳ�
	
	//ѡ���Ա�
	printf("��ѡ���Ա�:\n");
	printf("m(��)     or      f(Ů)\n");
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
		printf("��������,����������:\n");
	}

	//��������
	printf("�������������:\n");
	while(1)
	{
		while((ch = getchar() != '\n' && ch != EOF));
		if(scanf("%s", info->age) <= 0)
		{
			printf("�����������,����������!\n");		
			while((ch=getchar()) != '\n' && ch!= EOF);
		}       
		break;
	}

	printf("%s,%s,%c,%d\n", info->password, info->nickname,
			info->sex, info->age);
	return 0;
}

