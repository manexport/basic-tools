#include <stdlib.h>
#include <string.h>
#include "interface.h"
int WTC(unsigned char *utf8_src, unsigned char *dest_gbk, int gbk_len)
{
	wchar_t * lpUnicodeStr = NULL;
	int nRetLen = 0;

	if (!utf8_src)  //如果GBK字符串为NULL则出错退出
		return 0;

	nRetLen = MultiByteToWideChar(CP_UTF8, 0, (char *)utf8_src, -1, NULL, 0);  //获取转换到Unicode编码后所需要的字符空间长度
																			   //lpUnicodeStr = new WCHAR[nRetLen + 1];  //为Unicode字符串空间
	lpUnicodeStr = (WCHAR *)malloc((nRetLen + 1) * sizeof(WCHAR));  //为Unicode字符串空间
	if (lpUnicodeStr == NULL)
		return 0;
	memset(lpUnicodeStr, 0, (nRetLen + 1) * sizeof(WCHAR));
	nRetLen = MultiByteToWideChar(CP_UTF8, 0, (char *)utf8_src, -1, lpUnicodeStr, nRetLen);  //转换到Unicode编码
	if (!nRetLen)  //转换失败则出错退出
		return 0;

	nRetLen = WideCharToMultiByte(CP_ACP, 0, lpUnicodeStr, -1, NULL, 0, NULL, NULL);  //获取转换到UTF8编码后所需要的字符空间长度

	if (!dest_gbk)  //输出缓冲区为空则返回转换后需要的空间大小
	{
		if (lpUnicodeStr)
			//delete []lpUnicodeStr;
			free(lpUnicodeStr);
		return nRetLen;
	}

	if (gbk_len < nRetLen)  //如果输出缓冲区长度不够则退出
	{
		if (lpUnicodeStr)
			//delete []lpUnicodeStr;
			free(lpUnicodeStr);
		return 0;
	}

	nRetLen = WideCharToMultiByte(CP_ACP, 0, lpUnicodeStr, -1, (char *)dest_gbk, gbk_len, NULL, NULL);  //转换到UTF8编码

	if (lpUnicodeStr)
		//delete []lpUnicodeStr;
		free(lpUnicodeStr);

	return nRetLen;
}
int UTF8ToGBK(const char *utf8_src, char *&dest_gbk, int &gbk_len)
{
	USES_CONVERSION;
	//assert(utf8_src);
	if (!utf8_src)
		return 0;
	int nRetLen = 0;
	nRetLen = WTC((unsigned char*)utf8_src, NULL, 0);
	dest_gbk = (char *)malloc((nRetLen + 1) * sizeof(char));
	if (dest_gbk == NULL)
		return 0;
	gbk_len = nRetLen + 1;
	nRetLen = WTC((unsigned char*)utf8_src, (unsigned char*)dest_gbk, nRetLen);
	return nRetLen;
	return 0;
}
void write_data_to_file(char* data, int len)
{
	FILE* data_fd = NULL;
	data_fd = fopen("aaaaa.dat", "ab+");
	if (data_fd) 
	{
		fwrite(data, 1, len, data_fd);
	}
	fclose(data_fd);
	data_fd = NULL;
}
int GBKToUTF8(const char *src_gbk, char *&dest_utf8, int &uft8_len)
{
	if (!src_gbk)
		return 0;

	wchar_t * lpUnicodeStr = NULL;
	int nRetLen = 0;
	nRetLen = ::MultiByteToWideChar(CP_ACP, 0, (char *)src_gbk, -1, NULL, NULL);
	lpUnicodeStr = new WCHAR[nRetLen + 2];
	lpUnicodeStr[nRetLen] = 0;
	lpUnicodeStr[nRetLen + 1] = 0;
	nRetLen = ::MultiByteToWideChar(CP_ACP, 0, (char *)src_gbk, -1, lpUnicodeStr, nRetLen);
	if (!nRetLen)
	{
		delete[] lpUnicodeStr;
		lpUnicodeStr = NULL;
		return 0;
	}
	nRetLen = ::WideCharToMultiByte(CP_UTF8, 0, lpUnicodeStr, -1, NULL, 0, NULL, NULL);

	dest_utf8 = new char[nRetLen + 1];
	uft8_len = nRetLen;
	dest_utf8[uft8_len] = 0;
	nRetLen = ::WideCharToMultiByte(CP_UTF8, 0, lpUnicodeStr, -1, (char *)dest_utf8, uft8_len, NULL, NULL);

	if (lpUnicodeStr)
	{
		delete[]lpUnicodeStr;
		lpUnicodeStr = NULL;
	}
	return nRetLen;
}
