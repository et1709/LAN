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
	fgets(info->login_account, 6, stdin);
	
	//���ڴ�������
	printf("��������Ҫ����������:\n");
	system("stty -echo");
	while((ch = getchar() != '\n' && ch != EOF));
	fgets(info->password, 6, stdin);
	system("stty echo");     

	//�����ǳ�
	printf("��������Ҫ�������س�:\n");
	while((ch = getchar() != '\n' && ch != EOF));
	fgets(info->nickname, 6, stdin);
	
	//printf("%s",info->nickname);  //��ӡ������ǳ�

	//��������
	printf("�������������:\n");
	fgets(info->age, 4, stdin);
	/*
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
	//printf("%s,%s,%c,%d\n", info->password, info->nickname,
			//info->sex, info->age);
	*/
	
	//ѡ���Ա�
	printf("��ѡ���Ա�:\n");
	printf("��(m/M)    or      Ů(f/F)\n");
	while(1)
	{			
		while((ch = getchar() != '\n' && ch != EOF));
		fgets(info->sex, 2, stdin);
		if('m' == info->sex[0] || 'M' == info->sex[0]
			|| 'f' == info->sex[0] || 'F' == info->sex[0])
		{			
			break;
		}
		printf("�����������������:\n");
	}

		
	return 0;
}

