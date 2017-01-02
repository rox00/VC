//********************************************************************
// RIM30SQLITE.H �ļ�ע��
// �ļ��� ��: RIM30SQLITE.H
// �ļ�·�� : E:\PROJECT\RIM3.0\SRC\SDK\RIM30TOOLKIT/
// ���� ����: ����
// ����ʱ�� : 2015/4/23 14:33
// �ļ����� : 
//*********************************************************************
#pragma once

class CRIM30Sqlite
{
public:
	CRIM30Sqlite();
	virtual ~CRIM30Sqlite();
public:
	/** �򿪻ᴴ�� SQLite ���ݿ� */
	bool				Open(std::string lstrFile,bool lbCreate= true,bool lbWrite = true ); 
	/** �ر� SQLite ���ݿ�   */
	void				Close(); 
	/** �ж����ݿ��Ƿ�� */
	bool				IsOpen();
	/** ��ȡ���¼����Ŀ */
	int					Count(std::string lstrTable , std::string lstrCondition);
	/** ��ȡSQLite��� */
	sqlite3*			Handle() const ;
	/** ͳ�����ݿ��ļ��������� */
	virtual void		SetAttribute();
	/** ����ָ���ı��Ƿ���� */
	bool				IsExist(std::string lstrTableName);
	/** ִ��SQL��� */
	bool				ExeSql(std::string lstrSql );
protected:
	/** ���úͲ�ѯ���ݿ����� */
	bool    			Pragmas(std::string lstrCommands);

	bool    			IsTempExist(std::string lstrTableName);
	bool    			IsExist(std::string lstrDBName, std::string lstrTableName);
protected:
	char				*pError; 
	std::string          m_strFile  ;
	sqlite3*             m_pDb ;
};