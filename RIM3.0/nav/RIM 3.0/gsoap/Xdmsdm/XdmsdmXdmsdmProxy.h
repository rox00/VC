/* XdmsdmXdmsdmProxy.h
   Generated by gSOAP 2.8.2 from Xdmsdm.h
   Copyright(C) 2000-2011, Robert van Engelen, Genivia Inc. All Rights Reserved.
   The generated code is released under one of the following licenses:
   GPL OR Genivia's license for commercial use.
*/

#ifndef XdmsdmXdmsdmProxy_H
#define XdmsdmXdmsdmProxy_H
#include "XdmsdmH.h"

namespace Xdmsdm {

class SOAP_CMAC XdmsdmProxy
{ public:
	struct soap *soap;
	bool own;
	/// Endpoint URL of service 'XdmsdmProxy' (change as needed)
	const char *soap_endpoint;
	/// Constructor
	XdmsdmProxy();
	/// Constructor to use/share an engine state
	XdmsdmProxy(struct soap*);
	/// Constructor with endpoint URL
	XdmsdmProxy(const char *url);
	/// Constructor with engine input+output mode control
	XdmsdmProxy(soap_mode iomode);
	/// Constructor with URL and input+output mode control
	XdmsdmProxy(const char *url, soap_mode iomode);
	/// Constructor with engine input and output mode control
	XdmsdmProxy(soap_mode imode, soap_mode omode);
	/// Destructor frees deserialized data
	virtual	~XdmsdmProxy();
	/// Initializer used by constructors
	virtual	void XdmsdmProxy_init(soap_mode imode, soap_mode omode);
	/// Delete all deserialized data (uses soap_destroy and soap_end)
	virtual	void destroy();
	/// Disables and removes SOAP Header from message
	virtual	void soap_noheader();
	/// Get SOAP Fault structure (NULL when absent)
	virtual	const SOAP_ENV__Fault *soap_fault();
	/// Get SOAP Fault string (NULL when absent)
	virtual	const char *soap_fault_string();
	/// Get SOAP Fault detail as string (NULL when absent)
	virtual	const char *soap_fault_detail();
	/// Force close connection (normally automatic, except for send_X ops)
	virtual	int soap_close_socket();
	/// Print fault
	virtual	void soap_print_fault(FILE*);
#ifndef WITH_LEAN
	/// Print fault to stream
	virtual	void soap_stream_fault(std::ostream&);
	/// Put fault into buffer
	virtual	char *soap_sprint_fault(char *buf, size_t len);
#endif

	/// Web service operation 'QueryVersionInfo' (returns error code or SOAP_OK)
	virtual	int QueryVersionInfo(_Xdmsdm__QueryVersionInfo *Xdmsdm__QueryVersionInfo, _Xdmsdm__QueryVersionInfoResponse *Xdmsdm__QueryVersionInfoResponse);

	/// Web service operation 'QueryRegionNO' (returns error code or SOAP_OK)
	virtual	int QueryRegionNO(_Xdmsdm__QueryRegionNO *Xdmsdm__QueryRegionNO, _Xdmsdm__QueryRegionNOResponse *Xdmsdm__QueryRegionNOResponse);

	/// Web service operation 'QueryFreqLayout' (returns error code or SOAP_OK)
	virtual	int QueryFreqLayout(_Xdmsdm__QueryFreqLayout *Xdmsdm__QueryFreqLayout, _Xdmsdm__QueryFreqLayoutResponse *Xdmsdm__QueryFreqLayoutResponse);

	/// Web service operation 'QueryWebService' (returns error code or SOAP_OK)
	virtual	int QueryWebService(_Xdmsdm__QueryWebService *Xdmsdm__QueryWebService, _Xdmsdm__QueryWebServiceResponse *Xdmsdm__QueryWebServiceResponse);

	/// Web service operation 'AuthUser' (returns error code or SOAP_OK)
	virtual	int AuthUser(_Xdmsdm__AuthUser *Xdmsdm__AuthUser, _Xdmsdm__AuthUserResponse *Xdmsdm__AuthUserResponse);

	/// Web service operation 'AuthUserLicenseFunc' (returns error code or SOAP_OK)
	virtual	int AuthUserLicenseFunc(_Xdmsdm__AuthUserLicenseFunc *Xdmsdm__AuthUserLicenseFunc, _Xdmsdm__AuthUserLicenseFuncResponse *Xdmsdm__AuthUserLicenseFuncResponse);

	/// Web service operation 'QueryUserLicenseFunc' (returns error code or SOAP_OK)
	virtual	int QueryUserLicenseFunc(_Xdmsdm__QueryUserLicenseFunc *Xdmsdm__QueryUserLicenseFunc, _Xdmsdm__QueryUserLicenseFuncResponse *Xdmsdm__QueryUserLicenseFuncResponse);

	/// Web service operation 'SubmitOperateLog' (returns error code or SOAP_OK)
	virtual	int SubmitOperateLog(_Xdmsdm__SubmitOperateLog *Xdmsdm__SubmitOperateLog, _Xdmsdm__SubmitOperateLogResponse *Xdmsdm__SubmitOperateLogResponse);

	/// Web service operation 'QueryStandardTime' (returns error code or SOAP_OK)
	virtual	int QueryStandardTime(_Xdmsdm__QueryStandardTime *Xdmsdm__QueryStandardTime, _Xdmsdm__QueryStandardTimeResponse *Xdmsdm__QueryStandardTimeResponse);

	/// Web service operation 'QueryUserMobileNO' (returns error code or SOAP_OK)
	virtual	int QueryUserMobileNO(_Xdmsdm__QueryUserMobileNO *Xdmsdm__QueryUserMobileNO, _Xdmsdm__QueryUserMobileNOResponse *Xdmsdm__QueryUserMobileNOResponse);

	/// Web service operation 'SendMobileMessage' (returns error code or SOAP_OK)
	virtual	int SendMobileMessage(_Xdmsdm__SendMobileMessage *Xdmsdm__SendMobileMessage, _Xdmsdm__SendMobileMessageResponse *Xdmsdm__SendMobileMessageResponse);

	/// Web service operation 'QueryUserInfo' (returns error code or SOAP_OK)
	virtual	int QueryUserInfo(_Xdmsdm__QueryUserInfo *Xdmsdm__QueryUserInfo, _Xdmsdm__QueryUserInfoResponse *Xdmsdm__QueryUserInfoResponse);

	/// Web service operation 'SetMonthReportWorkPlans' (returns error code or SOAP_OK)
	virtual	int SetMonthReportWorkPlans(_Xdmsdm__SetMonthReportWorkPlans *Xdmsdm__SetMonthReportWorkPlans, _Xdmsdm__SetMonthReportWorkPlansResponse *Xdmsdm__SetMonthReportWorkPlansResponse);

	/// Web service operation 'GetMonthReportWorkPlans' (returns error code or SOAP_OK)
	virtual	int GetMonthReportWorkPlans(_Xdmsdm__GetMonthReportWorkPlans *Xdmsdm__GetMonthReportWorkPlans, _Xdmsdm__GetMonthReportWorkPlansResponse *Xdmsdm__GetMonthReportWorkPlansResponse);

	/// Web service operation 'SetMonthReportBusFreqs' (returns error code or SOAP_OK)
	virtual	int SetMonthReportBusFreqs(_Xdmsdm__SetMonthReportBusFreqs *Xdmsdm__SetMonthReportBusFreqs, _Xdmsdm__SetMonthReportBusFreqsResponse *Xdmsdm__SetMonthReportBusFreqsResponse);

	/// Web service operation 'GetMonthReportBusFreqs' (returns error code or SOAP_OK)
	virtual	int GetMonthReportBusFreqs(_Xdmsdm__GetMonthReportBusFreqs *Xdmsdm__GetMonthReportBusFreqs, _Xdmsdm__GetMonthReportBusFreqsResponse *Xdmsdm__GetMonthReportBusFreqsResponse);
};

} // namespace Xdmsdm

#endif
