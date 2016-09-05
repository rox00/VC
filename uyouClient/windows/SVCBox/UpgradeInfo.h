#pragma once
#ifndef UPGRADEINFO_H
#define UPGRADEINFO_H
#include <string>
#include "../Common/Common.h"

class UpgradeInfo
{
public:
	std::string vername;
	std::string vervalue;
	std::string type;
	std::string description;
	std::string downloadurl;
	std::string mand;
public:
	UpgradeInfo(){};
	~UpgradeInfo(){};
	static UpgradeInfo* getInstance(Json::Value data){
		if (data.isMember("vername")
			&& data.isMember("vervalue")
			&& data.isMember("type")
			&& data.isMember("description")
			&& data.isMember("downloadurl")
			&& data.isMember("mand")
			)
		{
			UpgradeInfo *pUpgradeInfo = new UpgradeInfo();
			pUpgradeInfo->description = data["description"].asString();
			pUpgradeInfo->downloadurl = data["downloadurl"].asString();
			pUpgradeInfo->mand = data["mand"].asString();
			pUpgradeInfo->type = data["type"].asString();
			pUpgradeInfo->vername = data["vername"].asString();
			pUpgradeInfo->vervalue = data["vervalue"].asString();
			return pUpgradeInfo;
		}
		return NULL;
	}

	UpgradeInfo(UpgradeInfo &upgradeInfo){
		description = upgradeInfo.description;
		vervalue = upgradeInfo.vervalue;
		vername = upgradeInfo.vername;
		type = upgradeInfo.type;
		mand = upgradeInfo.mand;
		downloadurl = upgradeInfo.downloadurl;
	}
};
#endif


