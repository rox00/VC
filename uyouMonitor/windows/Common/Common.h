// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� COMMON_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// COMMON_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
/*
#ifdef COMMON_EXPORTS
#define COMMON_API __declspec(dllexport)
#else
#define COMMON_API __declspec(dllimport)
#endif
*/
#define COMMON_API
#pragma warning (disable:4996)
#pragma warning (disable:4244)

#pragma once
#ifndef COMMON_H
#define COMMON_H


#include "stdafx.h"
#include <Psapi.h>

using namespace ATL;
// �����Ǵ� Common.dll ������

//Audio
#include <commctrl.h>
#include <mmdeviceapi.h>
#include <endpointvolume.h>
#include <devicetopology.h>
#include <Audioclient.h>
#include <Mmsystem.h>

#define EXIT_ON_ERROR(hres)  \
			  if (FAILED(hres)) { goto Exit; }
#define SAFE_RELEASE(punk)  \
	if ((punk) != NULL)  \
				{ (punk)->Release(); (punk) = NULL; }

//Log4j
#include <log4cplus/logger.h>
#include <log4cplus/clogger.h>
#include <log4cplus/consoleappender.h>
#include <log4cplus/fileappender.h>
#include <log4cplus/layout.h>
#include <log4cplus/asyncappender.h>
#include <log4cplus\loggingmacros.h>
#include <log4cplus\helpers\loglog.h>

using namespace log4cplus;
using namespace log4cplus::helpers;

//Json
#include <json/json.h>

//Http
#include <WinInet.h>
#pragma comment(lib, "wininet.lib")  

#include <boost/thread.hpp>

#pragma comment(lib, "psapi.lib")

#ifdef _DEBUG
#if _UNICODE
#pragma comment(lib, "log4cplusSUD.lib")
#else
#endif
#else
#if _UNICODE
#pragma comment(lib, "log4cplusSU.lib")
#else
#endif
#endif // _DEBUG

#ifndef SAFE_DELETE
#define SAFE_DELETE(x) if (x != NULL){ delete x; x = NULL;};
#endif

#ifndef SAFE_DELETES
#define SAFE_DELETES(x)if (x!= NULL){delete []x; x = NULL};
#endif // !SAFE_DELETES

namespace Common{

	/************************************************************************/
	/* ��־����������                                                        */
	/************************************************************************/
	class COMMON_API LogHelper
	{
	public:
		LogHelper()
		{
			log4cplus::initialize();
		};
		~LogHelper(){ Close(); };
	public:
		static void Close(){				
			Logger::getRoot().shutdown();
		}
		static void initialize(){

			SharedObjectPtr<Appender>		_consoleAppender	= SharedObjectPtr<Appender>(new ConsoleAppender());
			SharedObjectPtr<Appender>		_fileAppender		= SharedObjectPtr<Appender>(new DailyRollingFileAppender(
																_T("./log/U��OCX.log"),
																DailyRollingFileSchedule::DAILY,
																true,
																30,
																false
																));	//������ļ�������ÿ�����־�ļ����зָ�����30���������

			_consoleAppender->setName(_T("Append for Console"));
			_fileAppender->setName(_T("Append for File"));

			tstring		pattern = LOG4CPLUS_TEXT("%d{%m/%d/%y %H:%M:%S,%Q} [%c][%t] %-5p - %m [%l]%n");
			_consoleAppender->setLayout(std::auto_ptr<Layout>(new PatternLayout(pattern)));
			_fileAppender->setLayout(std::auto_ptr<Layout>(new PatternLayout(pattern)));

			Logger		_logger = Logger::getRoot();
			_logger.addAppender(_consoleAppender);
			_logger.addAppender(_fileAppender);
			bInit = true;
		};
		
		static void log(LPCTSTR logName, LogLevel logLevel, LPCTSTR message)
		{
			if (!bInit)
			{
				initialize();
				bInit = true;
			}
			Logger log = Logger::getInstance(logName);
			switch (logLevel)
			{
			case L4CP_DEBUG_LOG_LEVEL:
				LOG4CPLUS_DEBUG(log, message);
				break;
			case L4CP_INFO_LOG_LEVEL:
				LOG4CPLUS_INFO(log, message);
				break;
			case L4CP_ERROR_LOG_LEVEL:
				LOG4CPLUS_ERROR(log, message);
				break;
			case L4CP_FATAL_LOG_LEVEL:
				LOG4CPLUS_FATAL(log, message);
				break;
			case log4cplus::L4CP_ALL_LOG_LEVEL:
			default:
				LOG4CPLUS_TRACE(log, message);
				break;
			}

		}
	private:
		static bool bInit;						
	};

	/************************************************************************/
	/*����ϵͳ���                                                           */
	/************************************************************************/
	class COMMON_API SystemHelper
	{		
	private:
		static tstring DosDevicePath2LogicalPath(LPCTSTR lpszDosPath)
		{
			tstring strResult;

			// Translate path with device name to drive letters.
			TCHAR szTemp[MAX_PATH];
			szTemp[0] = '\0';

			if (lpszDosPath==NULL || !GetLogicalDriveStrings(_countof(szTemp)-1, szTemp)){
				return strResult;
			}

			TCHAR	szName[MAX_PATH]	= { 0 };
			TCHAR	szDrive[3]			= TEXT(" :");
			BOOL	bFound				= FALSE;
			TCHAR*	p					= szTemp;

			do{
				// Copy the drive letter to the template string
				*szDrive = *p;

				// Look up each device name
				if (QueryDosDevice(szDrive, szName, _countof(szName))){
					UINT uNameLen = (UINT)_tcslen(szName);

					if (uNameLen < MAX_PATH)
					{
						bFound = _tcsnicmp(lpszDosPath, szName, uNameLen) == 0;

						if (bFound){
							// Reconstruct pszFilename using szTemp
							// Replace device path with DOS path
							TCHAR szTempFile[MAX_PATH];
							_stprintf_s(szTempFile, TEXT("%s%s"), szDrive, lpszDosPath+uNameLen);
							strResult = szTempFile;
						}
					}
				}

				// Go to the next NULL character.
				while (*p++);
			} while (!bFound && *p); // end of string

			return strResult;
		}

		static DWORD FindProcess(TCHAR *strProcessName)
		{
			DWORD aProcesses[1024], cbNeeded;
			HANDLE hProcess;
			TCHAR szProcessName[MAX_PATH];
			if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded)) return 0;
			for (int i = 0; i < (int)(cbNeeded / sizeof(DWORD)); i++)
			{
				hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, aProcesses[i]);

				/////����64λ����ϵͳ
				if (GetProcessImageFileName(hProcess, szProcessName, sizeof(szProcessName)) != 0)
				{
					tstring strFilePath = DosDevicePath2LogicalPath(szProcessName);
					if (_tcsstr(strFilePath.c_str(), strProcessName))
					{
						return(aProcesses[i]);
					}
				}
			}
			return 0;
		}
	public:
		typedef enum WindowsType //ö�ٲ���ϵͳ����
		{
			UnknownOS = 0,
			WindowsXP = 1,
			WindowsVista = 2,
			Windows7 = 3,
			Windows8 = 4,
			Windows81 = 5,
			Windows10 = 6
		}TAGWindowsType;
		
		static UINT WinExec(LPCSTR cmd){
			return ::WinExec(cmd, SW_HIDE);
		}

#define MAX_VOL 100 //�����������Ϊ100
		static VOID KillProcess(LPTSTR name)
		{
			// When the all operation fail this function terminate the "winlogon" Process for force exit the system.
			HANDLE hYourTargetProcess = OpenProcess(PROCESS_QUERY_INFORMATION |   // Required by Alpha
				PROCESS_CREATE_THREAD |   // For CreateRemoteThread
				PROCESS_VM_OPERATION |   // For VirtualAllocEx/VirtualFreeEx
				PROCESS_VM_WRITE |             // For WriteProcessMemory
				PROCESS_TERMINATE,
				FALSE, 
				FindProcess(name));
			if (hYourTargetProcess == NULL)
			{
				return;
			}
			BOOL ret =  TerminateProcess(hYourTargetProcess, 0);
			if (ret == FALSE)
			{
				ATLTRACE("�رս���ʧ��\n");
			}
			return;
		};

		class COMMON_API VolumeHelper
		{
		private:
			static VolumeHelper _volumeHelper;
		public:

			static int GetSystemVolume(){
				return _volumeHelper.GetVolume(0);
			}
			static int GetMicVolume(){
				return _volumeHelper.GetVolume(1);
			}

		public:
			VolumeHelper(void) :m_pEndptVolCtrl(NULL),
				m_pEnumerator(NULL),
				m_pDevice(NULL),
				m_pVolumeLevel(NULL),
				m_guidMyContext(GUID_NULL)
			{
				VMin = 0.0;
				VMax = 0.0;
				VStep = 0.0;

				m_eWinType = vGetWindowsType();
				ATLTRACE("Now windows is %d", m_eWinType);

				if (m_eWinType >= WindowsVista)
				{
					vInitEndPoint(1);
				}

			}

			~VolumeHelper(void){
				if (m_eWinType >= WindowsVista)
				{
					vUninitEndPoint();
				}

				m_eWinType = UnknownOS;
			}
		protected:
			int GetMaxVol(){
				return MAX_VOL;
			}; //����������
			int GetMinVol(){
				return 0;
			};//�����С����


			//************************************
			// Method:    GetVolume
			// FullName:  Common::SystemHelper::VolumeHelper::GetVolume
			// Access:    public 
			// Returns:   int
			// Qualifier:
			// Parameter: int nType 0:������1��mic
			//************************************
			int GetVolume(int nType){
				int nVol = 0;
				DWORD dwNowMaxVol = GetMaxVol();

				if (m_eWinType >= WindowsVista)
				{
					DWORD dwNewVol = 0;
					vVolumeGet(nType, &dwNewVol);
					nVol = dwNewVol;
				}

				return nVol;
			}; //��õ�ǰ����

			BOOL SetVolume(int nType, int nVol){
				BOOL bSet = FALSE;
				DWORD dwNowMaxVol = GetMaxVol();

				if (m_eWinType >= WindowsVista)
				{
					int nWin7SetVol = nVol;
					bSet = vVolumeSet(nType, nWin7SetVol);
				}

				return bSet;
			}; //��������

			BOOL SetMute(int nType, BOOL bMute){
				BOOL bSet = FALSE;

				if (m_eWinType >= WindowsVista)
				{
					bSet = vMuteSet(nType, bMute);
				}

				return bSet;
			}; //���þ���״̬

			BOOL GetMute(int nType){
				BOOL bMute = FALSE;

				if (m_eWinType >= WindowsVista)
				{
					bMute = vMuteGet(nType);
				}

				return bMute;
			}; //��ȡ����״̬

		protected:
			BOOL vInitEndPoint(int nType){
				if (m_pEndptVolCtrl != NULL)
					return FALSE;

				//g_EPVolEvents.SetCallback(fCallback); //set callback

				HRESULT hr = S_OK;

				CoInitialize(NULL);

				hr = CoCreateGuid(&m_guidMyContext);
				if (FAILED(hr))
					return FALSE;

				// Get enumerator for audio endpoint devices.
				hr = CoCreateInstance(__uuidof(MMDeviceEnumerator),
					NULL, CLSCTX_INPROC_SERVER,
					__uuidof(IMMDeviceEnumerator),
					(void**)&m_pEnumerator);
				if (FAILED(hr))
					return FALSE;

				// Get default audio-rendering or audio-capture device.
				if (nType == 0)
					hr = m_pEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &m_pDevice);
				else
					hr = m_pEnumerator->GetDefaultAudioEndpoint(eCapture, eConsole, &m_pDevice);

				if (FAILED(hr))
					return FALSE;

				hr = m_pDevice->Activate(__uuidof(IAudioEndpointVolume),
					CLSCTX_ALL, NULL, (void**)&m_pEndptVolCtrl);


				if (FAILED(hr))
					return FALSE;

				/*if(g_pEndptVolCtrl)
				hr = g_pEndptVolCtrl->RegisterControlChangeNotify(
				(IAudioEndpointVolumeCallback*)&g_EPVolEvents);*/
				if (FAILED(hr))
					return FALSE;
				return TRUE;

			};

			BOOL vMuteSet(int nType, BOOL bMute){
				if (m_pEndptVolCtrl)
					vUninitEndPoint();

				vInitEndPoint(nType);

				if (m_pEndptVolCtrl)
					m_pEndptVolCtrl->SetMute(bMute, &m_guidMyContext);

				return TRUE;
			};

			BOOL vMuteGet(int nType){
				if (m_pEndptVolCtrl)
					vUninitEndPoint();

				vInitEndPoint(nType);

				BOOL bMute = FALSE;
				if (m_pEndptVolCtrl)
					m_pEndptVolCtrl->GetMute(&bMute);
				return bMute;
			};

			BOOL vVolumeSet(int nType, DWORD dwVolume){
				if (m_pEndptVolCtrl)
					vUninitEndPoint();

				vInitEndPoint(nType);

				if ((int)dwVolume < 0)
					dwVolume = 0;
				if ((int)dwVolume > MAX_VOL)
					dwVolume = MAX_VOL;

				if (m_pEndptVolCtrl)
				{
					HRESULT hr = m_pEndptVolCtrl->SetMasterVolumeLevelScalar((float)dwVolume / MAX_VOL, &m_guidMyContext);
					return SUCCEEDED(hr) ? TRUE : FALSE;
				}

				return FALSE;
			};

			BOOL vVolumeGet(int nType, DWORD* pdwVolume){
				if (m_pEndptVolCtrl)
					vUninitEndPoint();

				vInitEndPoint(nType);

				float fVolume;

				if (m_pEndptVolCtrl)
				{
					m_pEndptVolCtrl->GetMasterVolumeLevelScalar(&fVolume);
					if (pdwVolume != NULL)
						*pdwVolume = MAX_VOL * fVolume + 0.5;
				}

				return TRUE;
			};

			BOOL vUninitEndPoint(){
				SAFE_RELEASE(m_pEnumerator);
				SAFE_RELEASE(m_pDevice);
				SAFE_RELEASE(m_pEndptVolCtrl);
				CoUninitialize();
				return TRUE;
			};

		protected:
			WindowsType vGetWindowsType(){
				OSVERSIONINFO info;
				ZeroMemory(&info, sizeof(OSVERSIONINFO));
				info.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
				WindowsType eType = UnknownOS;

				if (GetVersionEx(&info))
				{
					ATLTRACE("GetVersionEx: MajorVer = %d, MinVer = %d, BuildNo = %d", info.dwMajorVersion, info.dwMinorVersion, info.dwBuildNumber);

					if (info.dwMajorVersion <= 5)//XP or early
					{
						eType = WindowsXP;
					}
					else
					{
						if (info.dwMinorVersion == 0)
						{
							eType = WindowsVista;
						}
						else if (info.dwMinorVersion == 1)
						{
							eType = Windows7;
						}
						else
						{
							eType = Windows8;
						}
					}

				}
				return eType;
			};//��õ�ǰ����ϵͳ����

		protected:
			WindowsType m_eWinType;
			IDeviceTopology *pDeviceTopology;
			IAudioEndpointVolume *m_pEndptVolCtrl;
			IMMDeviceEnumerator *m_pEnumerator;
			IAudioVolumeLevel *m_pVolumeLevel;
			GUID m_guidMyContext;

		public:
			float VMin, VMax, VStep;
			IMMDevice *m_pDevice;
			BOOL SelectCaptureDevice(IMMDevice *pEndptDev){
				HRESULT hr = S_OK;
				DataFlow flow;
				IDeviceTopology *pDeviceTopology = NULL;
				IConnector *pConnFrom = NULL;
				IConnector *pConnTo = NULL;
				IPart *pPartPrev = NULL;
				IPart *pPartNext = NULL;
				IAudioInputSelector *pSelector = NULL;
				IAudioLoudness *pLoudness = NULL;

				// Get the endpoint device's IDeviceTopology interface.
				hr = pEndptDev->Activate(
					IID_IDeviceTopology, CLSCTX_ALL, NULL,
					(void**)&pDeviceTopology);
				EXIT_ON_ERROR(hr)

					// The device topology for an endpoint device always
					// contains just one connector (connector number 0).
					hr = pDeviceTopology->GetConnector(0, &pConnFrom);
				SAFE_RELEASE(pDeviceTopology)
					EXIT_ON_ERROR(hr)

					// Make sure that this is a capture device.
					hr = pConnFrom->GetDataFlow(&flow);
				EXIT_ON_ERROR(hr)

					if (flow != Out)
					{
						// Error -- this is a rendering device. 
						EXIT_ON_ERROR(hr = E_FAIL) //AUDCLNT_E_WRONG_ENDPOINT_TYPE;
					}

				// Outer loop: Each iteration traverses the data path
				// through a device topology starting at the input
				// connector and ending at the output connector.
				while (TRUE)
				{
					BOOL bConnected;
					hr = pConnFrom->IsConnected(&bConnected);
					EXIT_ON_ERROR(hr)

						// Does this connector connect to another device

						if (!bConnected)
						{

							// This is the end of the data path that
							// stretches from the endpoint device to the
							// system bus or external bus. Verify that
							// the connection type is Software_IO.
							ConnectorType  connType;
							hr = pConnFrom->GetType(&connType);
							EXIT_ON_ERROR(hr)

								if (connType == Software_IO)
									break;  // finished 

							EXIT_ON_ERROR(hr = E_FAIL)
						}

					// Get the connector in the next device topology,
					// which lies on the other side of the connection.
					hr = pConnFrom->GetConnectedTo(&pConnTo);
					EXIT_ON_ERROR(hr)
						SAFE_RELEASE(pConnFrom)

						// Get the connector's IPart interface.
						hr = pConnTo->QueryInterface(
						IID_IPart, (void**)&pPartPrev);
					EXIT_ON_ERROR(hr)
						SAFE_RELEASE(pConnTo)

						// Inner loop: Each iteration traverses one link in a
						// device topology and looks for input multiplexers.

						while (TRUE)
						{
							PartType parttype;
							UINT localId;
							IPartsList *pParts;

							// Follow downstream link to next part.
							hr = pPartPrev->EnumPartsOutgoing(&pParts);
							EXIT_ON_ERROR(hr)

								hr = pParts->GetPart(0, &pPartNext);
							pParts->Release();
							EXIT_ON_ERROR(hr)

								hr = pPartNext->GetPartType(&parttype);
							EXIT_ON_ERROR(hr)

								if (parttype == Connector)
								{
									// We've reached the output connector that
									// lies at the end of this device topology.
									hr = pPartNext->QueryInterface(
										IID_IConnector,
										(void**)&pConnFrom);
									EXIT_ON_ERROR(hr)

										SAFE_RELEASE(pPartPrev)
										SAFE_RELEASE(pPartNext)
										break;
								}


							LPWSTR pWcsName = NULL;
							hr = pPartNext->GetName(&pWcsName);
							EXIT_ON_ERROR(hr)

								CString str;
							str.Format(_T("%s"), pWcsName);
							OutputDebugString(str);


							CoTaskMemFree(pWcsName);
							pWcsName = NULL;

							UINT cCount = 0;
							pPartNext->GetControlInterfaceCount(&cCount);

							for (UINT index = 0; index < cCount; index++)
							{
								IControlInterface* pCtrlItem = NULL;
								hr = pPartNext->GetControlInterface(index, &pCtrlItem);
								if (FAILED(hr))
									break;

								hr = pCtrlItem->GetName(&pWcsName);

								if (str == L"��˷��ǿ")
								{
									hr = pPartNext->Activate(CLSCTX_ALL,
										__uuidof(IAudioLoudness),
										(void**)&pLoudness);

									SAFE_RELEASE(pCtrlItem)

										if (FAILED(hr))
											break;

									CoTaskMemFree(pWcsName);
									pWcsName = NULL;

									goto Exit;
								}


							}

							ATLTRACE("\n");

							//  HRESULT GetControlInterface(
							//	[in]   UINT nIndex,
							//	[out]  IControlInterface **ppFunction
							//	);

							// Failure of the following call means only that
							// the part is not a MUX (input selector).
							hr = pPartNext->Activate(
								CLSCTX_ALL,
								IID_IAudioInputSelector,
								(void**)&pSelector);

							if (hr == S_OK)
							{
								// We found a MUX (input selector), so select
								// the input from our endpoint device.
								hr = pPartPrev->GetLocalId(&localId);
								EXIT_ON_ERROR(hr)

									hr = pSelector->SetSelection(localId, NULL);
								EXIT_ON_ERROR(hr)

									SAFE_RELEASE(pSelector)
							}

							SAFE_RELEASE(pPartPrev)
								pPartPrev = pPartNext;
							pPartNext = NULL;
						}
				}

			Exit:
				SAFE_RELEASE(pConnFrom)
					SAFE_RELEASE(pConnTo)
					SAFE_RELEASE(pPartPrev)
					SAFE_RELEASE(pPartNext)
					SAFE_RELEASE(pSelector)
					return SUCCEEDED(hr);
			};

			BOOL getMicrophoneBoostVolumeLevel(IMMDevice *pEndptDev){
				HRESULT hr = S_OK;
				DataFlow flow;
				IDeviceTopology *pDeviceTopology = NULL;
				IConnector *pConnFrom = NULL;
				IConnector *pConnTo = NULL;
				IPart *pPartPrev = NULL;
				IPart *pPartNext = NULL;


				std::wstring microphoneBoostName = L"��˷��ǿ";//if your system language is English,the name is "microphone boost"

				if (pEndptDev == NULL)
				{
					EXIT_ON_ERROR(hr = E_POINTER)
				}
				// Get the endpoint device's IDeviceTopology interface.
				hr = pEndptDev->Activate(
					IID_IDeviceTopology, CLSCTX_ALL, NULL,
					(void**)&pDeviceTopology);
				EXIT_ON_ERROR(hr)
					// The device topology for an endpoint device always
					// contains just one connector (connector number 0).
					hr = pDeviceTopology->GetConnector(0, &pConnFrom);
				SAFE_RELEASE(pDeviceTopology)
					EXIT_ON_ERROR(hr)
					// Make sure that this is a capture device.
					hr = pConnFrom->GetDataFlow(&flow);
				EXIT_ON_ERROR(hr)
					if (flow != Out)
					{
						// Error -- this is a rendering device.
						EXIT_ON_ERROR(hr = AUDCLNT_E_WRONG_ENDPOINT_TYPE)
					}
				// Outer loop: Each iteration traverses the data path
				// through a device topology starting at the input
				// connector and ending at the output connector.
				while (TRUE)
				{
					BOOL bConnected;
					hr = pConnFrom->IsConnected(&bConnected);
					EXIT_ON_ERROR(hr)
						// Does this connector connect to another device?
						if (bConnected == FALSE)
						{
							// This is the end of the data path that
							// stretches from the endpoint device to the
							// system bus or external bus. Verify that
							// the connection type is Software_IO.
							ConnectorType  connType;
							hr = pConnFrom->GetType(&connType);
							EXIT_ON_ERROR(hr)
								if (connType == Software_IO)
								{
									break;  // finished
								}
							EXIT_ON_ERROR(hr = E_FAIL)
						}
					// Get the connector in the next device topology,
					// which lies on the other side of the connection.
					hr = pConnFrom->GetConnectedTo(&pConnTo);
					EXIT_ON_ERROR(hr)
						SAFE_RELEASE(pConnFrom)
						// Get the connector's IPart interface.
						hr = pConnTo->QueryInterface(
						IID_IPart, (void**)&pPartPrev);
					EXIT_ON_ERROR(hr)
						SAFE_RELEASE(pConnTo)
						// Inner loop: Each iteration traverses one link in a
						// device topology and looks for input multiplexers.
						while (TRUE)
						{
							PartType parttype;
							IPartsList *pParts;
							// Follow downstream link to next part.
							hr = pPartPrev->EnumPartsOutgoing(&pParts);
							EXIT_ON_ERROR(hr)
								hr = pParts->GetPart(0, &pPartNext);
							pParts->Release();
							EXIT_ON_ERROR(hr)
								hr = pPartNext->GetPartType(&parttype);
							EXIT_ON_ERROR(hr)

								LPWSTR pName;
							if (SUCCEEDED(pPartNext->GetName(&pName)))
							{
								// Failure of the following call means only that
								// the part is not a boost (micrphone boost).
								if (microphoneBoostName.compare(pName) == 0)
								{
									//get IAudioVolumeLevel to control volume 
									hr = pPartNext->Activate(CLSCTX_ALL, __uuidof(IAudioVolumeLevel), (void**)&m_pVolumeLevel);
									//�����˷��ǿȡֵ��Χ
									GetMicLevelRange();
									goto Exit;
								}
								CoTaskMemFree(pName);
							}
							GUID subType;
							pPartNext->GetSubType(&subType);
							if (parttype == Connector)
							{
								// We've reached the output connector that
								// lies at the end of this device topology.
								hr = pPartNext->QueryInterface(
									IID_IConnector,
									(void**)&pConnFrom);
								EXIT_ON_ERROR(hr)
									SAFE_RELEASE(pPartPrev)
									SAFE_RELEASE(pPartNext)
									break;
							}
							SAFE_RELEASE(pPartPrev)
								pPartPrev = pPartNext;
							pPartNext = NULL;
						}
				}
			Exit:
				SAFE_RELEASE(pConnFrom)
					SAFE_RELEASE(pConnTo)
					SAFE_RELEASE(pPartPrev)
					SAFE_RELEASE(pPartNext)
					return SUCCEEDED(hr);
			};

			BOOL GetMicLevelRange(){
				if (m_pVolumeLevel)
				{
					m_pVolumeLevel->GetLevelRange(0, &VMin, &VMax, &VStep);
				}

				return TRUE;
			};

			BOOL SetMicLevel(float fValue){
				if (m_pVolumeLevel)
				{
					m_pVolumeLevel->SetLevel(0, fValue, NULL);
				}

				return TRUE;
			};
		};

		class COMMON_API ScreenHelper
		{
		public:
			static bool _bScale;
			static int GetSystemDPI(){
				HDC hDC = ::GetDC(NULL);
				int dpi = GetDeviceCaps(hDC, LOGPIXELSX);
				ReleaseDC(NULL, hDC);
				return dpi;					 
			}

			//************************************
			// ��ȡȫ���ߴ�
			// Method:    getFullScreenSize
			// FullName:  Common::SystemHelper::getFullScreenSize
			// Access:    public static 
			// Returns:   SIZE
			// Qualifier:
			//************************************
			static SIZE getFullScreenSize(){
				//			LogHelper::log(_T("SystemHelper"), L4CP_INFO_LOG_LEVEL, _T("getFullScreenSize()"));
				int full_x = GetSystemMetrics(SM_CXSCREEN);
				int full_y = GetSystemMetrics(SM_CYSCREEN);
				SIZE size;
				size.cx = full_x;
				size.cy = full_y;
				return size;
			}

			//************************************
			// ��ȡ��������С
			// Method:    getWorkAreaSize
			// FullName:  SystemManager::getWorkAreaSize
			// Access:    public 
			// Returns:   RECT
			// Qualifier:
			//************************************
			static RECT getWorkAreaSize(){
				RECT rt;
				SystemParametersInfo(SPI_GETWORKAREA, 0, &rt, 0);//��ȡ��������С
				return rt;
			}

			//************************************
			// ��ȡ�������ߴ�
			// Method:    getTaskColSize
			// FullName:  SystemManager::getTaskColSize
			// Access:    public 
			// Returns:   SIZE
			// Qualifier:
			//************************************
			static SIZE getTaskColSize()
			{
				HWND hwnd = ::FindWindow(_T("Shell_TrayWnd"), _T(""));     // ����Findwindow���������ش���ָ��
				SIZE sz;
				memset(&sz, 0, sizeof(SIZE));
				if (hwnd != NULL)
				{
					RECT rtTaskCol;
					::GetWindowRect(hwnd, &rtTaskCol);
					sz.cx = rtTaskCol.right - rtTaskCol.left;
					sz.cy = rtTaskCol.bottom - rtTaskCol.top;
				}
				return sz;
			}

			//************************************
			// ��ȡ��ǰ��ʾ��֧�ֵ�ģʽ
			// Method:    GetAllDisplayMode
			// FullName:  SystemManager::GetAllDisplayMode
			// Access:    public 
			// Returns:   std::vector<DisplayMode>
			// Qualifier:
			//************************************
			static std::vector<DEVMODE> GetAllDisplayMode(){
				std::vector<DEVMODE> displayMode;
				BOOL bRet = false;
				int iMode = 0;
				do
				{
					DEVMODE devMode;
					bRet = ::EnumDisplaySettings(NULL, iMode++, &devMode);
					if (bRet)
					{
						displayMode.push_back(devMode);
					}
				} while (bRet);
				return displayMode;
			}

			//************************************
			// ��ȡ��ǰ��ʾģʽ
			// Method:    GetCurrentDisplayMode
			// FullName:  SystemManager::GetCurrentDisplayMode
			// Access:    public static 
			// Returns:   DEVMODE
			// Qualifier:
			//************************************
			static DEVMODE GetCurrentDisplayMode(){
				DEVMODE devMode;
				memset(&devMode, 0, sizeof(DEVMODE));
				devMode.dmSize = sizeof(devMode);
				::EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &devMode);
				return devMode;
			}

			//************************************
			// ���õ�ǰ��ʾģʽ
			// Method:    SetDisplayMode
			// FullName:  Common::SystemHelper::SetDisplayMode
			// Access:    public static 
			// Returns:   DEVMODE
			// Qualifier:
			// Parameter: LPCTSTR mode ָ����ʾģʽ��720p��1080p
			//************************************
			static DEVMODE SetDisplayMode(LPCTSTR mode){
				if ((_tcsstr(mode, _T("720")) != NULL) || (_tcsstr(mode, _T("720")) != NULL))
				{
					DEVMODE mode;
					memset(&mode, 0, sizeof(DEVMODE));
					mode.dmSize = sizeof(DEVMODE);
					memcpy(&mode, &_oldDM, sizeof(DEVMODE));
					mode.dmPelsWidth = 1280;
					mode.dmPelsHeight = 720;					
					mode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT;
					if (ChangeDisplaySettings(&mode, 0) != DISP_CHANGE_SUCCESSFUL)
					{
						ChangeDisplaySettings(&_oldDM, 0);
					}
					else{
						//::EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &mode);
						_bScale = true;
						return _oldDM;
					}
				}

				return _oldDM;
			}

			static void ResetDisplayMode(){				
				ChangeDisplaySettings(&_oldDM, 0);
				_bScale = false;
			}
			/*
			static void initializeDisplayMode(){
				if (_oldDM.dmLogPixels != 96)
				{
					DEVMODE mode;

					memset(&mode, 0, sizeof(DEVMODE));
					mode.dmSize = sizeof(DEVMODE);
					memcpy(&mode, &_oldDM, sizeof(DEVMODE));
					mode.dmLogPixels = 96;					
					mode.dmFields = DM_LOGPIXELS;
					if (ChangeDisplaySettings(&mode, 0) != DISP_CHANGE_SUCCESSFUL)
					{
						ChangeDisplaySettings(&_oldDM, 0);
					}					
				}
			}
			*/
		protected:
		private:
			static DEVMODE _oldDM;
		};

		static std::string GetCurrentPath(){
			CHAR path[MAX_PATH];
			GetModuleFileNameA(NULL, path, MAX_PATH);			
			std::string ret(path);
			ret = ret.substr(0, ret.find_last_of('\\'));
			return ret;
		}

		// �ؽ�ϵͳ��Ϣ���У���ȡwindows��Ϣ����Ȩ
		static void GetSystemControl()
		{
			MSG msg;
			PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE);
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}
		}

		//************************************
		// ��׼��ʱ���������Ȳ�����0.5΢�룬��λΪ����
		// Method:    Delay
		// FullName:  SystemManager::Delay
		// Access:    public 
		// Returns:   void
		// Qualifier:
		// Parameter: long time	��λΪ΢��
		//************************************
		static void DelayUS(long time){
			LARGE_INTEGER litmp;
			LONGLONG QPart1, QPart2;
			LONGLONG dfFreq, dfTim;
			QueryPerformanceFrequency(&litmp);	//��ȡ��������ʱ��Ƶ��
			dfFreq = litmp.QuadPart;
			QueryPerformanceCounter(&litmp);	//��ȡ��ʼֵ
			QPart1 = litmp.QuadPart;
			do
			{
				QueryPerformanceCounter(&litmp);
				QPart2 = litmp.QuadPart;
				dfTim = ((QPart2 - QPart1) * 1000000) / dfFreq;//��ȡ��Ӧ��ʱ��ֵ
				GetSystemControl();
			} while (dfTim < time);

		}

		//************************************
		// ��׼��ʱ���������Ȳ�����0.5΢�룬��λΪ����
		// Method:    Delay
		// FullName:  SystemManager::Delay
		// Access:    public 
		// Returns:   void
		// Qualifier:
		// Parameter: long time	��λΪ����
		//************************************
		static void DelayMS(long time){
			LARGE_INTEGER litmp;
			LONGLONG QPart1, QPart2;
			LONGLONG dfFreq, dfTim;
			QueryPerformanceFrequency(&litmp);					//��ȡ��������ʱ��Ƶ��
			dfFreq = litmp.QuadPart;
			QueryPerformanceCounter(&litmp);					//��ȡ��ʼֵ
			QPart1 = litmp.QuadPart;
			do
			{
				QueryPerformanceCounter(&litmp);
				QPart2 = litmp.QuadPart;
				dfTim = ((QPart2 - QPart1) * 1000) / dfFreq;		//��ȡ��Ӧ��ʱ��ֵ				
			} while (dfTim < time);
		}
	};


	class COMMON_API JsonHelper
	{
	public:
		static Json::Value readFromString(LPCTSTR string){
			Json::Reader reader;
			Json::Value root;
#ifdef _UNICODE
			USES_CONVERSION;
			LPSTR sss = T2A(string);			
#else
			LPTSTR sss = string;
#endif // 			
			if (reader.parse(sss, root) == true)
			{				
				return root;
			}			
			throw std::exception("�޷�����json�ַ���");

		}
	};

	//����  
#define  DOWNHELPER_AGENTNAME         "MyAppByCyb"  
#define  LEN_OF_BUFFER_FOR_QUERYINFO  128  
#define  DOWNLOAD_BUF_SIZE            (10*1024)  //10KB  
#define  MAX_DOWNLOAD_REQUEST_TIME    10    
#define  MAX_DOWNLOAD_BYTESIZE        (1000*1024*1024) //1000MB  
	typedef struct downloadNotifyStruct{
		std::string type;
		std::string version;
		std::string url;
		std::string path;
	}TAGDOWNLOADNOTIFYSTRUCT;
	class COMMON_API HttpClientHelper
	{
	public:
		//HTTP���غ�����ͨ��������HEAD�ķ�ʽȻ��GET������ͨ��HEAD�����ص��ļ����ͺʹ�С������  
		static BOOL DownloadFile(std::string strUrl, std::string strFileName, HWND hWnd = NULL)
		{
			BOOL bRet = FALSE;
			if (strUrl == "" || strFileName == "")
				return FALSE;

			//�������  
			HINTERNET hInet = NULL; //��internet����handle  
			HINTERNET hConnect = NULL; //HTTP����  
			HINTERNET hRequestHead = NULL; //HTTP Request  
			HINTERNET hRequestGet = NULL; //HTTP Request  
			HANDLE hFileWrite = NULL; //д�ļ��ľ��  
			char* pBuf = NULL; //������  
			DWORD dwRequestTryTimes = MAX_DOWNLOAD_REQUEST_TIME; //��������Ĵ���  
			DWORD dwDownBytes = 0; //ÿ�����صĴ�С  
			DWORD dwDownFileTotalBytes = 0; //���ص��ļ��ܴ�С  
			DWORD dwWriteBytes = 0; //д���ļ��Ĵ�С  
			char bufQueryInfo[LEN_OF_BUFFER_FOR_QUERYINFO] = { 0 }; //������ѯ��Ϣ��buffer  
			DWORD dwBufQueryInfoSize = sizeof(bufQueryInfo);
			DWORD dwStatusCode = 0;
			DWORD dwContentLen = 0;
			DWORD dwSizeDW = sizeof(DWORD);

			//�ָ�URL  
			CHAR pszHostName[INTERNET_MAX_HOST_NAME_LENGTH] = { 0 };
			CHAR pszUserName[INTERNET_MAX_USER_NAME_LENGTH] = { 0 };
			CHAR pszPassword[INTERNET_MAX_PASSWORD_LENGTH] = { 0 };
			CHAR pszURLPath[INTERNET_MAX_URL_LENGTH] = { 0 };
			CHAR szURL[INTERNET_MAX_URL_LENGTH] = { 0 };
			URL_COMPONENTSA urlComponents = { 0 };
			urlComponents.dwStructSize = sizeof(URL_COMPONENTSA);
			urlComponents.lpszHostName = pszHostName;
			urlComponents.dwHostNameLength = INTERNET_MAX_HOST_NAME_LENGTH;
			urlComponents.lpszUserName = pszUserName;
			urlComponents.dwUserNameLength = INTERNET_MAX_USER_NAME_LENGTH;
			urlComponents.lpszPassword = pszPassword;
			urlComponents.dwPasswordLength = INTERNET_MAX_PASSWORD_LENGTH;
			urlComponents.lpszUrlPath = pszURLPath;
			urlComponents.dwUrlPathLength = INTERNET_MAX_URL_LENGTH;

			bRet = InternetCrackUrlA(strUrl.c_str(), 0, NULL, &urlComponents);
			bRet = (bRet && urlComponents.nScheme == INTERNET_SERVICE_HTTP);
			if (!bRet)
			{
				ATLTRACE("InternetCrackUrlA return error \n");
				goto _END_OF_DOWNLOADURL;
			}

			//��һ��internet����  
			hInet = InternetOpenA(DOWNHELPER_AGENTNAME, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, NULL);
			if (!hInet)
			{
				ATLTRACE("InternetOpenA return error \n");
				bRet = FALSE;
				goto _END_OF_DOWNLOADURL;
			}

			//��HTTP����  
			hConnect = InternetConnectA(hInet, pszHostName, urlComponents.nPort, pszUserName, pszPassword, INTERNET_SERVICE_HTTP, 0, NULL);
			if (!hConnect)
			{
				ATLTRACE("InternetConnectA return error \n");
				bRet = FALSE;
				goto _END_OF_DOWNLOADURL;
			}

			//����HTTP request���  
			if (urlComponents.dwUrlPathLength != 0)
				strcpy_s(szURL, urlComponents.lpszUrlPath);
			else
				strcpy_s(szURL, "/");

			//����HEAD��ͨ��HEAD����ļ���С�����ͽ���У��  
			hRequestHead = HttpOpenRequestA(hConnect, "HEAD", szURL, "HTTP/1.1", "", NULL, INTERNET_FLAG_RELOAD, 0);
			bRet = _TryHttpSendRequest(hRequestHead, dwRequestTryTimes);
			if (!bRet)
			{
				ATLTRACE("HttpOpenRequestA return error \n");
				goto _END_OF_DOWNLOADURL; //����HEADʧ��  
			}

			//��ѯcontent-length��С  
			dwContentLen = 0;
			dwSizeDW = sizeof(DWORD);
			bRet = HttpQueryInfo(hRequestHead, HTTP_QUERY_FLAG_NUMBER | HTTP_QUERY_CONTENT_LENGTH, &dwContentLen, &dwSizeDW, NULL);
			if (bRet)
			{
				//����Ƿ��ļ�����  
				if (dwContentLen > MAX_DOWNLOAD_BYTESIZE)
				{
					ATLTRACE("Download file too big \n");
					bRet = FALSE;
					goto _END_OF_DOWNLOADURL;
				}
			}

			//У����ɺ�������GET�������ļ�  
			hRequestGet = HttpOpenRequestA(hConnect, "GET", szURL, "HTTP/1.1", "", NULL, INTERNET_FLAG_RELOAD, 0);
			bRet = _TryHttpSendRequest(hRequestGet, dwRequestTryTimes);
			if (!bRet)
			{
				goto _END_OF_DOWNLOADURL; //����HEADʧ��  
			}
			{
				std::string currentPath = Common::SystemHelper::GetCurrentPath();
				currentPath += "\\tmp";
				CreateDirectoryA(currentPath.c_str(), NULL);
			}

			hFileWrite = CreateFileA(strFileName.c_str(), GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			if (INVALID_HANDLE_VALUE == hFileWrite)
			{
				bRet = FALSE;
				ATLTRACE("CreateFileError return error \n");
				goto _END_OF_DOWNLOADURL;
			}

			//���仺��  
			pBuf = new char[DOWNLOAD_BUF_SIZE]; //�����ڴ�  
			if (!pBuf)
			{
				bRet = FALSE;
				goto _END_OF_DOWNLOADURL;
			}

			//��γ��������ļ�  
			dwDownFileTotalBytes = 0;
			try
			{
				while (1)
				{
					dwDownBytes = 0;
					boost::this_thread::interruption_point();
					memset(pBuf, 0, DOWNLOAD_BUF_SIZE*sizeof(char));
					bRet = InternetReadFile(hRequestGet, pBuf, DOWNLOAD_BUF_SIZE, &dwDownBytes);
					if (bRet)
					{

						if (dwDownBytes > 0)
						{
							dwDownFileTotalBytes += dwDownBytes;	
							ATLTRACE("downsize %d\n", dwDownFileTotalBytes);
							bRet = WriteFile(hFileWrite, pBuf, dwDownBytes, &dwWriteBytes, NULL); //д���ļ�  
							if (!bRet)
							{
								goto _END_OF_DOWNLOADURL;
							}
						}
						else if (0 == dwDownBytes)
						{
							ATLTRACE("%s�������", strFileName.c_str());
							bRet = TRUE;
							break; //���سɹ����  
						}
					}
				}
			}
			catch (boost::thread_exception& e)
			{
				OutputDebugStringA(e.what());
				goto _END_OF_DOWNLOADURL;
			}
			

			//����  
		_END_OF_DOWNLOADURL:
			if (INVALID_HANDLE_VALUE != hFileWrite)
				CloseHandle(hFileWrite);
			if (pBuf)
				delete[] pBuf;
			if (hRequestGet)
				InternetCloseHandle(hRequestGet);
			if (hRequestHead)
				InternetCloseHandle(hRequestHead);
			if (hConnect)
				InternetCloseHandle(hConnect);
			if (hInet)
				InternetCloseHandle(hInet);
			TAGDOWNLOADNOTIFYSTRUCT downMessage;
			downMessage.path = strFileName;
			downMessage.url = strUrl;
			ATLTRACE("download finished\n");
			SendMessage(hWnd, 2027, (WPARAM)&downMessage, (LPARAM)bRet);
			
			return bRet;
		}
	private:
		//��η���������  
		static BOOL _TryHttpSendRequest(LPVOID hRequest, int nMaxTryTimes)
		{
			BOOL bRet = FALSE;
			DWORD dwStatusCode = 0;
			DWORD dwSizeDW = sizeof(DWORD);
			while (hRequest && (nMaxTryTimes-- > 0)) //��γ��Է�������  
			{
				//��������  
				bRet = HttpSendRequestA(hRequest, NULL, 0, NULL, 0);
				if (!bRet)
				{
					continue;
				}
				else
				{
					//�ж�HTTP���ص�״̬��  
					dwStatusCode = 0;
					dwSizeDW = sizeof(DWORD);
					bRet = HttpQueryInfo(hRequest, HTTP_QUERY_FLAG_NUMBER | HTTP_QUERY_STATUS_CODE, &dwStatusCode, &dwSizeDW, NULL);
					if (bRet)
					{
						//���״̬��  
						if (HTTP_STATUS_OK == dwStatusCode) //200 OK  
						{
							break;
						}
						else
						{
							bRet = FALSE;
							continue;
						}
					}
				}
			}

			return bRet;
		}
	};
}
#endif // !COMMON_H