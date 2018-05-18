#include "ZHead.h"
#include ".\XZip\XUnzip.h"
#include ".\XZip\XZip.h "
#include <string>
#include <list>
#include <tchar.h>
using namespace std;

 
/*
���ܣ�����Ŀ¼·�����ɶ�Ӧ��Ŀ¼
*/ 
BOOL CreateDirectorys( LPCSTR lpszPath )
{    
    WIN32_FIND_DATA fd; 
    HANDLE hFind ;

	hFind = FindFirstFile( lpszPath, &fd); 

    if (  hFind == INVALID_HANDLE_VALUE ) 		 
	{
		list< string > dirStack;
		
		string tmp = lpszPath;	
	 
		dirStack.push_front( tmp );
		
		int i = tmp.rfind( _T("\\") );	 
		while( i > 0 )
		{			
			tmp = tmp.substr( 0, i );
			dirStack.push_front( tmp );
			i = tmp.rfind( _T("\\"), i );		
		}
		
		//��Ŀ¼��ʽ����
		if( i < 0 && ( _T(":") != tmp.substr( tmp.length() - 1, 1 )) )
		{  
			FindClose( hFind ) ; 		    
		  	return FALSE ;		 
		}		
		try
		{
			list<string>::iterator it = dirStack.begin();

			for( ; it != dirStack.end(); it++ )
			{				 
				::CreateDirectory( ( *it ).c_str(), NULL );
			}
		}
		catch(...)
		{ 		 
		 	FindClose( hFind ); 			 
			return FALSE;	 		
		} 
	}    

	FindClose( hFind ); 

	return TRUE;
}
 

//�ļ�Ŀ¼��ʽ��
void FormatDirectorys( LPSTR lpszDirectorys )
{
	if( lpszDirectorys!= NULL )
	{
		int i=0;

		while( lpszDirectorys[ i ] !=  _T('\0') ) 
		{
			if( lpszDirectorys[ i ] == _T('/') ) 
			{   
				lpszDirectorys[ i ] = _T('\\');			
			}

			i++;
		}
	}	
}

/*
   ���ܣ���ɾ����Ŀ¼ѹ������
   ���أ�0 �ɹ�����������
*/
DWORD AddContentToZip(HZIP hZip, LPSTR lpszDirectory, LPSTR lpszSubDirectory )
{
	DWORD dwError = -1;
	HANDLE hFind  = INVALID_HANDLE_VALUE;  
	WIN32_FIND_DATA FindFileData;
	TCHAR szPthForSearch[ MAX_PATH ] = {0};
	TCHAR szSubDir[MAX_PATH] = {0};
	BOOL bEpty = TRUE;
    
	__try
	{
		if( NULL != lpszSubDirectory )
		{  
			bEpty = strcmp( lpszSubDirectory, _T("") );
			
			if( bEpty != 0 )  
			{
				dwError = ZipAdd( hZip, lpszSubDirectory, 0, 0, ZIP_FOLDER ) ;
				if ( dwError != ZR_OK )
				{
					__leave;
				}
			}
		}
		
		sprintf( szPthForSearch,  _T("%s\\%s\\*"), lpszDirectory, lpszSubDirectory );
		
		hFind = FindFirstFile( szPthForSearch, &FindFileData );  
		if( hFind == INVALID_HANDLE_VALUE )
		{
			dwError = -1;
			__leave;
		}
	 
		while( TRUE )  
		{
			if( FindNextFile( hFind, &FindFileData ) )
			{
				if ( (_tcscmp( FindFileData.cFileName, _T(".") ) == 0) 
					||	( _tcscmp( FindFileData.cFileName, _T("..") ) == 0))
				{
					continue;		
				}
				
				if( !bEpty )
				{
					sprintf( szSubDir,  _T("%s"), FindFileData.cFileName );
				}
				else
				{
					sprintf( szSubDir, _T("%s\\%s"), lpszSubDirectory, FindFileData.cFileName );
				}
				
				if( FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) 
				{
					dwError = AddContentToZip( hZip, lpszDirectory, szSubDir );
					if( 0 != dwError )
					{
						__leave ;
					}				
				}		 
				else
				{			 			 
					TCHAR szFilePath[ MAX_PATH ] = { 0 };			 	
					
					sprintf( szFilePath, _T("%s\\%s"),lpszDirectory,  szSubDir);	
										
					dwError = ZipAdd( hZip, szSubDir, szFilePath, 0, ZIP_FILENAME );
					if ( dwError != ZR_OK )
					{
						__leave;
					}
				}			
			} 
			else
			{
				dwError = GetLastError();
				if( dwError == ERROR_NO_MORE_FILES )  
				{
					dwError = 0;
					break;
				}	
				else
				{			 
					 __leave;
				}
			}
		} 
	}
	__finally
	{
		if( hFind != INVALID_HANDLE_VALUE )
		{
			FindClose( hFind ); 
		}
	}	
	 
	return dwError;	
}

 
 
DWORD WINAPI ZipingDirectory( LPSTR lpszDirectory, LPSTR lpszZipPathName ) 
{
	DWORD dwError =-1;
	HZIP hz = NULL;

	__try
	{		
		if( !DeleteFile( lpszZipPathName ) )	 
		{
			dwError = GetLastError();
			if(  dwError != ERROR_FILE_NOT_FOUND ) 
				__leave ;
		}
		 
		hz = CreateZip( lpszZipPathName, 0, ZIP_FILENAME ) ;		
		if( hz != NULL  ) 
		{       
			dwError = AddContentToZip( hz, lpszDirectory, _T("") ) ;
		}
	}
	__finally
	{
         if( hz != NULL  ) 
		 {
			 CloseZip( hz );
		 }
	}
	

    return dwError ;
}

 
DWORD WINAPI UnZipToDirectory( LPSTR lpszZipPathName, LPSTR lpszDestDirectory)
{ 
	DWORD dwError = -1;
	HZIP hz = NULL ;
	CHAR szOldDir[ MAX_PATH ] = _T("") ;

	__try
	{
		
		if ( lpszZipPathName == NULL || lpszDestDirectory == NULL )
		{  		
			__leave ;
		}
		if( strlen( lpszZipPathName ) ==0 || strlen( lpszDestDirectory ) == 0 )	
		{		
			__leave ;
		}
		
		if( !CreateDirectorys( lpszDestDirectory ) )
		{
			__leave;
		}    
		
		GetCurrentDirectory( MAX_PATH, szOldDir ) ; 
		SetCurrentDirectory( lpszDestDirectory ) ;    
		
		hz = OpenZip( lpszZipPathName, 0, ZIP_FILENAME ) ;
		if ( hz == NULL )
		{
			__leave;
		}
		
		ZIPENTRY ze; 
		GetZipItem( hz, -1, &ze ) ;
		
		unsigned nItems = ze.index; 	
		for ( int i = 0; i < nItems; i++ )
		{ 
			dwError = GetZipItem( hz, i, &ze ); 
			if ( dwError != ZR_OK )
			{
				__leave;
			}
			if( ze.attr == 145 || ze.attr == 177)
			{
				CHAR szDestDir[ MAX_PATH ] = _T("") ;
				
				FormatDirectorys( ze.name) ;
				
				sprintf( szDestDir, _T("%s\\%s"), lpszDestDirectory, ze.name ) ;
				
				if( !CreateDirectorys( szDestDir ) )
				{
					__leave;
				}   
			//	UnZipToDirectory();
				continue ;
			}
			
			dwError = UnzipItem( hz, i, ze.name, 0, ZIP_FILENAME );
			if( dwError != ZR_OK )
			{   		
				OutputDebugString("��ѹ�ļ�����ʧ��UnzipItem( hz, i, ze.name, 0, ZIP_FILENAME ) != ZR_OK ");
				__leave;
			}
		}
    }
	__finally
	{
		if( hz != NULL  ) 
		{
			CloseZip( hz );
		}
		
		SetCurrentDirectory( szOldDir ) ;
	};
	

	return dwError ; 
}
 
