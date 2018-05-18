#ifndef __ZHead__ 
#define __ZHead__
 
#pragma once
#include <stdio.h>
#include <Windows.h>

/* 
���ܣ���ָ����Ŀ¼ѹ��
������
  1.LPSTR lpszDirectory ��ѹ����Ŀ¼
     "C:\\Program Files\\OfficeIM�������ܰ칫�ͻ���\\���յ����ļ�\\SogouInput"
	 
  2.LPSTR lpszZipPathName ѹ���ļ���
     "d:\\aa.zip"
���أ�0 �ɹ� ����������ֵ
ע�⣺ָ�������У��Ϸ���
*/
DWORD  WINAPI ZipingDirectory( LPSTR lpszDirectory, LPSTR lpszZipPathName ); 


/*
���ܣ���ѹ���ļ���ѹ��ָ��Ŀ¼
������
  1.LPSTR lpszZipPathName ѹ���ļ���
      "d:\\aa.zip"

  2.LPSTR lpszDestDirectory ��ѹ��Ŀ��Ŀ¼
      "f:\\aa\\cc"
���أ�0 �ɹ� ����������ֵ
ע�⣺ָ�������У��Ϸ���
*/
DWORD WINAPI UnZipToDirectory( LPSTR lpszZipPathName, LPSTR lpszDestDirectory );

#endif // __ZHead__  
