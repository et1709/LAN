/*******************************************************************************
 * 名称: 
 * 形参: 无
 * 返回: 无
 * 说明: 无
 * 使用: 无
*******************************************************************************/
int main(void)
{
	callback *str;
	char *strPoit;
	QqList_Phead = Init_Kernel_List1();
	GroupInfo_Phead = Init_Kernel_List2();
	
	// (创建一个数据库存放所以用户的注册信息) database.db
	creat_database();
	
	// 想 database.db 数据库添加 用户信息同时给注册的用户创建一个用户数据库
	// liyan.db(qqlist_table, mygroup_table, othergroup_table)
	add_to_database("13245", "liyan", "11122", "21", "F");	
	add_to_database("13247", "lidao", "11123", "27", "M");
	
	// 向用户数据库的 好友列表 添加好友信息同时创建一个和该好友的聊天记录 table
	// qqlist_table(xiaoqi|1378139999|peiqi|123455441|21|F) peiqi_chatTable
	add_to_qqlist_table("13245", "16926", "yujuan", "18", "F");
	add_to_qqlist_table("13245", "18689", "peiqi", "21", "F");
	add_to_qqlist_table("13245", "12152", "xiaoqing", "21", "F");
	
	// 向该用户自己的 mygroup_table 添加群信息同时创建一个群成员 table 和 群聊天记录 table
	// mygroup_table(294583762|qq_linux|8) qq_linux_listTable qq_linux_chatTable
	add_to_mygroup_table("13245", "29458", "qq_linux", "8");
	
	// othergroup_table(xiaoqi|154583797|qq_window|9) qq_window_listTable qq_window_chatTable
	add_to_othergroup_table("13245", "15458", "qq_window", "9");
	
	// 向该用户的(谁)的聊天记录 table 添加数据
	add_to_friendchat_table("13245", "peiqi", "87433", "peiqi", "ni hao!", "9:00");
	
	// 记录该用户的(群)里的(谁) 的聊天信息
	add_to_groupchat_table("13245", "qq_linux", "43347", "peiqi", "ni hao!", "9:00");
	
	// 向 liyan 的 q 群 qq_linux 添加成员
	add_to_grouplist_table("13245", "qq_linux", "87433", "peiqi");
	
	// // 
	
	
	printf("---------------------------------------------------------\n");
	// 读取 liyan 的 qqlist_table 下的 1378139997 这个好友的信息
	str = read_qqlist_table("13245", "qqlist_table", "16926");
	printf("qqID: %s\n", str->parmt[0]);
	printf("qqNAME: %s\n", str->parmt[1]);
	printf("userAGE: %s\n", str->parmt[2]);
	printf("userSEX: %s\n", str->parmt[3]);
	printf("---------------------------------------------------------\n");
	printf("qqNum is: %s\n", get_next_qqNum());
	printf("---------------------------------------------------------\n");
	printf("PASSW is: %s\n", search_user_table_Passwd("13245"));
	printf("---------------------------------------------------------\n");
	printf("qqMAME is: %s\n", search_user_table_qqName("13245"));
	printf("---------------------------------------------------------\n");
	Display_Kernel_list1(get_qqList_table_info("13245"));
	Del_Kernel_list();
	printf("hello 7\n");
	//Display_Kernel_list2(get_qqgroup_table_info("13245", "qq_linux"));
	printf("hello 6\n");
	printf("---------------------------------------------------------\n");
	printf("hello 5\n");
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
	
	
_out:
	// 关闭数据库
	// sqlite3_close(db);
 
	return 0;
}
