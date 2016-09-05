#pragma once

#include <vector>

class AndroidDevice
{
public:
	AndroidDevice()
	{
		SetName(_T(""));
		SetSerialNo(_T(""));
		SetResolution(0, 0);
		SetRooted(FALSE);
		SetConnected(FALSE);

		SetSdPath(_T("/sdcard"));
	}

public:
	void SetName(CString szName) {this->szName = szName;}
	CString GetName() {return szName;}
	void SetSerialNo(CString szSerialNo) {this->szSerialNo = szSerialNo;}
	CString GetSerialNo() {return szSerialNo;}

	void SetSdPath(LPCTSTR szPath) {this->szSdPath = szPath;}
	CString GetSdPath() {return this->szSdPath;}
	
	void SetResolution(int x, int y) {resolution.x = x; resolution.y = y;}
	CPoint GetResolution() { return resolution; }

	void SetConnected(BOOL bConnect) {this->bConnected = bConnect;}
	BOOL IsConnected() {return bConnected;}

	void SetRooted(BOOL bRooted) {this->bRooted = bRooted;}
	BOOL IsRooted() {return bRooted;}

private:
	CString szName;
	CString szSerialNo;
	CPoint resolution;
	BOOL bConnected;
	BOOL bRooted;
	CString szSdPath;
};
