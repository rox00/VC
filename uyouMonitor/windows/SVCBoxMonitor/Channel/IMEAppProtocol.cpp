#include "stdafx.h"
#include "IMEAppProtocol.h"
#include "json/json.h"

void IMEAppProtocol::InitialInstance()
{
	typedef boost::bimap<EM_OPER_TYPE, string>::value_type BiMapItem;
	m_mapCommOperationName.insert(BiMapItem(EMV_OPER_HOME, "keyDownHome"));
	m_mapCommOperationName.insert(BiMapItem(EMV_OPER_RETURN, "keyDownBack"));
	m_mapCommOperationName.insert(BiMapItem(EMV_OPER_TASKMGR, "keyDownMenu"));
	m_mapCommOperationName.insert(BiMapItem(EMV_OPER_INPUT_CHAR, "keyDownChar"));
	m_mapCommOperationName.insert(BiMapItem(EMV_OPER_INPUT_KEY, "keySpecial"));
	m_mapCommOperationName.insert(BiMapItem(EMV_OPER_SDCARD_SPACE, "getSDCardSpace"));
	m_mapCommOperationName.insert(BiMapItem(EMV_OPER_GETXY, "getMobileDisplay"));		//��֪PC��android���ӵ���Ļ��Ⱥ͸߶�
	m_mapCommOperationName.insert(BiMapItem(EMV_OPER_DOWNLOADAPP, "downloadapp"));
	m_mapCommOperationName.insert(BiMapItem(EMV_OPER_INPUTM, "getImeStatus"));			//��ʾPC���Ƿ�Ҫ�����뷨
	m_mapCommOperationName.insert(BiMapItem(EMV_OPER_DOWNLOADAPPPROG, "getdownprogress"));
	m_mapCommOperationName.insert(BiMapItem(EMV_OPER_DOWNLOADPAUSE, "downloadPause"));
	m_mapCommOperationName.insert(BiMapItem(EMV_OPER_DOWNLOADFINISH, "downloadFinish"));
	m_mapCommOperationName.insert(BiMapItem(EMV_OPER_HEARTBEAT, "KEEP-BXBF-TEXT"));
	m_mapCommOperationName.insert(BiMapItem(EMV_OPER_APPRUNNING, "appRunning"));		//����android�����е�app�����Ѱ�����app���Ƹ�֪PC�ˣ��Ա�������Ӧ�ļ���ӳ��
	m_mapCommOperationName.insert(BiMapItem(EMV_OPER_OPENAPP, "openApp"));				//BOX��APK
	m_mapCommOperationName.insert(BiMapItem(EMV_OPER_UPGRADE, "getUpdateInfo"));		//PC֪ͨBOX��ѯ������Ϣ
	m_mapCommOperationName.insert(BiMapItem(EMV_ANDROID_TO_PC_UPGRADEINFO, "android_to_pc_upgradeinfo"));//BOX������Ϣ
	m_mapCommOperationName.insert(BiMapItem(EMV_PC_TO_ANDROID_PUSH_SUCESS, "pc_to_android_push_success"));//BOX�����ļ����ͳɹ�֪ͨ
	m_mapCommOperationName.insert(BiMapItem(EMV_ANDROID_TO_PC_UPGRADEFINISH, "android_to_pc_upgradefinish"));//BOX�������֪ͨ
	m_mapCommOperationName.insert(BiMapItem(EMV_OPER_OPENRECENTAPPLICATIONS, "openRecentApplications"));//�������б�
	m_mapCommOperationName.insert(BiMapItem(EMV_OPER_GETMEDIASTORAGEDIRECTORY, "getMediaStorageDirectory"));//��ȡBOXý��Ŀ¼
	m_mapCommOperationName.insert(BiMapItem(EMV_OPER_GETVOLUME, "getVolume"));//��ȡBOX����
	m_mapCommOperationName.insert(BiMapItem(EMV_OPER_SETVOLUME, "setVolume"));//����BOX����
	m_mapCommOperationName.insert(BiMapItem(EMV_OPER_REBOOT_DEVICE, "reboot"));//BOX����
	m_mapCommOperationName.insert(BiMapItem(EMV_ANDROID_TO_PC_UPGRADE_EXCEPTION, "android_to_pc_upgrade_exception"));//BOX�����쳣֪ͨ
}

string IMEAppProtocol::ToString(IMEAppMessage Message)
{
	EM_OPER_TYPE emType =  Message.first;
	LPCSTR lpszData = Message.second.c_str();

	auto iter = m_mapCommOperationName.left.find(emType);
	if(iter == m_mapCommOperationName.left.end())
	{
		ASSERT(FALSE);
		throw std::exception(__FUNCTION__);
	}

	Json::Value tmpJson;
	tmpJson["cmd"] = iter->second;
	tmpJson["data"] = lpszData;

	Json::FastWriter writer;
	string szJson = writer.write(tmpJson);
	
	//ȥ����β��\n
	size_t endpos = szJson.find_last_not_of('\n');
	szJson = (endpos == std::string::npos) ? "" : szJson.substr(0, endpos+1);


	stringstream BufferStream;
	BufferStream << JSON_DATA_BEGIN;
	BufferStream << szJson;
	BufferStream << JSON_DATA_END"\n";
	
	return BufferStream.str();
}

boost::optional<IMEAppProtocol::IMEAppMessage> IMEAppProtocol::FromString(string szBuffer)
{
	boost::optional<IMEAppProtocol::IMEAppMessage> NullMessage(boost::none);

	//check start with 
	if(!boost::algorithm::starts_with(szBuffer, JSON_DATA_BEGIN))
		return NullMessage;

	//check end with
	if(!boost::algorithm::ends_with(szBuffer, JSON_DATA_END))
		return NullMessage;

	//trim prefix and suffix
	boost::algorithm::erase_head(szBuffer, JSON_DATA_BEGIN_SIZE);
	boost::algorithm::erase_tail(szBuffer, JSON_DATA_END_SIZE);

	//parse json
	Json::Reader reader;
	Json::Value jsonRoot;
	if(!reader.parse(szBuffer, jsonRoot))
		return NullMessage;

	if (!jsonRoot.empty()){
		if (jsonRoot["cmd"].empty() || jsonRoot["data"].empty()){
			return NullMessage;
		}
		auto iter = m_mapCommOperationName.right.find(jsonRoot["cmd"].asCString());
		if (iter == m_mapCommOperationName.right.end())
			return NullMessage;

		return IMEAppMessage(iter->second, jsonRoot["data"].asString());
	}

	return NullMessage;
}

boost::optional<vector<string>> IMEAppProtocol::ParseMessage(const EM_OPER_TYPE MessageType, LPCSTR lpszData)
{
	boost::optional<vector<string>> ParsedData(boost::none);
	vector<string> ParseResult;

	boost::regex DataPattern;

	switch(MessageType)
	{
	case EMV_OPER_APPRUNNING:
		DataPattern = "([^=]+)=([^=]+)";
		break;
	case EMV_OPER_GETXY:
		DataPattern = "(\\d+)\\*(\\d+)";
		break;
	case EMV_OPER_INPUTM:
		ParsedData = ParseResult;
		ParsedData->push_back(strcmp(lpszData, "false") ? "1" : "0");
		break;
	case EMV_OPER_SDCARD_SPACE:
		DataPattern = "(\\d+)/(\\d+)";
		break;
	default:
		break;
	}

	boost::cmatch DataMatch;
	if(!DataPattern.empty() && boost::regex_match(lpszData, DataMatch, DataPattern))
	{
		ParsedData = ParseResult;
		for(auto iter = DataMatch.begin(); iter != DataMatch.end(); iter++)
		{
			if(iter == DataMatch.begin())
				continue;
			auto MatchedData = *iter;
			if(MatchedData.matched)
				ParsedData->push_back(MatchedData.str());
		}
	}
	else{
		std::string sss(lpszData);
		ParsedData = ParseResult;
		ParsedData->push_back(sss);
	}
	return ParsedData;
}