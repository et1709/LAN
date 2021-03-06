int creatNum = 0;
char creatNumStr[10];

/*******************************************************************************
 * 名称: 
 * 形参: 无
 * 返回: 无
 * 说明: 无
 * 使用: 无
*******************************************************************************/
int get_next_qqNum_callback(void *data, int col_count, char **col_values, char **col_Name)
{
	char string[10];
	
	sprintf(string, "%s", col_values[0]);
	creatNum = strtoul(string, NULL, 10);
	
	return 0;
}

/*******************************************************************************
 * 名称: 
 * 形参: 无
 * 返回: 无
 * 说明: 无
 * 使用: int tmp = get_next_qqNum()
*******************************************************************************/
char *get_next_qqNum(void)
{
	int err;
	char *pErrMsg = 0;
	char sql[100] = "select * from qqNum_table";

	if (SQLITE_OK != (err = sqlite3_open("database.db", &db))) {				// 打开数据库
	
		printf("创建数据库 %s 失败(%s)\n", "database.db", sqlite3_errstr(err));
		return NULL;
	}
	
	err = sqlite3_exec(db, sql, get_next_qqNum_callback, 0, &pErrMsg);
    if (err != SQLITE_OK) {
    
		sqlite3_close(db);
        return NULL;
    }
	
	creatNum += 1;																// 读取 QQNum + 1, 写回 qqNum_table
	sprintf(creatNumStr, "%d", creatNum);
	sprintf(sql, "update qqNum_table set qqNum = '%s';", creatNumStr);
	err = sqlite3_exec(db, sql, NULL, NULL, &pErrMsg);
    if (err != SQLITE_OK) {

		sqlite3_close(db);
        return NULL;
    }
	
	sqlite3_close(db);
	
	return creatNumStr;
}
