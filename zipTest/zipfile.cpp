#include "zlib.h"
#include <stdio.h>
#include <string.h>
#include "malloc.h"

int TY_Compress(char * dstZipFile, const char *srcfile)
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
	char *src_buf = (char *)malloc(ufileLength);
	char *dst_buf = (char *)malloc(ufileLength);

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
		return -1;
	}
	fwrite(dst_buf, dst_len, 1, fp_dst);
	fclose(fp_dst);

	free(src_buf);
	free(dst_buf);
	return 0;
}

int TY_UnCompress(char * Destbuf, long len, const char *zipfile)
{
	if (Destbuf == NULL && zipfile == NULL) {
		return -1;
	}
	char *zipBuf;
	FILE* fp_zip;
	FILE* fp4;  //创建解压文件的文件指针
	int ret = 0;
	uLongf dstlen = len;

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

	zipBuf = (char *)malloc(ufileLength);
	//读取文件到buffer
	fread(zipBuf, ufileLength, 1, fp_zip);
	fclose(fp_zip);

	//解压缩buffer中的数据
	ret = uncompress((Bytef*)Destbuf, (uLongf*)&dstlen, (Bytef*)zipBuf, (uLongf)ufileLength);
	if (ret != Z_OK)
	{
		printf("uncompress okay !\n");
		free(zipBuf);
		return -1;
	}

	free(zipBuf);
	return 0;
}

int main(void)
{
	char SrcZipfile[] = "./xx1.zip";
	char srcfile[] = "./xx1.txt";
	char dstzipFile[] = "./dst.zip";
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