/*==============================★★★★★====================================*-
-* 文件:
-* 作者:liyan
-* 时间： 
-* 版本：
-* ===========================================================================*/

#ifndef __DATABASE_H__
#define __DATABASE_H__


/* 宏定义 *********************************************************************/



/******************************全局变量声明************************************/
extern sqlite3 *db;
extern int creatNum;
extern char PasswdStr[10];
extern char NameStr[10];

/******************************函数声明****************************************/
int creat_database(void);
int add_to_database(char *qqID, char *qqName, char *qqPaswd, char *p_age, char *q_sex);
char *search_user_table_Passwd(char *qqID);
char *search_user_table_qqName(char *qqID);


#endif

/* =============================== 结束 ===================================== */
