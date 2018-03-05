#include<stdio.h>
#include<string.h>
#include<stdlib.h>

struct information
{
    
    char nickname[20];
    char password[20];
    char gender[20];
    char age[20];
   
}nap;


int pass()
{  
      
      struct information;
      printf("请输入需要创建的密码:");
      system("stty -echo");
      gets(nap.password);
      system("stty echo");      
      printf("%s", nap.password);
     
      return;
}

int name()
{
   struct information;
   printf("请输入需要创建的呢称:");
   gets(nap.nickname);
   printf("%s",nap.nickname);  
   return ;  
}

int main(void)
{
    struct information;
    printf("请输入性别:");
    


}

