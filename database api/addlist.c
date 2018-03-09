/*******************************************************************************
 * 名称: 
 * 功能: 添加好友的时候创建一个与好友的聊天记录 table
 * 形参: 无
 * 返回: 无
 * 说明: chatName：朋友的昵称
*******************************************************************************/
int create_friendchat_table(char *chatName)
{
	int err;
	char sql[100];
	
	memset(&sql, 0, sizeof(sql));
	sprintf(sql, "create table %s_chatTable(qqID TEXT, qqName TEXT, qqChat TEXT, qqTime TEXT)", chatName);
	// 创建一个数据库
	if (SQLITE_OK != sqlite3_exec(db, sql, NULL, NULL, NULL)) {

		// 关闭数据库
		return -1;
	}
	
	return 0;	// OK
}

/*******************************************************************************
 * 名称: 
 * 功能: 添加数据到 userName 用户数据库的 qqlist_table 列表
 * 形参: 无
 * 返回: 无
 * 说明: 无 
*******************************************************************************/
int add_to_qqlist_table(char *userName, char *qqID, char *qqName, char *p_age, char *q_sex)
{
	int err;
	char sql[100];
	char str[100];
	
	memset(&str, 0, sizeof(str));
	sprintf(str, "%s.db", userName);
	// 创建一个数据库
	if (SQLITE_OK != (err = sqlite3_open(str, &db))) {
	
		printf("创建数据库 %s 失败(%s)\n", str, sqlite3_errstr(err));
		return -1;
	}
	
	memset(&sql, 0, sizeof(sql));
	sprintf(sql, "insert into qqlist_table values('%s', '%s', '%s', '%s')", qqID, qqName, p_age, q_sex);
	if (SQLITE_OK != sqlite3_exec(db, sql, NULL, NULL, NULL)) {

		// 关闭数据库
		sqlite3_close(db);
		return -1;
	}
	
	create_friendchat_table(qqName);

	// 关闭数据库
	sqlite3_close(db);
	
	return 0;	// OK
}












typedef struct info
{
	char qqIdStr[10];
	char qqNameStr[10];
	char AgeStr[10];
	char SexStr[10];
} User_Info;
User_Info Check_Info;


/*******************************************************************************
 * 名称: 
 * 功能: 
 * 形参: 无
 * 返回: 无
 * 说明: 无
*******************************************************************************/
int get_qqID_callback(void *data, int col_count, char **col_values, char **col_Name)
{      
	memset(&Check_Info, 0, sizeof(Check_Info));     
	
	sprintf(Check_Info.qqIdStr, "%s", col_values[0]); 
	sprintf(Check_Info.qqNameStr, "%s", col_values[1]); 
	sprintf(Check_Info.AgeStr, "%s", col_values[3]); 
	sprintf(Check_Info.SexStr, "%s", col_values[4]); 
	
	return 0;                               
}                   

/*******************************************************************************
 * 名称: 
 * 功能: 
 * 形参: 无
 * 返回: 无
 * 说明: 无
*******************************************************************************/
User_Info *check_user_table_Info(char *qqID)
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
	if (SQLITE_OK != sqlite3_exec(db, sql, get_qqID_callback, NULL, &pErrMsg)) {
    
		// 关闭数据库
		sqlite3_close(db);
		return NULL;
	}
	
	if (strcmp(Check_Info.qqIdStr, qqID))
		return &Check_Info;
	else
		return NULL;
}

/* 使用方法
User_Info *tmp_Info = check_user_table_Info("13245");
	if (NULL == tmp_Info) {
		
		printf("找不到此用户\n");
	}
	else {
		
		printf("qqID：%-7s", tmp_Info->qqIdStr);
		printf("qqNAME：%-10s", tmp_Info->qqNameStr);
		printf("qqAGE：%-4s", tmp_Info->AgeStr);
		printf("qqSEX：%s\n", tmp_Info->SexStr);
	}
*/















