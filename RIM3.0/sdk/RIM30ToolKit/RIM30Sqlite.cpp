//********************************************************************
// RIM30SQLITE.CPP 文件注释
// 文件名 　: RIM30SQLITE.CPP
// 文件路径 : E:\PROJECT\RIM3.0\SRC\SDK\RIM30TOOLKIT/
// 作者 　　: 张永
// 创建时间 : 2015/4/23 14:33
// 文件描述 : 
//*********************************************************************
#include "StdAfx.h"
#include "RIM30Sqlite.h"

/**
* @class CSfwX
* @brief SQLite包装类
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
*  初始化统计，建立SQLite文件
* @param            [in]   lstrFile        
* @param            [in]   lbCreate ,如果不存在是否需要创建     
* @param            [in]   lbWrite       
* @param            [out]  NO
* @return           返回值描述 如果成功,返回true ,失败返回false 
* @node             注释:这里使用 SQLITE_OPEN_FULLMUTEX打开，表示进入Serialized模式，多线可以安全访问。
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
* 关闭统计数据库 
* @param            [in]   NO 
* @param            [out]  NO
* @return           返回值描述
* @node             注释     
* @par              示例:   
* @code
代码片断
* @endcode
* @see               关联          
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
*  判断数据库是否打开
* @param            [in]   lstr 
* @param            [out]  lstr
* @return           返回值描述
* @node             注释     
* @par              示例:   
* @code
代码片断
* @endcode
* @see               关联          
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
*  获取SQLite句柄
* @param            [in]   lstr 
* @param            [out]  lstr
* @return           返回值描述
* @node             注释     
* @par              示例:   
* @code
代码片断
* @endcode
* @see               关联          
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
* @return           返回值描述:
* @node             注释     
* @see              关联          
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
*  判断制定的数据库中，指定的表是否存在
* @param            [in]   lstr 
* @param            [out]  lstr
* @return           返回值描述  
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
*  判断指定的临时数据库中，指定的表是否存在
* @param            [in]   lstr 
* @param            [out]  lstr
* @return           返回值描述  
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
*  判断指定的临时数据库中，指定的表是否存在
* @param            [in]   lstr 
* @param            [out]  lstr
* @return           返回值描述  
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
*  创建统计数据库表,索引,初始数据等(3.7.5)
* @param            [in]   NO 
* @param            [out]  NO
* @return           返回值描述
* @node             注释     
* @par              示例:   
* @code
代码片断
* @endcode
* @see              关联          
*/
void CRIM30Sqlite::SetAttribute()
{ 
	std::string         lstrSQL ;
	SYSTEM_INFO         lsiSysInfo;
	TCHAR               ltcBuf[MAX_PATH] ;

	GetSystemInfo(&lsiSysInfo); 
	/** 这些设置只针对当前打开有效,部分会存为默认 */

	/**  异常中断数据库安全设置为关闭模式,可以提高读写速度 */
	lstrSQL =  "PRAGMA synchronous = 0;" ;   
	Pragmas(lstrSQL);

	/**  临时存储设定为内存模式(手动提交) */
	lstrSQL =  "PRAGMA temp_store = 2;"  ;   
	Pragmas(lstrSQL);
	/**  是否使用旧文件格式 */
	lstrSQL =  "PRAGMA legacy_file_format = 0;" ;   
	Pragmas(lstrSQL);
	/**  设置数据库页面尺寸 */
	/*1. matching pagesize to file system record size is recommended for write performance improvements
	2. default SQLite pagesize is 1k; max SQLite pagesize is 32k
	3. docs seem to say that 32k max pagesize is no longer required, i.e. could possibly compile w/ larger max pagesize (and thus larger pagesize)
	4. larger pagesize will cause more data to be read from file system to do a read (i.e. an entire page must be read)
	*/
	memset(ltcBuf,0,MAX_PATH) ;
	sprintf_s(ltcBuf,MAX_PATH,"PRAGMA page_size = %ud;",lsiSysInfo.dwPageSize );
	lstrSQL = ltcBuf ;   
	Pragmas(lstrSQL);
	/**  设置页面计数,空间达到2T */
	lstrSQL = "PRAGMA max_page_count = 1073741823 ;";
	Pragmas(lstrSQL);

	/**  设置空间自动回收 0--禁止自动空间回收 */
	lstrSQL = "PRAGMA auto_vacuum = 0 ;"  ;   
	Pragmas(lstrSQL);
	/** 设置自动索引,可以提高查询速度 */
	lstrSQL = "PRAGMA automatic_index = True;"  ;   
	Pragmas(lstrSQL);
	/**  设置本次打开的缓冲大小,对读写速度无太大影响 */
	lstrSQL =  "PRAGMA cache_size = 8192;"  ;   
	Pragmas(lstrSQL);
	/**  设置默认缓冲大小失败 ,对读写速度无太大影响 ,3.7.5版本开始该值已经无实际意义*/
	lstrSQL =  "PRAGMA default_cache_size = 8192;"  ;   
	Pragmas(lstrSQL);
	/**  设置Like查询是否区分大小写 ,设置为不区分大小写 */
	lstrSQL =  "PRAGMA case_sensitive_like = True; "  ;   
	Pragmas(lstrSQL);
	/**  设置WAL检查点,目前只有苹果的系统支持,并且它会影响共享内存等的使用,关闭 */
	lstrSQL =  "PRAGMA checkpoint_fullfsync =  False; "  ;   
	Pragmas(lstrSQL);
	/**  查询默认的编译开关 */
	lstrSQL =  "PRAGMA compile_options;"  ;   
	Pragmas(lstrSQL);
	/**  设置是否允许计数改变,3.7.5后该值已经被忽略,无论设置何值,都可以正常的插入,删除 */
	lstrSQL =  "PRAGMA count_changes = 1;" ;   
	Pragmas(lstrSQL);
	/**  空调用返回结果集 3.7.5后该值已经被忽略*/
	//lstrSQL =  "PRAGMA empty_result_callbacks = = True;" ;   
	//Pragmas(lstrSQL);
	/**  设置使用UTF8编码 */
	lstrSQL =  "PRAGMA encoding     = 'UTF-8'; "  ;   
	Pragmas(lstrSQL);
	/**  启动外键关联 */
	lstrSQL =  "PRAGMA foreign_keys = True;" ;   
	Pragmas(lstrSQL);
	/**  3.7.5忽略  */
	lstrSQL =  "PRAGMA full_column_names = True;" ;   
	Pragmas(lstrSQL);
	/**  设置检查是否支持F_FULLFSYNC,由于checkpoint_fullfsync关闭,这里也关闭  */
	lstrSQL =  "PRAGMA fullfsync = False;" ;   
	Pragmas(lstrSQL);
	/**   设置是否忽略约束检查 */
	lstrSQL =  "PRAGMA ignore_check_constraints = False;" ;   
	Pragmas(lstrSQL);
	/**  关闭回滚日志功能,可能导致数据崩溃,但能提高效率 */
	lstrSQL =  "PRAGMA journal_mode = OFF ;"     ;   
	Pragmas(lstrSQL);
	/**  建立数据库联接是否独占,设置为不独占 */
	lstrSQL =  "PRAGMA locking_mode = NORMAL ;"  ;   
	Pragmas(lstrSQL);
	/**  关闭SQLITE_DEBUG  */
	lstrSQL =  "PRAGMA parser_trace = False;" ;   
	Pragmas(lstrSQL);
	/**  关闭递归触发器  */
	lstrSQL =  "PRAGMA recursive_triggers = False; " ;   
	Pragmas(lstrSQL);
	/** 设置SQlite版本   */
	lstrSQL =  "PRAGMA schema_version = 375; " ;   
	Pragmas(lstrSQL);
	/**  设置User版本  */
	lstrSQL =  "PRAGMA user_version = 7;" ;   
	Pragmas(lstrSQL);
	/**  设置允许递归删除   */
	lstrSQL =  "PRAGMA secure_delete = True " ;   
	Pragmas(lstrSQL);
	/**  使用独立读模式,对速度有严重影响 */
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