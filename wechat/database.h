#ifndef __DATABASE_H__
#define __DATABASE_H__


/* 宏定义 *********************************************************************/

typedef struct dataInfo
{
	char qqIdStr[10];
	char qqNameStr[10];
	char AgeStr[10];
	char SexStr[10];
} User_Info;


/******************************全局变量声明************************************/
extern int creatNum;
extern char PasswdStr[10];
extern char NameStr[10];
extern User_Info Check_Info;
extern int creatNum;
extern char creatNumStr[10];

/******************************函数声明****************************************/
int creat_database(void);
int add_to_database(char *qqID, char *qqName, char *qqPaswd, char *p_age, char *q_sex);
char *search_user_table_Passwd(char *qqID);
char *search_user_table_qqName(char *qqID);
int create_friendchat_table(char *chatName);
int add_to_qqlist_table(char *userName, char *qqID, char *qqName, char *p_age, char *q_sex);
int get_next_qqNum_callback(void *data, int col_count, char **col_values, char **col_Name);
char *get_next_qqNum(void);
User_Info *check_user_table_Info(char *qqID);

#endif

/* =============================== 结束 ===================================== */

