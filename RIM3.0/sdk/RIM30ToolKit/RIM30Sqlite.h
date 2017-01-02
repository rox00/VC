//********************************************************************
// RIM30SQLITE.H 文件注释
// 文件名 　: RIM30SQLITE.H
// 文件路径 : E:\PROJECT\RIM3.0\SRC\SDK\RIM30TOOLKIT/
// 作者 　　: 张永
// 创建时间 : 2015/4/23 14:33
// 文件描述 : 
//*********************************************************************
#pragma once

class CRIM30Sqlite
{
public:
	CRIM30Sqlite();
	virtual ~CRIM30Sqlite();
public:
	/** 打开会创建 SQLite 数据库 */
	bool				Open(std::string lstrFile,bool lbCreate= true,bool lbWrite = true ); 
	/** 关闭 SQLite 数据库   */
	void				Close(); 
	/** 判断数据库是否打开 */
	bool				IsOpen();
	/** 获取表记录集数目 */
	int					Count(std::string lstrTable , std::string lstrCondition);
	/** 获取SQLite句柄 */
	sqlite3*			Handle() const ;
	/** 统计数据库文件属性设置 */
	virtual void		SetAttribute();
	/** 查找指定的表是否存在 */
	bool				IsExist(std::string lstrTableName);
	/** 执行SQL语句 */
	bool				ExeSql(std::string lstrSql );
protected:
	/** 设置和查询数据库属性 */
	bool    			Pragmas(std::string lstrCommands);

	bool    			IsTempExist(std::string lstrTableName);
	bool    			IsExist(std::string lstrDBName, std::string lstrTableName);
protected:
	char				*pError; 
	std::string          m_strFile  ;
	sqlite3*             m_pDb ;
};