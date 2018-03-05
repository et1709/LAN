#include <stdio.h>
#include <unistd.h>
#include "sqlite3.h"
#include "string.h"

sqlite3 *db;

/* 判断数据库是否已经存在 */
int is_file_exist(const char *file_path)
{
	if (file_path == NULL) 
		return -1;
	else if (access(file_path, F_OK) == 0) 	
		return 0;
	
	return -1;
}

/* 创建用户数据库 */
int creat_qqlist(char *name)
{
	char sq3[100];
	char *pErrMsg = 0;
	
	memset(&sq3, 0, sizeof(sq3));
	strcpy(sq3, "create table ");
	strcat(sq3, name); 
	strcat(sq3, "(qs TEXT, qn TEXT)");
	if (SQLITE_OK != sqlite3_exec(db, sq3, 0, 0, &pErrMsg)) {

		return -1;
	}
	
	return 0;
}

/* 创建数据库和用户数据库 */
int add_user(char *p_qs, char *p_qn, char *p_bath, char *q_sex)
{
	char sql[100];
	char *pErrMsg = 0;
	
	memset(&sql, 0, sizeof(sql));
	strcpy(sql, "insert into user values(");
	strcat(sql, p_qs); 
	strcat(sql, ", '");
    strcat(sql, p_qn); // 字符串拼接
	strcat(sql, "', '");
	strcat(sql, p_bath); // 字符串拼接
	strcat(sql, "', ");
	strcat(sql, q_sex); // 字符串拼接
	strcat(sql, ")");
	if (SQLITE_OK != sqlite3_exec(db, sql, NULL, NULL, &pErrMsg)) {

		return -1;
	}
	
	creat_qqlist(p_qn);
	
	return 0;
}

int add_qqlist(char *q_user, char *q_qs, char *q_qn)
{
	char sq2[100];
	
	memset(&sq2, 0, sizeof(sq2));
	strcpy(sq2, "insert into ");
	strcat(sq2, q_user); 
	strcat(sq2, " values('");
	strcat(sq2, q_qs); 
	strcat(sq2, "', '");
    strcat(sq2, q_qn); // 字符串拼接
	strcat(sq2, "')");
	
	if (SQLITE_OK != sqlite3_exec(db, sq2, NULL, NULL, NULL)) {

		return -1;
	}
	
	return 0;
}

int select_callback(void *data, int col_count, char **col_values, char **col_Name)
{
	// 每条记录回调一次该函数,有多少条就回调多少次
	int i;

	for (i=0; i<col_count; i++) {
		
		col_values[i];
	}

	return 0;
}

int read_tables(char *q_tab)
{
	char *pErrMsg = 0;
	char sq4[100];
	
	memset(&sq4, 0, sizeof(sq4));
	strcpy(sq4, "select * from ");
	strcat(sq4, q_tab); 
	
	if (-1 == sqlite3_exec(db, sq4, select_callback, 0, &pErrMsg)) {
		
		return -1;
	}
	
	return 0;
}

int read_specify(char *q_tab, char *q_qs) 
{
	char *pErrMsg = 0;
	char sq5[100];
	
	memset(&sq5, 0, sizeof(sq5));
	strcpy(sq5, "select * from ");
	strcat(sq5, q_tab); 
	strcat(sq5, "where qs='"); 
	strcat(sq5, q_qs);
	strcat(sq5, "'");
	if (-1 == sqlite3_exec(db, sq5, NULL, NULL, &pErrMsg)) {
		
		return -1;
	}
	
	return 0;
}



int main(void)
{
	// 定义 sqlite3 指针
	char *pErrMsg = 0;
	char *dbname = "mydata.db";
	int err, i;
	char *sql[5];
	const char * sSQL1 = "create table user(qs TEXT, qn TEXT, bath TEXT, sex INTEGER);";
	
	if (-1 == is_file_exist("mydata.db")) {
		
		// 创建一个数据库
		if (SQLITE_OK != (err = sqlite3_open(dbname, &db))) {
		
			printf("创建数据库 %s 失败(%s)\n", dbname, sqlite3_errstr(err));
			return -1;
		}
		// 执行建表SQL
		sqlite3_exec(db, sSQL1, 0, 0, &pErrMsg);
		if (err != SQLITE_OK) {
			
			fprintf(stderr, "创建用户数据库失败: %s\n", pErrMsg);
			sqlite3_free(pErrMsg);
		}
	}
	else {
		
		// 打开一个数据库
		if (SQLITE_OK != (err = sqlite3_open(dbname, &db))) {
		
			printf("创建数据库 %s 失败(%s)\n", dbname, sqlite3_errstr(err));
			return -1;
		}
	}
	
	// if (-1 == read_specify("user1") ) 
		// printf("fail\n");
	// else
		// printf("ok\n");
	
	
	// 执行 sql 语句
	add_user("1378139998", "xiaoqi", "200.1.2", "21");
	add_user("1378139991", "peiqi", "2001.2.3", "24");
	add_user("1378139992", "yujuan", "2002.3.4", "23");
	
	add_qqlist("peiqi", "1378139998", "liyan1");
	add_qqlist("peiqi", "1378139997", "liyan2");
	add_qqlist("peiqi", "1378139996", "liyan3");
	
	// creat_qqlist("liyan");
	// creat_qqlist("xiaoqing");
	
_out:
	// 关闭数据库
	sqlite3_close(db);
 
	return 0;
}

