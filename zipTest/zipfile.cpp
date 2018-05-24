#include "zlib.h"
#include <stdio.h>
#include <string.h>
#include "malloc.h"
#include <string>
#include <iostream>
#include<vector>
#include <direct.h>  
#include <io.h>  
#include "ty_type.h"

using namespace std;

TY_S32 TY_Compress(TY_CHAR * dstZipFile, const TY_CHAR *srcfile)
{
	if (dstZipFile == NULL && srcfile == NULL) {
		return -1;
	}

	FILE *fp_src = fopen(srcfile, "rb");
	if (NULL == fp_src) {
		printf("fopen error !\n");
		return -1;
	}

	long ucur = ftell(fp_src);
	fseek(fp_src, 0L, SEEK_END);
	long ufileLength = ftell(fp_src);
	fseek(fp_src, ucur, SEEK_SET);
	unsigned char *src_buf = (unsigned char *)malloc(ufileLength);
	unsigned char *dst_buf;
	if (ufileLength < 256) {
		dst_buf = (unsigned char *)malloc(256);
	}
	else {
		dst_buf = (unsigned char *)malloc(ufileLength);
	}

	// 读取文件到buffer
	fread(src_buf, ufileLength, 1, fp_src);
	fclose(fp_src);
	uLongf dst_len;
	int ret = compress((Bytef*)dst_buf, &dst_len, (Bytef*)src_buf, ufileLength);
	if (ret != Z_OK)
	{
		printf("compress okay !\n");
		free(src_buf);
		free(dst_buf);
		return -1;
	}

	FILE *fp_dst = fopen(dstZipFile, "wb+");
	if (NULL == fp_dst) {
		printf("fopen error !\n");
		free(src_buf);
		free(dst_buf);
		return -1;
	}
	fwrite(dst_buf, dst_len, 1, fp_dst);
	fclose(fp_dst);

	free(src_buf);
	free(dst_buf);
	return 0;
}

TY_S32 TY_UnCompress(TY_U8 * Destbuf, TY_U64 *len, const TY_CHAR *zipfile)
{
	if (Destbuf == NULL && zipfile == NULL) {
		return -1;
	}
	unsigned char *zipBuf;
	FILE* fp_zip;
	int ret = 0;

	fp_zip = fopen(zipfile, "rb");
	if (NULL == fp_zip) {
		printf("fopen error !\n");
		return -1;
	}

	//获取欲解压文件的大小
	long ucur = ftell(fp_zip);
	fseek(fp_zip, 0L, SEEK_END);
	long ufileLength = ftell(fp_zip);
	fseek(fp_zip, ucur, SEEK_SET);

	zipBuf = (unsigned char *)malloc(ufileLength);
	//读取文件到buffer
	fread(zipBuf, ufileLength, 1, fp_zip);
	fclose(fp_zip);

	//解压缩buffer中的数据
	ret = uncompress((Bytef*)Destbuf, (uLongf *)len, (Bytef*)zipBuf, (uLongf)ufileLength);
	if (ret != Z_OK)
	{
		printf("uncompress error !\n");
		free(zipBuf);
		return -1;
	}

	free(zipBuf);
	return 0;
}

/*
@function: 获取cate_dir目录下的所有文件名
@param: cate_dir - string类型
@result：vector<string>类型
*/

vector<string> getFiles(string cate_dir)
{
	vector<string> files;//存放文件名  

#ifdef _WIN32  
	_finddata_t file;
	long lf;
	//输入文件夹路径  
	if ((lf = _findfirst(cate_dir.c_str(), &file)) == -1) {
		cout << cate_dir << " not found!!!" << endl;
	}
	else {
		while (_findnext(lf, &file) == 0) {
			//输出文件名  
			//cout<<file.name<<endl;  
			if (strcmp(file.name, ".") == 0 || strcmp(file.name, "..") == 0)
				continue;
			files.push_back(file.name);
		}
	}
	_findclose(lf);
#endif  

#ifdef linux  
	DIR *dir;
	struct dirent *ptr;
	char base[1000];

	if ((dir = opendir(cate_dir.c_str())) == NULL)
	{
		perror("Open dir error...");
		exit(1);
	}

	while ((ptr = readdir(dir)) != NULL)
	{
		if (strcmp(ptr->d_name, ".") == 0 || strcmp(ptr->d_name, "..") == 0)    ///current dir OR parrent dir  
			continue;
		else if (ptr->d_type == 8)    ///file  
									  //printf("d_name:%s/%s\n",basePath,ptr->d_name);  
			files.push_back(ptr->d_name);
		else if (ptr->d_type == 10)    ///link file  
									   //printf("d_name:%s/%s\n",basePath,ptr->d_name);  
			continue;
		else if (ptr->d_type == 4)    ///dir  
		{
			files.push_back(ptr->d_name);
			/*
			memset(base,'\0',sizeof(base));
			strcpy(base,basePath);
			strcat(base,"/");
			strcat(base,ptr->d_nSame);
			readFileList(base);
			*/
		}
	}
	closedir(dir);
#endif  

	return files;
}
#if 1
int UnCompress_CurDirFiles(void)
{
	char current_address[100];
	char srcFileName[256] = { 0 };
	char dstfileName[256] = { 0 };
	char AbsolutePath[128] = { 0 };
	int OutWidth = 17712;
	int OutHeight = 2732;
	uLongf len = OutWidth*OutHeight * sizeof(short);
	short *OutBuf = (short *)malloc(OutWidth*OutHeight * sizeof(short));

	memset(current_address, 0, 100);
	getcwd(current_address, 100); //获取当前路径  
	strcat(current_address, "\\TY-CM-8325A2-V1_ZIP\\MagData");
	strcpy(AbsolutePath, current_address);
	strcat(AbsolutePath, "\\");
	cout << AbsolutePath << endl;
	strcat(current_address, "\\*");
	cout << current_address << endl;
	const char *fileName;
	uLongf dstlen;

	char *p;
	int i;

	vector<string> files = getFiles((string)current_address);
	for (i = 0; i < files.size(); i++)
	{
		fileName = files[i].data();
		strcpy(srcFileName, AbsolutePath);
		strcat(srcFileName, fileName);
		strcpy(dstfileName, srcFileName);
		p = strstr(dstfileName, ".zip");
		if (p != NULL) {
			memset(p, 0, sizeof(".zip"));
			strcpy(p, ".txt");
			printf("%s\n", dstfileName);
			dstlen = len;
			int ret = TY_UnCompress((unsigned char *)OutBuf, (TY_U64 *)&dstlen, srcFileName);
			if (ret != 0) {
				printf("TY_UnCompress error !\n");
				free(OutBuf);
				return -1;
			}

			FILE *fp = fopen(dstfileName, "wb+");
			if (NULL == fp) {
				free(OutBuf);
				return -1;
			}

			fwrite(OutBuf, dstlen, 1, fp);
			fclose(fp);
		}
		memset(srcFileName, 0, sizeof(srcFileName));
		memset(dstfileName, 0, sizeof(dstfileName));
	}
	printf("i === %d\n", i);

	cout << "end..." << endl;
	free(OutBuf);
	cin.get();
	return 0;
}

int Compress_CurDirFiles(void)
{
	char current_address[100];
	char srcFileName[256] = { 0 };
	char dstfileName[256] = { 0 };
	char AbsolutePath[128] = { 0 };

	memset(current_address, 0, 100);
	getcwd(current_address, 100); //获取当前路径  
	//strcat(current_address, "\\TY-CM-2106A3-V1\\seamline");
	strcpy(AbsolutePath, current_address);
	strcat(AbsolutePath, "\\");
	cout << AbsolutePath << endl;
	strcat(current_address, "\\*");
	cout << current_address << endl;
	const char *fileName;
	
	char *p;
	int i;

	vector<string> files = getFiles((string)current_address);
	for ( i = 0; i < files.size(); i++ )
	{
		//cout << files[i] << endl;
		fileName = files[i].data();
		strcpy(srcFileName, AbsolutePath);
		strcat(srcFileName, fileName);
		strcpy(dstfileName, srcFileName);
		p = strstr(dstfileName, ".txt");
		if( p != NULL ) {
			memset(p, 0, sizeof(".txt"));
			strcpy(p, ".zip");
			printf("%s\n", dstfileName);

			int ret = TY_Compress(dstfileName, srcFileName);
			if (ret != 0) {
				printf("TY_Compress error !\n");
				return -1;
			}
		}
		memset(srcFileName, 0, sizeof(srcFileName));
		memset(dstfileName, 0, sizeof(dstfileName));
	}
	printf("i === %d\n", i);

	cout << "end..." << endl;
	//cin.get();
	return 0;
}

int main(void)
{
	Compress_CurDirFiles();
	return 0;
}

#else
int main(void)
{
	char srcfile[] = "./HW.txt";
	char dstzipFile[] = "./HW.zip";
	int OutWidth = 16992;
	int OutHeight = 1748;
	long len = OutWidth*OutHeight * sizeof(short);
	short *xx1 = (short *)malloc(OutWidth*OutHeight * sizeof(short));

	int ret = TY_Compress(dstzipFile, srcfile);
	if (ret != 0) {
		printf("TY_Compress error !\n");
		return -1;
	}

	ret = TY_UnCompress((char *)xx1, len, dstzipFile);
	if (ret != 0) {
		printf("TY_UnCompress error !\n");
		return -1;
	}
	FILE *fp = fopen("./after_uncompress.txt", "wb+");
	if (NULL == fp) {
		return -1;
	}

	fwrite(xx1, len, 1, fp);
	return 0;
}
#endif