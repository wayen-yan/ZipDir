#include <stdio.h>
#include<tchar.h>
#include "ZHead.h"
#include "unzip.h"


int main(void)
{
	char zipFile[MAX_PATH] = { 0 };
	char unzipFile[MAX_PATH] = { 0 };

	CHAR FileDir[MAX_PATH] = _T("");
/*
	DWORD  ret = ZipingDirectory((LPSTR)filePath, (LPSTR)zipPath);
	if (ret != 0) {
		printf("ZipingDirectory error !\n");
		return -1;
	}
	*/
	GetCurrentDirectory(MAX_PATH, FileDir);
	sprintf(zipFile, _T("%s\\%s"), FileDir, "TY-CM-8225A2-V1.zip"); 
	sprintf(unzipFile, _T("%s"), FileDir, "TY-CM-8225A2-V1");
		

	DWORD  ret = UnZipToDirectory((LPSTR)zipFile, (LPSTR)unzipFile);
	if (ret != 0) {
		printf("ZipingDirectory error !\n");
		return -1;
	}
	
	return 0;
}