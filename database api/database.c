/*==============================★★★★★====================================*-
-* 文件:
-* 作者:liyan
-* 时间： 
-* 版本：
-* Includes  =================================================================*/
#include <stdio.h>
#include <unistd.h>
#include <string.h"
#include <stdlib.h>

#include "sqlite3.h"
#include "database.h"

/******************************外部全局变量声明********************************/
typedef struct {
	
	char *parmt[5];
} callback;
callback backupdat;

sqlite3 *db;
int creatNum = 0;
char string[10];

/*******************************************************************************
 * 名称: 
 * 功能: 
 * 形参: 无
 * 返回: 无
 * 说明: file_path: 需要判断的文件名
*******************************************************************************/
int is_file_exist(const char *file_path)
{
	if (file_path == NULL) 
		return -1;
	else if (0 == access(file_path, F_OK)) 	
		return 0;
	
	return -1;
}

/*******************************************************************************
 * 名称: 
 * 功能: 在 database.db
 * 形参: 无
 * 返回: 无
 * 说明: 返回 0：成功 -1：失败
*******************************************************************************/
int creat_database(void)
{
	// 定义 sqlite3 指针
	char *pErrMsg = 0;
	char *dbname = "database.db";
	int err;
	const char *table1 = "create table user_table(qqID TEXT, qqNAME TEXT, PASSW TEXT, userAGE TEXT, userSEX TEXT);";
	const char *table2 = "create table qqNum_table(qqNum TEXT);";
	const char *table3 = "insert into qqNum_table values('10000')";
	
	if (-1 == is_file_exist("database.db")) {
		
		// 创建一个数据库
		if (SQLITE_OK != (err = sqlite3_open(dbname, &db))) {
		
			printf("创建数据库 %s 失败(%s)\n", dbname, sqlite3_errstr(err));
			return -1;
		}
		// 执行建表SQL, 创建一个table
		sqlite3_exec(db, table1, NULL, NULL, &pErrMsg);
		if (err != SQLITE_OK) {
			
			fprintf(stderr, "创建数据 table 失败: %s\n", pErrMsg);
			sqlite3_free(pErrMsg);
		}
		// 执行建表SQL, 创建一个table
		sqlite3_exec(db, table2, NULL, NULL, &pErrMsg);
		if (err != SQLITE_OK) {
			
			fprintf(stderr, "创建数据 table 失败: %s\n", pErrMsg);
			sqlite3_free(pErrMsg);
		}
		sqlite3_exec(db, table3, NULL, NULL, &pErrMsg);
		if (err != SQLITE_OK) {
			
			fprintf(stderr, "插入数据失败: %s\n", pErrMsg);
			sqlite3_free(pErrMsg);
		}
	
		// 关闭数据库
		sqlite3_close(db);
	}
	
	return 0;
}

/*******************************************************************************
 * 名称: 
 * 功能: 在 userName 数据库下创建 qqlist_table, mygroup_table, othergroup_table, 
		 friendchat_table 和 groupchat_table
 * 形参: 无
 * 返回: 无
 * 说明: 返回 0：成功 -1：失败 (用户注册的时候用到)
*******************************************************************************/
int creat_userbase(char *userName)
{
	// 定义 sqlite3 指针
	char *pErrMsg = 0;
	char str[100];
	int err;
	const char *table1 = "create table qqlist_table(qqID TEXT, qqNMAE TEXT, userAGE TEXT, userSEX TEXT);";
	const char *table2 = "create table mygroup_table(qqID TEXT, qqNAME TEXT, popNUM TEXT);";
	const char *table3 = "create table othergroup_table(qqID TEXT, qqNAME TEXT, popNUM TEXT);";
	
	memset(&str, 0, sizeof(str));
	sprintf(str, "%s.db", userName);
	if (-1 == is_file_exist(str)) {
		
		// 创建一个数据库
		if (SQLITE_OK != (err = sqlite3_open(str, &db))) {
		
			printf("创建数据库 %s 失败(%s)\n", str, sqlite3_errstr(err));
			return -1;
		}
		// 执行建表SQL, 创建一个table
		sqlite3_exec(db, table1, NULL, NULL, &pErrMsg);
		if (err != SQLITE_OK) {
			
			fprintf(stderr, "创建用户数据 table 失败: %s\n", pErrMsg);
			sqlite3_free(pErrMsg);
		}
		sqlite3_exec(db, table2, NULL, NULL, &pErrMsg);
		if (err != SQLITE_OK) {
			
			fprintf(stderr, "创建用户数据 table 失败: %s\n", pErrMsg);
			sqlite3_free(pErrMsg);
		}
		sqlite3_exec(db, table3, NULL, NULL, &pErrMsg);
		if (err != SQLITE_OK) {
			
			fprintf(stderr, "创建用户数据 table 失败: %s\n", pErrMsg);
			sqlite3_free(pErrMsg);
		}
		
		// 关闭数据库
		sqlite3_close(db);
	}
	
	return 0;
}

/*******************************************************************************
 * 名称: 
 * 功能: 
 * 形参: 无
 * 返回: 无
 * 说明: p_qs：账号 p_qn：昵称 q_paswd：密码 q_sex：性别
*******************************************************************************/
int add_to_database(char *qqID, char *qqName, char *qqPaswd, char *p_age, char *q_sex)
{
	int err;
	char sql[100];
	char *pErrMsg = 0;
	
	// 打开数据库
	if (SQLITE_OK != (err = sqlite3_open("database.db", &db))) {
	
		printf("创建数据库 %s 失败(%s)\n", "database.db", sqlite3_errstr(err));
		return -1;
	}
	
	memset(&sql, 0, sizeof(sql));
	sprintf(sql, "insert into user_table values('%s', '%s', '%s', '%s', '%s')", qqID, qqName, qqPaswd, p_age, q_sex);
	if (SQLITE_OK != sqlite3_exec(db, sql, NULL, NULL, &pErrMsg)) {

		// 关闭数据库
		sqlite3_close(db);
		return -1;
	}
	
	// 新用户注册之后 创建一个用户数据库
	creat_userbase(qqName);
	
	// 关闭数据库
	sqlite3_close(db);
	
	return 0;
}

/*******************************************************************************
 * 名称: 
 * 功能: 
 * 形参: 无
 * 返回: 无
 * 说明: 无
*******************************************************************************/
int get_userPasswd_callback(void *data, int col_count, char **col_values, char **col_Name)
{
	memset(&PasswdStr, 0, sizeof(PasswdStr));
	sprintf(PasswdStr, "%s", col_values[2]);
	
	return 0;
}

/*******************************************************************************
 * 名称: 
 * 功能: 
 * 形参: 无
 * 返回: 无
 * 说明: 无
*******************************************************************************/
char *search_user_table_Passwd(char *qqID)
{
	int err;
	char *pErrMsg = 0;
	char sql[100];
	 
	// 创建一个数据库
	if (SQLITE_OK != (err = sqlite3_open("database.db", &db))) {
	
		printf("创建数据库 %s 失败(%s)\n", "database.db", sqlite3_errstr(err));
		return NULL;
	}
	
	memset(&sql, 0, sizeof(sql));
	sprintf(sql, "select * from user_table where qqID = '%s';", qqID);
	if (SQLITE_OK != sqlite3_exec(db, sql, get_userPasswd_callback, NULL, &pErrMsg)) {

		// 关闭数据库
		sqlite3_close(db);
		return NULL;
	}
	
	return PasswdStr;
}

/*******************************************************************************
 * 名称: 
 * 功能: 
 * 形参: 无
 * 返回: 无
 * 说明: 无
*******************************************************************************/
int get_userQQname_callback(void *data, int col_count, char **col_values, char **col_Name)
{
	memset(&NameStr, 0, sizeof(NameStr));
	sprintf(NameStr, "%s", col_values[1]);
	
	return 0;
}

/*******************************************************************************
 * 名称: 
 * 功能: 
 * 形参: 无
 * 返回: 无
 * 说明: 无
*******************************************************************************/
char *search_user_table_qqName(char *qqID)
{
	int err;
	char *pErrMsg = 0;
	char sql[100];
	 
	// 创建一个数据库
	if (SQLITE_OK != (err = sqlite3_open("database.db", &db))) {
	
		printf("创建数据库 %s 失败(%s)\n", "database.db", sqlite3_errstr(err));
		return NULL;
	}
	
	memset(&sql, 0, sizeof(sql));
	sprintf(sql, "select * from user_table where qqID = '%s';", qqID);
	if (SQLITE_OK != sqlite3_exec(db, sql, get_userQQname_callback, NULL, &pErrMsg)) {

		// 关闭数据库
		sqlite3_close(db);
		return NULL;
	}
	
	return NameStr;
}















/* =============================== 结束 ===================================== */


