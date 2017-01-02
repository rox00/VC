//********************************************************************
// RIM30SQLITE.CPP �ļ�ע��
// �ļ��� ��: RIM30SQLITE.CPP
// �ļ�·�� : E:\PROJECT\RIM3.0\SRC\SDK\RIM30TOOLKIT/
// ���� ����: ����
// ����ʱ�� : 2015/4/23 14:33
// �ļ����� : 
//*********************************************************************
#include "StdAfx.h"
#include "RIM30Sqlite.h"

/**
* @class CSfwX
* @brief SQLite��װ��
* @brief  
* ******************************************************************************************************/
CRIM30Sqlite::CRIM30Sqlite()
: m_pDb(NULL)
{
	pError		= new char[512];
	m_strFile.clear() ;
}
CRIM30Sqlite::~CRIM30Sqlite()
{
	if(pError) delete [] pError; pError = NULL;

	Close();
}
/**                
*  ��ʼ��ͳ�ƣ�����SQLite�ļ�
* @param            [in]   lstrFile        
* @param            [in]   lbCreate ,����������Ƿ���Ҫ����     
* @param            [in]   lbWrite       
* @param            [out]  NO
* @return           ����ֵ���� ����ɹ�,����true ,ʧ�ܷ���false 
* @node             ע��:����ʹ�� SQLITE_OPEN_FULLMUTEX�򿪣���ʾ����Serializedģʽ�����߿��԰�ȫ���ʡ�
*/
bool CRIM30Sqlite::Open(std::string lstrFile,bool lbCreate ,bool lbWrite )
{
	try
	{
		int  lnFlags      =  0 ;
		if(_access(lstrFile.c_str(),0) == 0)  
		{
			lnFlags =  lbWrite ? (SQLITE_OPEN_READWRITE |  SQLITE_OPEN_FULLMUTEX /* | SQLITE_OPEN_PRIVATECACHE*/  | SQLITE_OPEN_SHAREDCACHE ) : (SQLITE_OPEN_READONLY | SQLITE_OPEN_FULLMUTEX | SQLITE_OPEN_SHAREDCACHE) ;
			//ACE_DEBUG((LM_DEBUG,ACE_TEXT("[%D(%t)]  Prepare open  SQLite File: %s\n"),lstrFile.c_str()));
		}
		else if(lbCreate)
		{
			lnFlags = SQLITE_OPEN_CREATE | SQLITE_OPEN_READWRITE | SQLITE_OPEN_FULLMUTEX | SQLITE_OPEN_SHAREDCACHE ;
			//ACE_DEBUG((LM_DEBUG,ACE_TEXT("[%D(%t)]  Prepare create SQLite File: %s\n"),lstrFile.c_str()));
		}
		else
		{
			sprintf_s(pError, 512, "Open SQLite File %s fail: not exist.\n",lstrFile.c_str());
			OutputDebugString(pError);
		}

		if(lnFlags != 0)
		{
			if(sqlite3_open_v2(ASCIItoUTF8(lstrFile).c_str(),&m_pDb, lnFlags , NULL) != SQLITE_OK)
			{
				sprintf_s(pError, 512, "SQLite error:Open SQLite File %s fail:%s\n",lstrFile.c_str(),sqlite3_errmsg(m_pDb));
				OutputDebugString(pError);
			}
			else
			{
				//ACE_DEBUG((LM_DEBUG,ACE_TEXT("[%D(%t)]  Open SQLite File %s success \n"),lstrFile.c_str())); 
				m_strFile = lstrFile ;
				SetAttribute();
				return true ;
			}
		}
	}
	catch(...)
	{
		
		sprintf_s(pError, 512, "Open SQLite File %s exception",lstrFile.c_str());
		OutputDebugString(pError);
	}
	Close();
	return false ;
}
/**                
* �ر�ͳ�����ݿ� 
* @param            [in]   NO 
* @param            [out]  NO
* @return           ����ֵ����
* @node             ע��     
* @par              ʾ��:   
* @code
����Ƭ��
* @endcode
* @see               ����          
*/
void    CRIM30Sqlite::Close()
{
	try
	{
		if(m_pDb)
		{
			sqlite3_close(m_pDb);
			m_pDb = NULL ;
		}
		//ACE_DEBUG((LM_DEBUG,ACE_TEXT("[%D(%t)]  SQLite Close %s success\n"),m_strFile.c_str())) ;
	}
	catch (...)
	{
		if(m_pDb)sqlite3_close(m_pDb);
		m_pDb = NULL ;
		
		sprintf_s(pError, 512, "SQLite Close %s exception",m_strFile.c_str());
		OutputDebugString(pError);
	}
	m_strFile.clear() ;
}
/**                
*  �ж����ݿ��Ƿ��
* @param            [in]   lstr 
* @param            [out]  lstr
* @return           ����ֵ����
* @node             ע��     
* @par              ʾ��:   
* @code
����Ƭ��
* @endcode
* @see               ����          
*/
bool CRIM30Sqlite::IsOpen()
{
	if(m_pDb)
	{
		
		sprintf_s(pError, 512, "SQLite DB %s is open\n",m_strFile.c_str());
		OutputDebugString(pError);
		return true ;
	}
	else
	{
		
		sprintf_s(pError, 512, "SQLite DB %s is not open\n",m_strFile.c_str());
		OutputDebugString(pError);
		return false ;
	}	  
}
int CRIM30Sqlite::Count(std::string lstrTable , std::string lstrCondition)
{
	int lnCount = 0 ;
	if(m_pDb)
	{
		sqlite3_stmt*       lstmt ;
		const char*         ltail ;
		std::string     lstrSQL ;
		char *lpSQL   = sqlite3_mprintf("select count(*) from %s %s %s ;", lstrTable.c_str(),lstrCondition.size() ? "where": " "  ,lstrCondition.c_str());
		if(lpSQL)
		{
			lstrSQL = lpSQL ;
			sqlite3_free(lpSQL);
		}
		else
		{
			
			sprintf_s(pError, 512, "Format SQL failure in Count func\n");
			OutputDebugString(pError);
			return 0 ;
		}

		if(sqlite3_prepare(m_pDb, ASCIItoUTF8(lstrSQL).c_str(), -1, &lstmt, &ltail) != SQLITE_OK) 
		{
			
			sprintf_s(pError, 512, "SQLite error:prepare sql failure:%s , error: %s\n",lstrSQL.c_str(),sqlite3_errmsg(m_pDb));
			OutputDebugString(pError);
			return 0 ;
		}
		if(sqlite3_step(lstmt) == SQLITE_ROW) 
		{    
			if(sqlite3_column_count(lstmt) == 1)
			{ 
				lnCount  = sqlite3_column_int(lstmt,0) ;	 
			}
		}
		sqlite3_finalize(lstmt);
	}
	else
	{
		
		sprintf_s(pError, 512, "Count fail : SQLite db not open\n");
		OutputDebugString(pError);
		return 0 ;
	}
	return lnCount ;
}
/**                
*  ��ȡSQLite���
* @param            [in]   lstr 
* @param            [out]  lstr
* @return           ����ֵ����
* @node             ע��     
* @par              ʾ��:   
* @code
����Ƭ��
* @endcode
* @see               ����          
*/
sqlite3* CRIM30Sqlite::Handle() const
{
	return m_pDb ;
}

/**                
*  The PRAGMA statement is a SQL extension specific to SQLite and used to modify the operation of the SQLite library ,
*  or to query the SQLite library for internal (non-table) data
* @param            [in]   lstrCommands 
* @param            [out]  No
* @return           ����ֵ����:
* @node             ע��     
* @see              ����          
*/
bool CRIM30Sqlite::Pragmas(std::string lstrCommands)
{
	try 
	{
		if(m_pDb)
		{
			std::string     lstrSQL = lstrCommands ;
			if(sqlite3_exec(m_pDb , ASCIItoUTF8(lstrSQL).c_str(), NULL, NULL, NULL) != SQLITE_OK )
			{
				
				sprintf_s(pError, 512, "Pragmas fail : %s\n",lstrSQL.c_str());
				OutputDebugString(pError);
			} 
			else
			{

				/*sqlite3_stmt*       lstmt ;
				const char*         ltail ;
				lstrSQL = lstrCommands.substr(0,lstrCommands.find(_T("=")));
				lstrSQL += _T(";");
				if(sqlite3_prepare(m_pDb,CRIMToolKitUtil::ASCIItoUTF8(lstrSQL).c_str(), -1, &lstmt, &ltail) != SQLITE_OK) 
				{
				ACE_ERROR((LM_ERROR,ACE_TEXT("[%D(%t)] SQLite error:prepare sql failure:%s , error: %s\n"),lstrSQL.c_str(),sqlite3_errmsg(m_pDb)));
				}
				while(sqlite3_step(lstmt) == SQLITE_ROW) 
				{    
				if(sqlite3_column_count(lstmt) == 1)
				{ 
				ACE_DEBUG((LM_DEBUG,ACE_TEXT("[%D(%t)] Set:%s Query:%s \n"),lstrCommands.c_str(), (char *)sqlite3_column_text(lstmt,0))) ;	 
				}
				}
				sqlite3_finalize(lstmt);*/
				//ACE_DEBUG ((LM_DEBUG,"[%T(%t)]  SQLite release memory: %.04fMB \n",sqlite3_release_memory((int)sqlite3_memory_highwater(true))/(1024*1024.f)));
				return true ;
			}
		}
		else
		{
			
			sprintf_s(pError, 512, "Pragmas fail : SQLite db not open\n");
			OutputDebugString(pError);
		}
	}
	catch(...)
	{
		
		sprintf_s(pError, 512, "Exc pragma command exception: %s\n",lstrCommands.c_str());
		OutputDebugString(pError);
	}
	return false ;
}
/**                
*  �ж��ƶ������ݿ��У�ָ���ı��Ƿ����
* @param            [in]   lstr 
* @param            [out]  lstr
* @return           ����ֵ����  
*/
bool CRIM30Sqlite::IsExist(std::string lstrTableName)
{
	sqlite3_stmt*       lstmt ;
	const char*         ltail ;
	std::string     lstrSQL ;
	try
	{
		if(m_pDb == NULL)
		{
			
			sprintf_s(pError, 512, "SQLite DB not open: %s\n", lstrTableName.c_str());
			OutputDebugString(pError);
			return false ;
		}
		char *lpSQL = sqlite3_mprintf("SELECT name FROM sqlite_master WHERE type='table' and name = '%s' ORDER BY name;", lstrTableName.c_str());
		if(lpSQL)
		{
			lstrSQL = lpSQL ;
			sqlite3_free(lpSQL);
		}
		else
		{
			
			sprintf_s(pError, 512, "Format SQL failure in IsExist func %s\n", lpSQL);
			OutputDebugString(pError);
			return false ;
		}

		if(sqlite3_prepare(m_pDb,ASCIItoUTF8(lstrSQL).c_str(), -1, &lstmt, &ltail) != SQLITE_OK) 
		{
			
			sprintf_s(pError, 512, "SQLite error:Prepare sql failure:%s , error: %s\n",lstrSQL.c_str(),sqlite3_errmsg(m_pDb));
			OutputDebugString(pError);
			Close();
			return false ;
		}
		while(sqlite3_step(lstmt) == SQLITE_ROW) 
		{    
			if(sqlite3_column_count(lstmt) == 1)
			{ 
				sqlite3_finalize(lstmt);
				return true ;
			}
		}
		sqlite3_finalize(lstmt);
	}
	catch(...)
	{
		
		sprintf_s(pError, 512, "Is table command exception\n");
		OutputDebugString(pError);
		Close();
	}
	return false ;
}
/**                
*  �ж�ָ������ʱ���ݿ��У�ָ���ı��Ƿ����
* @param            [in]   lstr 
* @param            [out]  lstr
* @return           ����ֵ����  
*/
bool CRIM30Sqlite::IsTempExist(std::string lstrTableName)
{
	sqlite3_stmt*       lstmt ;
	const char*         ltail ;
	std::string     lstrSQL ;
	try
	{
		if(m_pDb == NULL)
		{
			
			sprintf_s(pError, 512, "SQLite DB not open: %s\n", lstrTableName.c_str());
			OutputDebugString(pError);
			return false ;
		}
		char *lpSQL = sqlite3_mprintf("SELECT name FROM [TEMP].sqlite_temp_master WHERE type='table' and name = '%s' ORDER BY name;", lstrTableName.c_str());
		if(lpSQL)
		{
			lstrSQL = lpSQL ;
			sqlite3_free(lpSQL);
		}
		else
		{
			
			sprintf_s(pError, 512, "Format SQL failure in IsExist func %s\n", lpSQL);
			OutputDebugString(pError);
			return false ;
		}

		if(sqlite3_prepare(m_pDb,ASCIItoUTF8(lstrSQL).c_str(), -1, &lstmt, &ltail) != SQLITE_OK) 
		{
			
			sprintf_s(pError, 512, "SQLite error:Prepare sql failure:%s , error: %s\n",lstrSQL.c_str(),sqlite3_errmsg(m_pDb));
			OutputDebugString(pError);
			Close();
			return false ;
		}
		while(sqlite3_step(lstmt) == SQLITE_ROW) 
		{    
			if(sqlite3_column_count(lstmt) == 1)
			{ 
				sqlite3_finalize(lstmt);
				return true ;
			}
		}
		sqlite3_finalize(lstmt);
	}
	catch(...)
	{
		
		sprintf_s(pError, 512, "Is table command exception\n");
		OutputDebugString(pError);
		Close();
	}
	return false ;
}
/**                
*  �ж�ָ������ʱ���ݿ��У�ָ���ı��Ƿ����
* @param            [in]   lstr 
* @param            [out]  lstr
* @return           ����ֵ����  
*/
bool CRIM30Sqlite::IsExist(std::string lstrDBName, std::string lstrTableName)
{
	sqlite3_stmt*       lstmt ;
	const char*         ltail ;
	std::string     lstrSQL ;
	try
	{
		if(m_pDb == NULL)
		{
			
			sprintf_s(pError, 512, "SQLite DB not open: %s\n", lstrDBName.c_str());
			OutputDebugString(pError);
			return false ;
		}
		char *lpSQL = sqlite3_mprintf("SELECT name FROM [%s].sqlite_master WHERE type='table' and name = '%s' ORDER BY name;",lstrDBName.c_str(), lstrTableName.c_str());
		if(lpSQL)
		{
			lstrSQL = lpSQL ;
			sqlite3_free(lpSQL);
		}
		else
		{
			
			sprintf_s(pError, 512, "Format SQL failure in IsExist func %s\n", lstrSQL.c_str());
			OutputDebugString(pError);
			return false ;
		}

		if(sqlite3_prepare(m_pDb,ASCIItoUTF8(lstrSQL).c_str(), -1, &lstmt, &ltail) != SQLITE_OK) 
		{
			
			sprintf_s(pError, 512, "SQLite error:Prepare sql failure:%s , error: %s\n",lstrSQL.c_str(),sqlite3_errmsg(m_pDb));
			OutputDebugString(pError);
			Close();
			return false ;
		}
		while(sqlite3_step(lstmt) == SQLITE_ROW) 
		{    
			if(sqlite3_column_count(lstmt) == 1)
			{ 
				sqlite3_finalize(lstmt);
				return true ;
			}
		}
		sqlite3_finalize(lstmt);
	}
	catch(...)
	{
		
		sprintf_s(pError, 512, "Is table command exception\n");
		OutputDebugString(pError);
		Close();
	}
	return false ;
}
/**                
*  ����ͳ�����ݿ��,����,��ʼ���ݵ�(3.7.5)
* @param            [in]   NO 
* @param            [out]  NO
* @return           ����ֵ����
* @node             ע��     
* @par              ʾ��:   
* @code
����Ƭ��
* @endcode
* @see              ����          
*/
void CRIM30Sqlite::SetAttribute()
{ 
	std::string         lstrSQL ;
	SYSTEM_INFO         lsiSysInfo;
	TCHAR               ltcBuf[MAX_PATH] ;

	GetSystemInfo(&lsiSysInfo); 
	/** ��Щ����ֻ��Ե�ǰ����Ч,���ֻ��ΪĬ�� */

	/**  �쳣�ж����ݿⰲȫ����Ϊ�ر�ģʽ,������߶�д�ٶ� */
	lstrSQL =  "PRAGMA synchronous = 0;" ;   
	Pragmas(lstrSQL);

	/**  ��ʱ�洢�趨Ϊ�ڴ�ģʽ(�ֶ��ύ) */
	lstrSQL =  "PRAGMA temp_store = 2;"  ;   
	Pragmas(lstrSQL);
	/**  �Ƿ�ʹ�þ��ļ���ʽ */
	lstrSQL =  "PRAGMA legacy_file_format = 0;" ;   
	Pragmas(lstrSQL);
	/**  �������ݿ�ҳ��ߴ� */
	/*1. matching pagesize to file system record size is recommended for write performance improvements
	2. default SQLite pagesize is 1k; max SQLite pagesize is 32k
	3. docs seem to say that 32k max pagesize is no longer required, i.e. could possibly compile w/ larger max pagesize (and thus larger pagesize)
	4. larger pagesize will cause more data to be read from file system to do a read (i.e. an entire page must be read)
	*/
	memset(ltcBuf,0,MAX_PATH) ;
	sprintf_s(ltcBuf,MAX_PATH,"PRAGMA page_size = %ud;",lsiSysInfo.dwPageSize );
	lstrSQL = ltcBuf ;   
	Pragmas(lstrSQL);
	/**  ����ҳ�����,�ռ�ﵽ2T */
	lstrSQL = "PRAGMA max_page_count = 1073741823 ;";
	Pragmas(lstrSQL);

	/**  ���ÿռ��Զ����� 0--��ֹ�Զ��ռ���� */
	lstrSQL = "PRAGMA auto_vacuum = 0 ;"  ;   
	Pragmas(lstrSQL);
	/** �����Զ�����,������߲�ѯ�ٶ� */
	lstrSQL = "PRAGMA automatic_index = True;"  ;   
	Pragmas(lstrSQL);
	/**  ���ñ��δ򿪵Ļ����С,�Զ�д�ٶ���̫��Ӱ�� */
	lstrSQL =  "PRAGMA cache_size = 8192;"  ;   
	Pragmas(lstrSQL);
	/**  ����Ĭ�ϻ����Сʧ�� ,�Զ�д�ٶ���̫��Ӱ�� ,3.7.5�汾��ʼ��ֵ�Ѿ���ʵ������*/
	lstrSQL =  "PRAGMA default_cache_size = 8192;"  ;   
	Pragmas(lstrSQL);
	/**  ����Like��ѯ�Ƿ����ִ�Сд ,����Ϊ�����ִ�Сд */
	lstrSQL =  "PRAGMA case_sensitive_like = True; "  ;   
	Pragmas(lstrSQL);
	/**  ����WAL����,Ŀǰֻ��ƻ����ϵͳ֧��,��������Ӱ�칲���ڴ�ȵ�ʹ��,�ر� */
	lstrSQL =  "PRAGMA checkpoint_fullfsync =  False; "  ;   
	Pragmas(lstrSQL);
	/**  ��ѯĬ�ϵı��뿪�� */
	lstrSQL =  "PRAGMA compile_options;"  ;   
	Pragmas(lstrSQL);
	/**  �����Ƿ���������ı�,3.7.5���ֵ�Ѿ�������,�������ú�ֵ,�����������Ĳ���,ɾ�� */
	lstrSQL =  "PRAGMA count_changes = 1;" ;   
	Pragmas(lstrSQL);
	/**  �յ��÷��ؽ���� 3.7.5���ֵ�Ѿ�������*/
	//lstrSQL =  "PRAGMA empty_result_callbacks = = True;" ;   
	//Pragmas(lstrSQL);
	/**  ����ʹ��UTF8���� */
	lstrSQL =  "PRAGMA encoding     = 'UTF-8'; "  ;   
	Pragmas(lstrSQL);
	/**  ����������� */
	lstrSQL =  "PRAGMA foreign_keys = True;" ;   
	Pragmas(lstrSQL);
	/**  3.7.5����  */
	lstrSQL =  "PRAGMA full_column_names = True;" ;   
	Pragmas(lstrSQL);
	/**  ���ü���Ƿ�֧��F_FULLFSYNC,����checkpoint_fullfsync�ر�,����Ҳ�ر�  */
	lstrSQL =  "PRAGMA fullfsync = False;" ;   
	Pragmas(lstrSQL);
	/**   �����Ƿ����Լ����� */
	lstrSQL =  "PRAGMA ignore_check_constraints = False;" ;   
	Pragmas(lstrSQL);
	/**  �رջع���־����,���ܵ������ݱ���,�������Ч�� */
	lstrSQL =  "PRAGMA journal_mode = OFF ;"     ;   
	Pragmas(lstrSQL);
	/**  �������ݿ������Ƿ��ռ,����Ϊ����ռ */
	lstrSQL =  "PRAGMA locking_mode = NORMAL ;"  ;   
	Pragmas(lstrSQL);
	/**  �ر�SQLITE_DEBUG  */
	lstrSQL =  "PRAGMA parser_trace = False;" ;   
	Pragmas(lstrSQL);
	/**  �رյݹ鴥����  */
	lstrSQL =  "PRAGMA recursive_triggers = False; " ;   
	Pragmas(lstrSQL);
	/** ����SQlite�汾   */
	lstrSQL =  "PRAGMA schema_version = 375; " ;   
	Pragmas(lstrSQL);
	/**  ����User�汾  */
	lstrSQL =  "PRAGMA user_version = 7;" ;   
	Pragmas(lstrSQL);
	/**  ��������ݹ�ɾ��   */
	lstrSQL =  "PRAGMA secure_delete = True " ;   
	Pragmas(lstrSQL);
	/**  ʹ�ö�����ģʽ,���ٶ�������Ӱ�� */
	lstrSQL =  "PRAGMA read_uncommitted = True ;" ;   
	Pragmas(lstrSQL);
}

bool CRIM30Sqlite::ExeSql(std::string lstrSql )
{
	int result = 0;
	char *pErrInfo = NULL;
	if(SQLITE_OK == sqlite3_exec(m_pDb, lstrSql.c_str(), NULL, NULL, &pErrInfo))
	{
		return true;
	}
	else
	{
		OutputDebugStringA(pErrInfo);
		return false;
	}
}