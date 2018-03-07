//协议头文件
#include "mtp.h"

//C库
#include <stdio.h>

//==========================================================
//	函数名称：	MTP_NewBuffer
//
//	函数功能：	申请内存
//
//	入口参数：	mtpPacket：包结构体
//				size：大小
//
//	返回参数：	无
//
//	说明：		1.可使用动态分配来分配内存
//				2.可使用局部或全局数组来指定内存
//==========================================================
void MTP_NewBuffer(MTP_PACKET_STRUCTURE *mtpPacket, int size)
{
	int i;
	
	if(mtpPacket->_data == NULL)//如果数据为空
	{
		mtpPacket->_memFlag = MEM_FLAG_ALLOC;
		
		mtpPacket->_data = (char *)MTP_MallocBuffer(size);
		if(mtpPacket->_data != NULL)
		{
			mtpPacket->_len = 0;
			
			mtpPacket->_size = size;
			
			for(i = 0; i < mtpPacket->_size; i++)
			{
				mtpPacket->_data[i] = 0;
			}
		}
	}
	else
	{
		mtpPacket->_memFlag = MEM_FLAG_STATIC;
		
		for(i = 0; i < )
	}
}