//********************************************************************
// SQLITEDATADEFINE.H �ļ�ע��
// �ļ��� ��: SQLITEDATADEFINE.H
// �ļ�·�� : E:\PROJECT\RIM3.0\SRC\SDK\RIM30TOOLKIT/
// ���� ����: ����
// ����ʱ�� : 2015/4/23 14:28
// �ļ����� : 
//*********************************************************************
#pragma once

/*******************************

Copyright (C), 2008-2010, Rongxing company.

Filename:		 
Author:		    ��С��
version:		1.0
Date:			2010-3-20

Description:    BOSD(Business Operation Support Data)���еĺ궨�弰��ṹ����

*******************************/
/** �������� */
#define BOSD_T_RIM_COM_REGION_NO                                  				"T_RIM_COM_REGION_NO"
#define BOSD_T_RIM_COM_REGION_NO_COLUMN_NO                        				"REGIONNO"
#define BOSD_T_RIM_COM_REGION_NO_COLUMN_NAME                      				"REGIONNAME" 
/** ϵͳ������Ȩ�� */
#define BOSD_T_RIM_COM_AUTH_SYS_FUNC                              				"T_RIM_COM_AUTH_SYS_FUNC"
#define BOSD_T_RIM_COM_AUTH_SYS_FUNC_COLUMN_NO                    				"SYSFUNCNO"
#define BOSD_T_RIM_COM_AUTH_SYS_FUNC_COLUMN_NOTE                  				"NOTE" 
/** ϵͳվ��������Ȩ�� */
#define BOSD_T_RIM_COM_AUTH_SYS_DATA_STATION                      				"T_RIM_COM_AUTH_SYS_DATA_STATION"
#define BOSD_T_RIM_COM_AUTH_SYS_DATA_STATION_COLUMN_NO            				"STATIONNO"
#define BOSD_T_RIM_COM_AUTH_SYS_DATA_STATION_COLUMN_NOTE          				"NOTE" 
/** Ƶ�ʹ滮�� */
#define BOSD_T_RIM_COM_FREQ_LAYOUT                              				"T_RIM_COM_FREQ_LAYOUT"
#define BOSD_T_RIM_COM_FREQ_LAYOUT_COLUMN_LAYOUTNAME            				"LAYOUTNAME"
#define BOSD_T_RIM_COM_FREQ_LAYOUT_COLUMN_STARTFREQ             				"STARTFREQ" 
#define BOSD_T_RIM_COM_FREQ_LAYOUT_COLUMN_ENDFREQ               				"ENDFREQ"
#define BOSD_T_RIM_COM_FREQ_LAYOUT_COLUMN_GROUPNAME             				"GROUPNAME" 
#define BOSD_T_RIM_COM_FREQ_LAYOUT_COLUMN_DISTINCE              				"DISTINCE"
#define BOSD_T_RIM_COM_FREQ_LAYOUT_COLUMN_EMITWIDTH             				"EMITWIDTH" 

/** ���վ���ͱ� */
#define BOSD_T_RIM_RTM_REF_STATION_TYPE                           				"T_RIM_RTM_REF_STATION_TYPE"
#define BOSD_T_RIM_RTM_REF_STATION_TYPE_COLUMN_NOID               				"NOID"
#define BOSD_T_RIM_RTM_REF_STATION_TYPE_COLUMN_NAME               				"NAME"
/** ���վ�豸�����(����豸,����豸��) */
#define BOSD_T_RIM_RTM_REF_DEVICE_TYPE                            				"T_RIM_RTM_REF_DEVICE_TYPE"
#define BOSD_T_RIM_RTM_REF_DEVICE_TYPE_COLUMN_NOID                				"NOID" 
#define BOSD_T_RIM_RTM_REF_DEVICE_TYPE_COLUMN_NAME                				"NAME"
/** ���վ�豸�����(����豸,����豸��) */
#define BOSD_T_RIM_RTM_REF_DEVICE_PROTOCOL                        				"T_RIM_RTM_REF_DEVICE_PROTOCOL"
#define BOSD_T_RIM_RTM_REF_DEVICE_PROTOCOL_COLUMN_NOID            				"NOID"
#define BOSD_T_RIM_RTM_REF_DEVICE_PROTOCOL_COLUMN_NAME            				"NAME" 
#define BOSD_T_RIM_RTM_REF_DEVICE_PROTOCOL_COLUMN_NOTE            				"NOTE" 
/** ���վ�豸���ͱ� */
#define BOSD_T_RIM_RTM_REF_DEVICE_MODEL                           				"T_RIM_RTM_REF_DEVICE_MODEL"
#define BOSD_T_RIM_RTM_REF_DEVICE_MODEL_COLUMN_NOID               				"NOID"
#define BOSD_T_RIM_RTM_REF_DEVICE_MODEL_COLUMN_NAME               				"NAME"
#define BOSD_T_RIM_RTM_REF_DEVICE_MODEL_COLUMN_NOTE               				"NOTE" 
/** ��⹦�����ͱ� */
#define BOSD_T_RIM_RTM_REF_DEVICE_FUNC                            				"T_RIM_RTM_REF_DEVICE_FUNC"
#define BOSD_T_RIM_RTM_REF_DEVICE_FUNC_COLUMN_NOID                				"NOID"
#define BOSD_T_RIM_RTM_REF_DEVICE_FUNC_COLUMN_NAME                				"NAME"
#define BOSD_T_RIM_RTM_REF_DEVICE_FUNC_COLUMN_KEY                 				"KEYWORD"
/** ����豸���ܲ��� */
#define BOSD_T_RIM_RTM_REF_DEVICE_MODEL_FUNC_PARAM                				"T_RIM_RTM_REF_DEVICE_MODEL_FUNC_PARAM"
#define BOSD_T_RIM_RTM_REF_DEVICE_MODEL_FUNC_PARAM_COLUMN_DEVICE  				"DEVICE"
#define BOSD_T_RIM_RTM_REF_DEVICE_MODEL_FUNC_PARAM_COLUMN_FUNC    				"FUNC"
#define BOSD_T_RIM_RTM_REF_DEVICE_MODEL_FUNC_PARAM_COLUMN_PARAM   				"PARAM"

/** �����֯������ */
#define BOSD_T_RIM_RTM_MON_ORG                      							"T_RIM_RTM_MON_ORG"
#define BOSD_T_RIM_RTM_MON_ORG_COLUMN_NO            							"MONORGNO"
#define BOSD_T_RIM_RTM_MON_ORG_COLUMN_NAME          							"MONORGNAME"
#define BOSD_T_RIM_RTM_MON_ORG_COLUMN_ADDRESS       							"ADDRESS"
#define BOSD_T_RIM_RTM_MON_ORG_COLUMN_POST          							"POST"
#define BOSD_T_RIM_RTM_MON_ORG_COLUMN_RESPONPERSON  							"RESPONPERSON"
#define BOSD_T_RIM_RTM_MON_ORG_COLUMN_CONTACT       							"CONTACT"
#define BOSD_T_RIM_RTM_MON_ORG_COLUMN_BRIEF         							"BRIEF" 
#define BOSD_T_RIM_RTM_MON_ORG_COLUMN_NOTE          							"NOTE"

/** ���������ı� */
#define BOSD_T_RIM_RTM_MON_CONTROL_CENTER                      					"T_RIM_RTM_MON_CONTRL_CENTER"
#define BOSD_T_RIM_RTM_MON_CONTROL_CENTER_COLUMN_NAME          					"NAME"
#define BOSD_T_RIM_RTM_MON_CONTROL_CENTER_COLUMN_AREACODE      					"AREACODE"
#define BOSD_T_RIM_RTM_MON_CONTROL_CENTER_COLUMN_ADDRESS       					"ADDRESS"
#define BOSD_T_RIM_RTM_MON_CONTROL_CENTER_COLUMN_LONG          					"LONG"
#define BOSD_T_RIM_RTM_MON_CONTROL_CENTER_COLUMN_LATI          					"LATI"
#define BOSD_T_RIM_RTM_MON_CONTROL_CENTER_COLUMN_NOTE          					"NOTE"

/** ���վ�� */
#define BOSD_T_RIM_RTM_STATION                              					"T_RIM_RTM_STATION"
#define BOSD_T_RIM_RTM_STATION_COLUMN_STATION_NO            					"STATIONNO"
#define BOSD_T_RIM_RTM_STATION_COLUMN_NAME                  					"STATIONNAME"
#define BOSD_T_RIM_RTM_STATION_COLUMN_CREATETIME            					"CREATETIME"
#define BOSD_T_RIM_RTM_STATION_COLUMN_ADDRESS               					"ADDRESS"
#define BOSD_T_RIM_RTM_STATION_COLUMN_LONGITUDE             					"LONGITUDE"
#define BOSD_T_RIM_RTM_STATION_COLUMN_LATITUDE              					"LATITUDE"
#define BOSD_T_RIM_RTM_STATION_COLUMN_MONRANGE              					"MONRANGE"
#define BOSD_T_RIM_RTM_STATION_COLUMN_TYPE                  					"TYPE"
#define BOSD_T_RIM_RTM_STATION_COLUMN_CONTACT               					"CONTACT"
#define BOSD_T_RIM_RTM_STATION_COLUMN_DESC                  					"DESC"
#define BOSD_T_RIM_RTM_STATION_COLUMN_GPSADDR               					"GPSADDR"
#define BOSD_T_RIM_RTM_STATION_COLUMN_GPSPORT               					"GPSPORT"
#define BOSD_T_RIM_RTM_STATION_COLUMN_COMPASSADDR           					"COMPASSADDR"
#define BOSD_T_RIM_RTM_STATION_COLUMN_COMPASSPORT           					"COMPASSPORT"
#define BOSD_T_RIM_RTM_STATION_COLUMN_GRIDNO                					"GRIDNO"
#define BOSD_T_RIM_RTM_STATION_COLUMN_NETTYPE               					"NETTYPE"
/** ����豸 */
#define BOSD_T_RIM_RTM_DEVICE                                   				"T_RIM_RTM_DEVICE"
#define BOSD_T_RIM_RTM_DEVICE_COLUMN_STATIONNO                  				"STATIONNO"         /** ���վ���(8λ) */
#define BOSD_T_RIM_RTM_DEVICE_COLUMN_DEVNO                      				"DEVICENO"                /** 5λ��� */
#define BOSD_T_RIM_RTM_DEVICE_COLUMN_MODEL                      				"MODEL"             /** �豸������ͺ� */
#define BOSD_T_RIM_RTM_DEVICE_COLUMN_NAME                       				"ALIASNAME"         /** �豸���� */
#define BOSD_T_RIM_RTM_DEVICE_COLUMN_ESC                        				"DESC"              /** �豸���� */ 
#define BOSD_T_RIM_RTM_DEVICE_COLUMN_TYPE                       				"TYPE"              /** �豸���� */          
#define BOSD_T_RIM_RTM_DEVICE_COLUMN_BUYDATE                    				"BUYDATE"           /** �������� */
#define BOSD_T_RIM_RTM_DEVICE_COLUMN_SUPPLIER                   				"SUPPLIER"          /** ��Ӧ�� */
#define BOSD_T_RIM_RTM_DEVICE_COLUMN_REPAIRTEL                  				"REPAIRTEL"         /** ά�޵绰 */
#define BOSD_T_RIM_RTM_DEVICE_COLUMN_CURADDRESS                 				"CURADDRESS"        /** ��ǰ��ַ */
#define BOSD_T_RIM_RTM_DEVICE_COLUMN_CURSTATE                   				"CURSTATE"          /** ��ǰ״̬ */
#define BOSD_T_RIM_RTM_DEVICE_COLUMN_RESPONER                   				"RESPONER"
#define BOSD_T_RIM_RTM_DEVICE_COLUMN_MAXRUNNUM                  				"MAXRUNNUM"
#define BOSD_T_RIM_RTM_DEVICE_COLUMN_SERVICEIP                  				"SERVICEIP"
#define BOSD_T_RIM_RTM_DEVICE_COLUMN_SERVICEPORT                				"SERVICEPORT"
#define BOSD_T_RIM_RTM_DEVICE_COLUMN_DEVPROTOCOL                				"DEVPROTOCOL"
#define BOSD_T_RIM_RTM_DEVICE_COLUMN_ISINTEGRATED               				"ISINTEGRATED"

/** �豸�������߱� */
#define BOSD_T_RIM_RTM_ANTENNA                                  				"T_RIM_RTM_ANTENNA"
#define BOSD_T_RIM_RTM_ANTENNA_COLUMN_STATIONNO                 				"STATIONNO"
#define BOSD_T_RIM_RTM_ANTENNA_COLUMN_ANTNAME                   				"ANTNAME"
#define BOSD_T_RIM_RTM_ANTENNA_COLUMN_ANTFACTOR                 				"ANTFACTOR"
#define BOSD_T_RIM_RTM_ANTENNA_COLUMN_STARTFREQ                 				"STARTFREQ"
#define BOSD_T_RIM_RTM_ANTENNA_COLUMN_STOPFREQ                  				"STOPFREQ"
#define BOSD_T_RIM_RTM_ANTENNA_COLUMN_ANTHEIGHT                 				"ANTHEIGHT"

/** ���վ��Դ������ */
#define BOSD_T_RIM_RTM_RNC                                  					"T_RIM_RTM_RNC"
#define BOSD_T_RIM_RTM_RNC_COLUMN_STATIONNO                 					"STATIONNO"
#define BOSD_T_RIM_RTM_RNC_COLUMN_RNCNAME                   					"RNCNAME"
#define BOSD_T_RIM_RTM_RNC_COLUMN_RNCIP                     					"RNCIP"
#define BOSD_T_RIM_RTM_RNC_COLUMN_RNCPORT                   					"RNCPORT"
#define BOSD_T_RIM_RTM_RNC_COLUMN_CTRLDEVICENAME            					"CTRLDEVICENAME"
#define BOSD_T_RIM_RTM_RNC_COLUMN_CTRLPORT                  					"CTRLPORT"
#define BOSD_T_RIM_RTM_RNC_COLUMN_CTRLOSHAS                 					"CTRLOSHAS"
#define BOSD_T_RIM_RTM_RNC_COLUMN_CTRLOSIP                  					"CTRLOSIP"
#define BOSD_T_RIM_RTM_RNC_COLUMN_CTRLOSPORT                					"CTRLOSPORT"


/** �豸�������߱� */
#define BOSD_T_RIM_RTM_DEVICE_FUNC_ANTENNA                      				"T_RIM_RTM_DEVICE_FUNC_ANTENNA"
#define BOSD_T_RIM_RTM_DEVICE_FUNC_ANTENNA_COLUMN_STATIONNO     				"STATIONNO"
#define BOSD_T_RIM_RTM_DEVICE_FUNC_ANTENNA_COLUMN_DEVICENO      				"DEVICENO"
#define BOSD_T_RIM_RTM_DEVICE_FUNC_ANTENNA_COLUMN_FUNCNO        				"FUNCNO"
#define BOSD_T_RIM_RTM_DEVICE_FUNC_ANTENNA_COLUMN_ANTKEYWORD    				"ANTKEYWORD"
#define BOSD_T_RIM_RTM_DEVICE_FUNC_ANTENNA_COLUMN_ANTNAME       				"ANTNAME"

/** �豸���ܱ� */
#define BOSD_T_RIM_RTM_DEVICE_FUNC                              				"T_RIM_RTM_DEVICE_FUNC"
#define BOSD_T_RIM_RTM_DEVICE_FUNC_COLUMN_STATIONNO             				"STATIONNO"
#define BOSD_T_RIM_RTM_DEVICE_FUNC_COLUMN_DEVICENO              				"DEVICENO"
#define BOSD_T_RIM_RTM_DEVICE_FUNC_COLUMN_FUNCNO                				"FUNCNO"

/** �豸���ܲ��� */
#define BOSD_T_RIM_FAVORITES_RTM_PARAM_CONFIG                      				"T_RIM_FAVORITES_RTM_PARAM_CONFIG"
#define BOSD_T_RIM_FAVORITES_RTM_PARAM_CONFIG_COLUMN_NAME          				"NAME"
#define BOSD_T_RIM_FAVORITES_RTM_PARAM_CONFIG_COLUMN_STATIONNO     				"STATIONNO"
#define BOSD_T_RIM_FAVORITES_RTM_PARAM_CONFIG_COLUMN_DEVICENO      				"DEVICENO"
#define BOSD_T_RIM_FAVORITES_RTM_PARAM_CONFIG_COLUMN_FUNCNO        				"FUNCNO"
#define BOSD_T_RIM_FAVORITES_RTM_PARAM_CONFIG_COLUMN_PARAM         				"PARAM"


/** ̨վӳ��� */
#define BOSD_T_RIM_MGM_GRID_BIND_EMIT_STATION                      				"T_RIM_MGM_GRID_BIND_EMIT_STATION"
#define BOSD_T_RIM_MGM_GRID_BIND_EMIT_STATION_COLUMN_GRIDUID       				"GRIDUID"
#define BOSD_T_RIM_MGM_GRID_BIND_EMIT_STATION_COLUMN_EMITSTATIONUID				"EMITSTATIONUID"

/** ̨վ�� */
#define BOSD_T_RIM_SMS_EMIT_STATION												"T_RIM_SMS_EMIT_STATION"
#define BOSD_T_RIM_SMS_EMIT_STATION_COLUMN_GUID          						"GUID"					//̨վID
#define BOSD_T_RIM_SMS_EMIT_STATION_COLUMN_APPLYCODE     						"APPLYCODE"				//�������
#define BOSD_T_RIM_SMS_EMIT_STATION_COLUMN_DECLCODE      						"DECLCODE"				//�������ϱ���
#define BOSD_T_RIM_SMS_EMIT_STATION_COLUMN_DEPARTMENT    						"DEPARTMENT"			//̨վ��ַ
#define BOSD_T_RIM_SMS_EMIT_STATION_COLUMN_STATADDR      						"STATADDR"    			//̨վ��ַ
#define BOSD_T_RIM_SMS_EMIT_STATION_COLUMN_STATNAME      						"STATNAME"    			//̨վ����
#define BOSD_T_RIM_SMS_EMIT_STATION_COLUMN_STATTYPE      						"STATTYPE"    			//̨վ����
#define BOSD_T_RIM_SMS_EMIT_STATION_COLUMN_LONGI         						"LONGI"					//����
#define BOSD_T_RIM_SMS_EMIT_STATION_COLUMN_LATI          						"LATI"					//γ��
#define BOSD_T_RIM_SMS_EMIT_STATION_COLUMN_FREQEMIT      						"FREQEMIT"				//����Ƶ��
#define BOSD_T_RIM_SMS_EMIT_STATION_COLUMN_FREQEMITLOW   						"FREQEMITLOW"			//����Ƶ������
#define BOSD_T_RIM_SMS_EMIT_STATION_COLUMN_FREQEMITUP    						"FREQEMITUP"			//����Ƶ������
#define BOSD_T_RIM_SMS_EMIT_STATION_COLUMN_SMODULATION   						"MODULATION"			//���Ʒ�ʽ
#define BOSD_T_RIM_SMS_EMIT_STATION_COLUMN_ALTITUDE      						"ALTITUDE"				//���θ߶�


/** �������� */
#define BOSD_T_RIM_MGM_MGR_GRID                                     			"T_RIM_MGM_MGR_GRID"
#define BOSD_T_RIM_MGM_MGR_GRID_COLUMN_REFLATI        							"REFLATI"             	/** ��������Ĳο�γ�� */
#define BOSD_T_RIM_MGM_MGR_GRID_COLUMN_REFLONG        							"REFLONG"             	/** ��������Ĳο����� */
#define BOSD_T_RIM_MGM_MGR_GRID_COLUMN_SIDE           							"SIDE"                	/** �߳� */
#define BOSD_T_RIM_MGM_MGR_GRID_COLUMN_LAYERS         							"LAYERCOUNT"          	/** ͼ������ */
#define BOSD_T_RIM_MGM_MGR_GRID_COLUMN_NOTE           							"NOTE"                	/** ��ע */
#define BOSD_T_RIM_MGM_MGR_GRID_COLUMN_RESERVE        							"RESERVE"             	/** Ԥ�� */
#define BOSD_T_RIM_MGM_MGR_GRID_COLUMN_ISBINDES       							"ISBINDES"            	/** �Ƿ��Ѿ���̨վ */
#define BOSD_T_RIM_MGM_MGR_GRID_COLUMN_LASTUPDATEDATE 							"LASTUPDATEDATE"      	/** ������ʱ�� */

/**�����������Ͳ��ձ� */
#define BOSD_T_RIM_MGM_GRID_DATA_REF                                			"T_RIM_MGM_GRID_DATA_REF"
#define BOSD_T_RIM_MGM_GRID_DATA_REF_COLUMN_TYPE       							"TYPE"               	/** �������� */
#define BOSD_T_RIM_MGM_GRID_DATA_REF_COLUMN_NAME       							"NAME"               	/** ������������ */

/**������������ͼ����ʾ���� */
#define BOSD_T_RIM_MGM_GRID_DATA_SHOW_LAYER_CONF                     			"T_RIM_MGM_GRID_DATA_SHOW_LAYER_CONF"
#define BOSD_T_RIM_MGM_GRID_DATA_SHOW_LAYER_CONF_COLUMN_DATE_TYPE    			"DATETYPE"           	/** �������� */
#define BOSD_T_RIM_MGM_GRID_DATA_SHOW_LAYER_CONF_COLUMN_DATA_NAME    			"DATANAME"           	/** �������� */
#define BOSD_T_RIM_MGM_GRID_DATA_SHOW_LAYER_CONF_COLUMN_LAYER_INDEX  			"LAYERINDEX"         	/** ��ʾͼ�� */

/**��������-�������� */
#define BOSD_T_RIM_MGM_GRID_DATA_BASE                         					"T_RIM_MGM_GRID_DATA_BASE"
#define BOSD_T_RIM_MGM_GRID_DATA_BASE_COLUMN_GRIDID           					"GRIDID"           		/** ����ID */
#define BOSD_T_RIM_MGM_GRID_DATA_BASE_COLUMN_RESPONER         					"RESPONER"         		/** ������ */
#define BOSD_T_RIM_MGM_GRID_DATA_BASE_COLUMN_PERSONS          					"PERSONS"          		/** �˿����� */
#define BOSD_T_RIM_MGM_GRID_DATA_BASE_COLUMN_REGION           					"REGION"           		/** ������� */
#define BOSD_T_RIM_MGM_GRID_DATA_BASE_COLUMN_NOTE             					"NOTE"             		/** ע�� */
#define BOSD_T_RIM_MGM_GRID_DATA_BASE_COLUMN_RESERVEA         					"RESERVEA"         		/** Ԥ��A */
#define BOSD_T_RIM_MGM_GRID_DATA_BASE_COLUMN_RESERVEB         					"RESERVEB"         		/** Ԥ��B */

/**��������-���� */
#define BOSD_T_RIM_MGM_GRID_DATA_GRIDNAME                        				"T_RIM_MGM_GRID_DATA_GRIDNAME"
#define BOSD_T_RIM_MGM_GRID_DATA_GRIDNAME_COLUMN_GRIDID          				"GRIDID"               	/** ����ID */
#define BOSD_T_RIM_MGM_GRID_DATA_GRIDNAME_COLUMN_NAME            				"NAME"                 	/** �������� */

/**��������-ͨ������ */
#define BOSD_T_RIM_MGM_GRID_DATA_UTIL                            				"T_RIM_MGM_GRID_DATA_UTIL"
#define BOSD_T_RIM_MGM_GRID_DATA_UTIL_COLUMN_GRIDID              				"GRIDID"               	/** ����ID */
#define BOSD_T_RIM_MGM_GRID_DATA_UTIL_COLUMN_TYPE                				"TYPE"                 	/** ������������ */
#define BOSD_T_RIM_MGM_GRID_DATA_UTIL_COLUMN_CONTEXT             				"CONTEXT"              	/** ��������XML */



/** ������� */
#define BOSD_T_RIM_MGM_MON_GRID													"T_RIM_MGM_MON_GRID"
#define BOSD_T_RIM_MGM_MON_GRID_COLUMN_GRIDID        							"MONGRIDID"     
#define BOSD_T_RIM_MGM_MON_GRID_COLUMN_MONGRIDNAME   							"MONGRIDNAME"     
#define BOSD_T_RIM_MGM_MON_GRID_COLUMN_MSTATIONNO    							"MSTATIONNO"     
#define BOSD_T_RIM_MGM_MON_GRID_COLUMN_FREQNAME      							"FREQNAME"  
#define BOSD_T_RIM_MGM_MON_GRID_COLUMN_MINFREQ       							"MINFREQ"     
#define BOSD_T_RIM_MGM_MON_GRID_COLUMN_MAXFREQ       							"MAXFREQ"     
#define BOSD_T_RIM_MGM_MON_GRID_COLUMN_POWER         							"POWER"     
#define BOSD_T_RIM_MGM_MON_GRID_COLUMN_ANTHEIGHT     							"ANTHEIGHT"     
#define BOSD_T_RIM_MGM_MON_GRID_COLUMN_SENSITIVITY   							"SENSITIVITY"  
#define BOSD_T_RIM_MGM_MON_GRID_COLUMN_EMCMODELTYPE  							"EMCMODELTYPE"    
#define BOSD_T_RIM_MGM_MON_GRID_COLUMN_EMCMODELPARAM 							"EMCMODELPARAM"
/** ҵ������ */  
#define BOSD_T_RIM_MGM_BUS_GRID                          						"T_RIM_MGM_BUS_GRID"
#define BOSD_T_RIM_MGM_BUS_GRID_COLUMN_GRIDID            						"BUSGRIDID"     
#define BOSD_T_RIM_MGM_BUS_GRID_COLUMN_BUSGRIDNAME       						"BUSGRIDNAME"         
#define BOSD_T_RIM_MGM_BUS_GRID_COLUMN_FREQNAME          						"FREQNAME"  
#define BOSD_T_RIM_MGM_BUS_GRID_COLUMN_MINFREQ           						"MINFREQ"     
#define BOSD_T_RIM_MGM_BUS_GRID_COLUMN_MAXFREQ           						"MAXFREQ"   
#define BOSD_T_RIM_MGM_BUS_GRID_COLUMN_EMITBAND          						"EMITBAND"     
#define BOSD_T_RIM_MGM_BUS_GRID_COLUMN_OCC_THRESHOLD     						"THRESHOLD"   
#define BOSD_T_RIM_MGM_BUS_GRID_COLUMN_POWER             						"POWER"     
#define BOSD_T_RIM_MGM_BUS_GRID_COLUMN_ANTHEIGHT         						"ANTHEIGHT"     
#define BOSD_T_RIM_MGM_BUS_GRID_COLUMN_SENSITIVITY       						"SENSITIVITY"  
#define BOSD_T_RIM_MGM_BUS_GRID_COLUMN_EMCMODELTYPE      						"EMCMODELTYPE"    
#define BOSD_T_RIM_MGM_BUS_GRID_COLUMN_EMCMODELPARAM     						"EMCMODELPARAM"
/** ��������󶨵�̨վ�� */
#define BOSD_T_RIM_MGM_MGRGRID_BIND_ESTATION                      				"T_RIM_MGM_BIND_MGR_INCLUDE_ESTATION"
#define BOSD_T_RIM_MGM_MGRGRID_BIND_ESTATION_COLUMN_ESTATIONUID   				"ESTATIONUID"							/** ��������ID */
#define BOSD_T_RIM_MGM_MGRGRID_BIND_ESTATION_COLUMN_MGRGRIDID     				"MGRGRIDID"								/** �������ID */
/** ҵ������󶨹������� */
#define BOSD_T_RIM_MGM_BUSGRID_BIND_MGRGRID                        				"T_RIM_MGM_BIND_BUS_INCLUDE_MGRGRID"
#define BOSD_T_RIM_MGM_BUSGRID_BIND_MGRGRID_COLUMN_MGRGRIDID       				"MGRGRIDID"                        	 	/** ��������ID */
#define BOSD_T_RIM_MGM_BUSGRID_BIND_MGRGRID_COLUMN_BUSGRIDID       				"BUSGRIDID"                        	 	/** ��������ID */
#define BOSD_T_RIM_MGM_BUSGRID_BIND_MGRGRID_COLUMN_SORTID          				"SORTID"								/** ��������ID */
/** �������ӳ��� */
#define BOSD_T_RIM_MGM_MONGRID_BIND_MGRGRID                         			"T_RIM_MGM_BIND_MON_INCLUDE_MGRGRID"
#define BOSD_T_RIM_MGM_MONGRID_BIND_MGRGRID_COLUMN_MGRGRIDID        			"MGRGRIDID"								/** ̨վID */
#define BOSD_T_RIM_MGM_MONGRID_BIND_MGRGRID_COLUMN_MONGRIDID        			"MONGRIDID"								/** ��������ID */

/** ���վ����ӳ��� */
#define BOSD_T_RIM_MGM_BIND_MONSTATION_INCLUDE_MGRGRIDID						"T_RIM_MGM_BIND_MONSTATION_INCLUDE_MGRGRIDID"
#define BOSD_T_RIM_MGM_BIND_MONSTATION_INCLUDE_MGRGRIDID_COLUMN_MONSTATIONID	"MONSTATIONID"                        
#define BOSD_T_RIM_MGM_BIND_MONSTATION_INCLUDE_MGRGRIDID_COLUMN_MGRGRIDID		"MGRGRIDID"           

/** �û���Ϣ�� */
#define BOSD_T_RIM30_USERINFO													"T_RIM30_USERINFO"
#define BOSD_T_RIM30_USERINFO_USERID											"USERID"                      
#define BOSD_T_RIM30_USERINFO_USERNAME											"USERNAME"   
#define BOSD_T_RIM30_USERINFO_EMPNO												"EMPNO"		 
#define BOSD_T_RIM30_USERINFO_AREACODE											"AREACODE"	 
#define BOSD_T_RIM30_USERINFO_DEPTID											"DEPTID"	 
#define BOSD_T_RIM30_USERINFO_DEPTNAME											"DEPTNAME"	 

/** �û����������� */
#define BOSD_T_RIM30_BRANCHINFO													"T_RIM30_BRANCHINFO"
#define BOSD_T_RIM30_BRANCHINFO_DEPTNAME										"DEPTNAME"	            
#define BOSD_T_RIM30_BRANCHINFO_PARENTID										"PARENTID"	
#define BOSD_T_RIM30_BRANCHINFO_TEL												"TEL"		
#define BOSD_T_RIM30_BRANCHINFO_FAX												"FAX"		
#define BOSD_T_RIM30_BRANCHINFO_EMAIL											"EMAIL"		
#define BOSD_T_RIM30_BRANCHINFO_MANAGER											"MANAGER"	
#define BOSD_T_RIM30_BRANCHINFO_NOTE											"NOTE"		
#define BOSD_T_RIM30_BRANCHINFO_DEPTLEVEL										"DEPTLEVEL"	
#define BOSD_T_RIM30_BRANCHINFO_DEPTID											"DEPTID"	
#define BOSD_T_RIM30_BRANCHINFO_DELFLAG											"DELFLAG"	
							

#define  BOSD_RIM_XML_PARAM_FRAME "<?xml version=\"1.0\"  encoding=\"gb2312\" >\n"\
	"<!-- �豸������� -->"\
	"<config>\n"\
	"<device name=\"\" >\n"\
	"</device>\n"\
	"</config>"
////////////////////////////////////////////////////////�ṹ����BEGIN////////////////////////////////////////////////////////

/**
* @class  tagCRTKMONSTATION 
*
* @brief 
****************************************************************************
*/
typedef class tagRTMSTATION
{
public:
	tagRTMSTATION::tagRTMSTATION()
	{
		sStationNO			= ""; 
		sName				= ""; 
		nCreateTime			= 0; 
		sAddress			= ""; 
		fLong 				= 0;
		fLati 				= 0;
		fMonRange			= 0;
		nStationType		= 0;
		sContact			= "";
		sDes				= "";
		sGPSAddr 			= "";
		nGPSPort 			= 0;
		sCompassAddr 		= "";
		nCompassPort 		= 0;
		nGridNO				= 0;
		sNettype			= "";
	}

	tagRTMSTATION::tagRTMSTATION(const tagRTMSTATION& ob)
	{
		*this = ob;
	}

	tagRTMSTATION& tagRTMSTATION::operator=(const tagRTMSTATION& ob)
	{
		sStationNO			= ob.sStationNO; 
		sName				= ob.sName; 
		nCreateTime			= ob.nCreateTime; 
		sAddress			= ob.sAddress; 
		fLong 				= ob.fLong; 
		fLati 				= ob.fLati; 
		fMonRange			= ob.fMonRange; 
		nStationType		= ob.nStationType; 
		sContact			= ob.sContact; 
		sDes				= ob.sDes; 
		sGPSAddr 			= ob.sGPSAddr; 
		nGPSPort 			= ob.nGPSPort; 
		sCompassAddr 		= ob.sCompassAddr; 
		nCompassPort 		= ob.nCompassPort; 
		nGridNO				= ob.nGridNO; 
		sNettype			= ob.sNettype; 

		return *this;
	}

	std::string  			sStationNO; 
	std::string  			sName; 
	LONG64       			nCreateTime; 
	std::string  			sAddress; 
	double       			fLong; 
	double       			fLati; 
	double       			fMonRange; 
	long         			nStationType; 
	std::string  			sContact; 
	std::string  			sDes; 
	std::string  			sGPSAddr; 
	short        			nGPSPort; 
	std::string  			sCompassAddr; 
	short        			nCompassPort; 
	long         			nGridNO; 
	std::string  			sNettype ;
}RTMSTATION, *LPRTMSTATION;
/**
* @class  RTKDEVICE
*
* @brief 
****************************************************************************
*/
typedef class tagRTMDEVICE
{
public:
	tagRTMDEVICE()
	{
		sStationNO			= "";	         
		sDeviceNO			= "";	         
		sDeviceModel		= "";	       
		sName 				= "";	             
		sDesc 				= "";	             
		nDeviceType			= 0;	       
		nBuyTime			= 0;	           
		sSupplier			= "";	         
		sRepairTel			= "";	         
		sDeviceCurAddress	= "";	 
		nStatus				= 0;	           
		sResponer			= "";	         
		nMaxRunNUM			= 0;	           
		sDriverIP			= "";	         
		nDriverPort			= 0;	       
		nDeviceProtocol		= 0;	
		bIsintegrated		= 0;
	}
	tagRTMDEVICE(const tagRTMDEVICE& ob)
	{
		*this = ob;
	}
	tagRTMDEVICE& operator=(const tagRTMDEVICE& ob)
	{
		sStationNO			= ob.sStationNO;	               
		sDeviceNO			= ob.sDeviceNO;	               
		sDeviceModel		= ob.sDeviceModel;	       	       
		sName 				= ob.sName;	                   
		sDesc 				= ob.sDesc;	                   
		nDeviceType 		= ob.nDeviceType;	             	             
		nBuyTime			= ob.nBuyTime;	                           
		sSupplier			= ob.sSupplier;	               
		sRepairTel			= ob.sRepairTel;	               
		sDeviceCurAddress	= ob.sDeviceCurAddress;	 
		nStatus				= ob.nStatus;	                               
		sResponer			= ob.sResponer;	               
		nMaxRunNUM			= ob.nMaxRunNUM;	                           
		sDriverIP			= ob.sDriverIP;	               
		nDriverPort			= ob.nDriverPort;	             	             
		nDeviceProtocol		= ob.nDeviceProtocol;
		bIsintegrated		= ob.bIsintegrated ;

		return *this;
	}

	std::string 			sStationNO;	         
	std::string 			sDeviceNO;	         
	std::string 			sDeviceModel;	       
	std::string 			sName;	             
	std::string 			sDesc;	             
	long					nDeviceType;	               
	LONG64					nBuyTime;	               
	std::string 			sSupplier;	         
	std::string 			sRepairTel;	         
	std::string 			sDeviceCurAddress;	 
	long					nStatus;	                   
	std::string				sResponer;	         
	long					nMaxRunNUM;	               
	std::string				sDriverIP;	         
	short					nDriverPort;	             
	long 					nDeviceProtocol;	
	bool 					bIsintegrated ;
}RTMDEVICE, *LPRTMDEVICE;

/**
* @class  tagSTATIONDEVICEFUNC 
*
* @brief 
****************************************************************************
*/
typedef class tagRTMDEVICEFUNC
{
public:
	tagRTMDEVICEFUNC()
	{
		sStationNO			=  "";	         
		sDeviceNO			=  "";	         
		nFunNO				=  0;	       
	}
	tagRTMDEVICEFUNC(const tagRTMDEVICEFUNC& ob)
	{
		*this = ob;
	}
	tagRTMDEVICEFUNC& operator=(const tagRTMDEVICEFUNC& ob)
	{
		sStationNO			= ob.sStationNO;	            
		sDeviceNO			= ob.sDeviceNO;	         	       
		nFunNO				= ob.nFunNO;		            

		return *this;
	}

	std::string 			sStationNO;	         
	std::string 			sDeviceNO;	         
	long					nFunNO;		
}RTMDEVICEFUNC, *LPRTMDEVICEFUNC;

/**
* @class  tagRTMREFDEVICEFUNC 
*
* @brief 
****************************************************************************
*/
typedef class tagRTMREFDEVICEFUNC
{
public:
	tagRTMREFDEVICEFUNC()
	{
		nFuncID				=  0;	         
		sFuncName			=  "";	         
		sFuncKey			=  "";	       
	}
	tagRTMREFDEVICEFUNC(const tagRTMREFDEVICEFUNC& ob)
	{
		*this = ob;
	}
	tagRTMREFDEVICEFUNC& operator=(const tagRTMREFDEVICEFUNC& ob)
	{
		nFuncID				= ob.nFuncID;	        
		sFuncName			= ob.sFuncName;	 	       
		sFuncKey			= ob.sFuncKey;	        

		return *this;
	}

	long        			nFuncID;	
	std::string 			sFuncName;	
	std::string 			sFuncKey;	
}RTMREFDEVICEFUNC, *LPRTMREFDEVICEFUNC;

/**
* @class  tagCOMREGIONNO 
*
* @brief 
****************************************************************************
*/
typedef class tagCOMREGIONNO
{
public:
	tagCOMREGIONNO()
	{
		sRegionNo			=  "";	         
		sRegionName			=  "";	       
	}
	tagCOMREGIONNO(const tagCOMREGIONNO& ob)
	{
		*this = ob;
	}
	tagCOMREGIONNO& operator=(const tagCOMREGIONNO& ob)
	{
		sRegionNo			= ob.sRegionNo;	 	       
		sRegionName			= ob.sRegionName;	        

		return *this;
	}

	std::string 			sRegionNo;	
	std::string 			sRegionName;	
}COMREGIONNO, *LPCOMREGIONNO;

/**
* @class  tagRTMREFDEVICEPROTOCOL 
*
* @brief 
****************************************************************************
*/
typedef class tagRTMREFDEVICEPROTOCOL
{
public:
	tagRTMREFDEVICEPROTOCOL()
	{
		nNoID				=  0;	         
		sName				=  "";	         
		sNote				=  "";	       
	}
	tagRTMREFDEVICEPROTOCOL(const tagRTMREFDEVICEPROTOCOL& ob)
	{
		*this = ob;
	}
	tagRTMREFDEVICEPROTOCOL& operator=(const tagRTMREFDEVICEPROTOCOL& ob)
	{
		nNoID				= ob.nNoID;	        
		sName				= ob.sName;	 	       
		sNote				= ob.sNote;	        

		return *this;
	}

	long        			nNoID;	
	std::string 			sName;	
	std::string 			sNote;	
}RTMREFDEVICEPROTOCOL, *LPRTMREFDEVICEPROTOCOL;

/**
* @class  tagRTMREFSTATIONTYPE 
*
* @brief 
****************************************************************************
*/
typedef class tagRTMREFSTATIONTYPE
{
public:
	tagRTMREFSTATIONTYPE()
	{
		nTypeNo				=  0;	         
		sTypeName			=  "";	       
	}
	tagRTMREFSTATIONTYPE(const tagRTMREFSTATIONTYPE& ob)
	{
		*this = ob;
	}
	tagRTMREFSTATIONTYPE& operator=(const tagRTMREFSTATIONTYPE& ob)
	{
		nTypeNo				= ob.nTypeNo;	 	       
		sTypeName			= ob.sTypeName;	        

		return *this;
	}

	long					nTypeNo;	
	std::string 			sTypeName;	
}RTMREFSTATIONTYPE, *LPRTMREFSTATIONTYPE;

/**
* @class  tagRTMREFDEVICETYPE 
*
* @brief 
****************************************************************************
*/
typedef class tagRTMREFDEVICETYPE
{
public:
	tagRTMREFDEVICETYPE()
	{
		nTypeNo				=  0;	         
		sTypeName			=  "";	       
	}
	tagRTMREFDEVICETYPE(const tagRTMREFDEVICETYPE& ob)
	{
		*this = ob;
	}
	tagRTMREFDEVICETYPE& operator=(const tagRTMREFDEVICETYPE& ob)
	{
		nTypeNo				= ob.nTypeNo;	 	       
		sTypeName			= ob.sTypeName;	        

		return *this;
	}

	long					nTypeNo;	
	std::string 			sTypeName;	
}RTMREFDEVICETYPE, *LPRTMREFDEVICETYPE;

/**
* @class  tagRTMMONORG 
*
* @brief 
****************************************************************************
*/
typedef class tagRTMMONORG
{
public:
	tagRTMMONORG()
	{
		sOrgNO	 			=  "";	       
		sName	 			=  "";	       
		sAddress	 		=  "";	       
		sPost				=  "";	       
		sResponpersion		=  "";	       
		sContact	 		=  "";	       
		sBrief 				=  "";	       
		sNote	 			=  "";	       
	}
	tagRTMMONORG(const tagRTMMONORG& ob)
	{
		*this = ob;
	}
	tagRTMMONORG& operator=(const tagRTMMONORG& ob)
	{
		sOrgNO	 			= ob.sOrgNO;	   
		sName	 			= ob.sName;	 
		sAddress	 		= ob.sAddress;	   
		sPost				= ob.sPost; 
		sResponpersion		= ob.sResponpersion;	  
		sContact	 		= ob.sContact;	 
		sBrief 				= ob.sBrief;	   
		sNote	 			= ob.sNote;	 

		return *this;
	}
	std::string 			sOrgNO;	 
	std::string 			sName;	 
	std::string 			sAddress;	 
	std::string 			sPost; 
	std::string 			sResponpersion;	 
	std::string 			sContact;	 
	std::string 			sBrief;	 
	std::string 			sNote;	 
}RTMMONORG, *LPRTMMONORG;

/**
* @class  tagRTMMONCTRLCENTER 
*
* @brief 
****************************************************************************
*/
typedef class tagRTMMONCTRLCENTER
{
public:
	tagRTMMONCTRLCENTER()
	{
		sCenterNO			=  "";	       
		sName	 			=  "";	       
		sAddress			=  "";	       
		fLong 				=  0;	       
		fLati	 			=  0;	       
		sNote	 			=  "";	       
	}
	tagRTMMONCTRLCENTER(const tagRTMMONCTRLCENTER& ob)
	{
		*this = ob;
	}
	tagRTMMONCTRLCENTER& operator=(const tagRTMMONCTRLCENTER& ob)
	{
		sCenterNO 			= ob.sCenterNO; 
		sName	 			= ob.sName;	 
		sAddress	 		= ob.sAddress;	 
		fLong 				= ob.fLong; 
		fLati	 			= ob.fLati;	  
		sNote	 			= ob.sNote;	 

		return *this;
	}
	std::string 			sCenterNO; 
	std::string 			sName;	 
	std::string 			sAddress;	 
	double 					fLong; 
	double 					fLati;	 
	std::string 			sNote;	 
}RTMMONCTRLCENTER, *LPRTMMONCTRLCENTER;

/**
* @class  tagRTMSTATIONDEVICEFUNCANT
*
* @brief  
****************************************************************************
*/
typedef class tagRTMSTATIONDEVICEFUNCANT
{
public:
	tagRTMSTATIONDEVICEFUNCANT()
	{
		sStationNO 			=  "";	       
		sDeviceNO  			=  "";	       
		nFunNO   			=  0;	       
		sAnt 				=  "";	       
		sDes 				=  "";	       
	}
	tagRTMSTATIONDEVICEFUNCANT(const tagRTMSTATIONDEVICEFUNCANT& ob)
	{
		*this = ob;
	}
	tagRTMSTATIONDEVICEFUNCANT& operator=(const tagRTMSTATIONDEVICEFUNCANT& ob)
	{
		sStationNO 			= ob.sStationNO;	      
		sDeviceNO  			= ob.sDeviceNO;	      
		nFunNO   			= ob.nFunNO;	  
		sAnt 				= ob.sAnt ;
		sDes 				= ob.sDes ;

		return *this;
	}
	std::string 			sStationNO;	         
	std::string 			sDeviceNO;	         
	long        			nFunNO;	  
	std::string 			sAnt ;
	std::string 			sDes ;
}RTMSTATIONDEVICEFUNCANT, *LPRTMSTATIONDEVICEFUNCANT;

/**
* @class  tagRTMANT
*
* @brief  
****************************************************************************
*/
typedef class tagRTMANT
{
public:
	tagRTMANT()
	{
		sStationNO			=  "";	       
		sAntName			=  "";	       
		sAntFactor			=  "";	       
		nBeginFreq			=  0;	       
		nEndFreq			=  0;	       
		nAntHeight			=  0;	       
	}
	tagRTMANT(const tagRTMANT& ob)
	{
		*this = ob;
	}
	tagRTMANT& operator=(const tagRTMANT& ob)
	{
		sStationNO			= ob.sStationNO;	    
		sAntName			= ob.sAntName;	
		sAntFactor			= ob.sAntFactor;	
		nBeginFreq			= ob.nBeginFreq; 
		nEndFreq			= ob.nEndFreq;	
		nAntHeight			= ob.nAntHeight;	

		return *this;
	}
	std::string 			sStationNO;	 
	std::string 			sAntName;	 
	std::string 			sAntFactor;	 
	LONG64      			nBeginFreq; 
	LONG64      			nEndFreq;	 
	long        			nAntHeight;	  
}RTMANT, *LPRTMANT;

/**
* @class  tagRTMDEVICEMODELFUNC
*
* @brief  
****************************************************************************
*/
typedef class tagRTMREFDEVICEMODEL
{
public:
	tagRTMREFDEVICEMODEL()
	{
		sNoId				=  "";	       
		sName				=  "";	       
		sNote				=  "";	        
	}
	tagRTMREFDEVICEMODEL(const tagRTMREFDEVICEMODEL& ob)
	{
		*this = ob;
	}
	tagRTMREFDEVICEMODEL& operator=(const tagRTMREFDEVICEMODEL& ob)
	{
		sNoId				= ob.sNoId;	    
		sName				= ob.sName;	
		sNote				= ob.sNote;	

		return *this;
	}

	std::string				sNoId;	
	std::string 			sName;	
	std::string 			sNote;	
}RTMREFDEVICEMODEL, *LPRTMREFDEVICEMODEL;

/**
* @class  tagRTMREFDEVICEMODELPARAM
*
* @brief  
****************************************************************************
*/
typedef class tagRTMREFDEVICEMODELFUNCPARAM
{
public:
	tagRTMREFDEVICEMODELFUNCPARAM()
	{
		sModel				=  "";	       
		nFuncID				=  0;	       
		sParam				=  "";	        
	}
	tagRTMREFDEVICEMODELFUNCPARAM(const tagRTMREFDEVICEMODELFUNCPARAM& ob)
	{
		*this = ob;
	}
	tagRTMREFDEVICEMODELFUNCPARAM& operator=(const tagRTMREFDEVICEMODELFUNCPARAM& ob)
	{
		sModel				= ob.sModel;	
		nFuncID				= ob.nFuncID;	    
		sParam				= ob.sParam;	

		return *this;
	}

	std::string 			sModel;	
	long					nFuncID;	
	std::string 			sParam;	
}RTMREFDEVICEMODELFUNCPARAM, *LPRTMREFDEVICEMODELFUNCPARAM;

/**
* @class  tagRIM30RTMRNC
*
* @brief  
****************************************************************************
*/
typedef class tagRIM30RTMRNC
{
public:
	tagRIM30RTMRNC()
	{
		sSTATNo             =  "";             
		sRncName            =  "";             
		sRncAddr            =  "";             
		nRncPort			=  0;               
		sDevName            =  "";             
		sCtrlPort           =  0;               
		bHasOS              =  0;               
		sOSIP				=  "";             
		nOSPort             =  0;               
		nType				=  0;               
	}
	tagRIM30RTMRNC(const tagRIM30RTMRNC& ob)
	{
		*this = ob;
	}
	tagRIM30RTMRNC& operator=(const tagRIM30RTMRNC& ob)
	{
		sSTATNo             = ob.sSTATNo;     
		sRncName            = ob.sRncName;      
		sRncAddr            = ob.sRncAddr;         
		nRncPort            = ob.nRncPort; 
		sDevName            = ob.sDevName;      
		sCtrlPort           = ob.sCtrlPort;          
		bHasOS              = ob.bHasOS;   
		sOSIP               = ob.sOSIP;       
		nOSPort             = ob.nOSPort;  
		nType				= ob.nType;  

		return *this;
	}
	std::string          	sSTATNo; 
	std::string          	sRncName;      
	std::string          	sRncAddr;         
	short                   nRncPort; 
	std::string             sDevName;      
	short                   sCtrlPort;          
	bool                    bHasOS;   
	std::string             sOSIP;       
	short                   nOSPort;    
	short                   nType;    
}RIM30RTMRNC, *LPRIM30RTMRNC;

/**
* @class  tagRIM30COMAUTHSYSDATASTATION
*
* @brief  
****************************************************************************
*/
typedef class tagRIM30COMAUTHSYSDATASTATION
{
public:
	tagRIM30COMAUTHSYSDATASTATION()
	{
		sSTATNo             =  "";             
		sNote				=  "";                        
	}
	tagRIM30COMAUTHSYSDATASTATION(const tagRIM30COMAUTHSYSDATASTATION& ob)
	{
		*this = ob;
	}
	tagRIM30COMAUTHSYSDATASTATION& operator=(const tagRIM30COMAUTHSYSDATASTATION& ob)
	{
		sSTATNo             = ob.sSTATNo;     
		sNote				= ob.sNote;      

		return *this;
	}
	std::string          	sSTATNo; 
	std::string          	sNote;         
}RIM30COMAUTHSYSDATASTATION, *LPRIM30COMAUTHSYSDATASTATION;

/**
* @class  tagRIM30COMAUTHSYSFUNC
*
* @brief  
****************************************************************************
*/
typedef class tagRIM30COMAUTHSYSFUNC
{
public:
	tagRIM30COMAUTHSYSFUNC()
	{
		sSysFuncNo          =  "";             
		sNote				=  "";                        
	}
	tagRIM30COMAUTHSYSFUNC(const tagRIM30COMAUTHSYSFUNC& ob)
	{
		*this = ob;
	}
	tagRIM30COMAUTHSYSFUNC& operator=(const tagRIM30COMAUTHSYSFUNC& ob)
	{
		sSysFuncNo           = ob.sSysFuncNo;     
		sNote				= ob.sNote;      

		return *this;
	}
	std::string          	sSysFuncNo; 
	std::string          	sNote;         
}RIM30COMAUTHSYSFUNC, *LPRIM30COMAUTHSYSFUNC;

/**
* @class  tagRIM30USERINFO
*
* @brief  
****************************************************************************
*/
typedef class tagRIM30USERINFO
{
public:
	tagRIM30USERINFO()
	{
		UserID				=  "";	       
		UserName			=  "";	       
		EmpNO				=  "";	       
		AreaCode			=  "";	       
		DeptID				=  "";	       
		DeptName			=  "";	       
	}
	tagRIM30USERINFO(const tagRIM30USERINFO& ob)
	{
		*this = ob;
	}
	tagRIM30USERINFO& operator=(const tagRIM30USERINFO& ob)
	{
		UserID				= ob.UserID;
		UserName			= ob.UserName;
		EmpNO				= ob.EmpNO;	
		AreaCode			= ob.AreaCode;
		DeptID				= ob.DeptID;
		DeptName			= ob.DeptName;

		return *this;
	}

	std::string 			UserID;
	std::string 			UserName;	
	std::string 			EmpNO;	
	std::string 			AreaCode;	
	std::string 			DeptID;
	std::string 			DeptName;	

}RIM30USERINFO, *LPRIM30USERINFO;


/**
* @class  tagRIM30BRANCHINFO
*
* @brief  
****************************************************************************
*/
typedef class tagRIM30BRANCHINFO
{
public:
	tagRIM30BRANCHINFO()
	{
		Deptname			=  "";	       
		Parentid			=  "";	       
		Tel					=  "";	       
		Fax					=  "";	       
		Email				=  "";	       
		Manager				=  "";	       
		Note				=  "";	       
		Deptlevel			=  "";	       
		Deptid				=  "";	       
		Delflag				=  "";	       
	}
	tagRIM30BRANCHINFO(const tagRIM30BRANCHINFO& ob)
	{
		*this = ob;
	}
	tagRIM30BRANCHINFO& operator=(const tagRIM30BRANCHINFO& ob)
	{
		Deptname			= ob.Deptname;
		Parentid			= ob.Parentid;
		Tel					= ob.Tel;	
		Fax					= ob.Fax;	
		Email				= ob.Email;	
		Manager				= ob.Manager;
		Note				= ob.Note;	
		Deptlevel			= ob.Deptlevel;
		Deptid				= ob.Deptid;	
		Delflag				= ob.Delflag;

		return *this;
	}

	std::string 			Deptname;
	std::string 			Parentid;
	std::string 			Tel;	
	std::string 			Fax;	
	std::string 			Email;	
	std::string 			Manager;
	std::string 			Note;	
	std::string 			Deptlevel;
	std::string 			Deptid;	
	std::string 			Delflag;

}RIM30BRANCHINFO, *LPRIM30BRANCHINFO;

/**
* @class  tagRIMCOMFREQLAYOUT
*
* @brief  
****************************************************************************
*/
typedef class tagRIMCOMFREQLAYOUT
{
public:
	tagRIMCOMFREQLAYOUT()
	{
		sName				=  "";	       
		nStartFreq			=  0;	       
		nEndFreq			=  0;	       
		sGroupName			=  "";	       
		nDistince			=  0;	       
		nEmitWidth			=  0;	              
	}
	tagRIMCOMFREQLAYOUT(const tagRIMCOMFREQLAYOUT& ob)
	{
		*this = ob;
	}
	tagRIMCOMFREQLAYOUT& operator=(const tagRIMCOMFREQLAYOUT& ob)
	{
		sName				= ob.sName;
		nStartFreq			= ob.nStartFreq;
		nEndFreq			= ob.nEndFreq;	
		sGroupName			= ob.sGroupName;	
		nDistince			= ob.nDistince;	
		nEmitWidth			= ob.nEmitWidth;	

		return *this;
	}

	std::string 			sName;
	int 					nStartFreq;
	int 					nEndFreq;	
	std::string 			sGroupName;	
	int 					nDistince;	
	int 					nEmitWidth;	

}RIMCOMFREQLAYOUT, *LPRIMCOMFREQLAYOUT;

////////////////////////////////////////////////////////�ṹ����END////////////////////////////////////////////////////////


#define BOSD_T_RIM_RTM_STATION_TYPE_INIT_DATA(table,id,name)  "DELETE FROM " ##table ";"\
	"INSERT INTO "##table "("##id","##name ") VALUES('1','�̶�վ');"\
	"INSERT INTO "##table "("##id","##name ") VALUES('2','�ɰ���վ');"\
	"INSERT INTO "##table "("##id","##name ") VALUES('3','�ƶ�վ');"\
	"INSERT INTO "##table "("##id","##name ") VALUES('4','��ʱվ');"\
	"INSERT INTO "##table "("##id","##name ") VALUES('5','С��վ');"

#define BOSD_T_RIM_RTM_FUNC_TYPE_INIT_DATA(table,id,name,key)  "DELETE FROM " ##table ";"\
	"INSERT INTO "##table "("##id","##name "," ##key ")  VALUES(10,'��Ƶ����','FIXFQ');"\
	"INSERT INTO "##table "("##id","##name "," ##key ")  VALUES(11,'��Ƶ����','IF_FQ');"\
	"INSERT INTO "##table "("##id","##name "," ##key ")  VALUES(12,'��Ƶ����','FIXDF');"\
	"INSERT INTO "##table "("##id","##name "," ##key ")  VALUES(13,'��Ƶ����','IF_DF');"\
	"INSERT INTO "##table "("##id","##name "," ##key ")  VALUES(14,'��ɢɨ��','MSCAN');"\
	"INSERT INTO "##table "("##id","##name "," ##key ")  VALUES(15,'Ƶ��ɨ��','FSCAN');"\
	"INSERT INTO "##table "("##id","##name "," ##key ")  VALUES(16,'����ɨ��','DSCAN');"\
	"INSERT INTO "##table "("##id","##name "," ##key ")  VALUES(17,'Ƶ��ɨ��','PSCAN');"\
	"INSERT INTO "##table "("##id","##name "," ##key ")  VALUES(18,'Ƶ�׷���','SPECTRUM');"\
	"INSERT INTO "##table "("##id","##name "," ##key ")  VALUES(19,'ʱ�����','TIMEDOM');"\
	"INSERT INTO "##table "("##id","##name "," ##key ")  VALUES(20,'��������','MULTIDF');"\
	"INSERT INTO "##table "("##id","##name "," ##key ")  VALUES(21,'Ƶ�ʲ���','SCANDF');"\
	"INSERT INTO "##table "("##id","##name "," ##key ")  VALUES(22,'��ɢ�ź�����','MSCANE');"\
	"INSERT INTO "##table "("##id","##name "," ##key ")  VALUES(23,'�ź�����','FSCANE');"\
	"INSERT INTO "##table "("##id","##name "," ##key ")  VALUES(24,'ITU����','ITU');"\
	"INSERT INTO "##table "("##id","##name "," ##key ")  VALUES(25,'���������','WB_DF');"\
	"INSERT INTO "##table "("##id","##name "," ##key ")  VALUES(26,'��Ƶ�������','IFDFEXT');"\
	"INSERT INTO "##table "("##id","##name "," ##key ")  VALUES(27,'��Ƶ���','LPI');"\
	"INSERT INTO "##table "("##id","##name "," ##key ")  VALUES(28,'������','WB_FQ');"\
	"INSERT INTO "##table "("##id","##name "," ##key ")  VALUES(29,'���ֽ��','DIGDEM');"\
	"INSERT INTO "##table "("##id","##name "," ##key ")  VALUES(30,'���ɨ��','WMON');"\
	"INSERT INTO "##table "("##id","##name "," ##key ")  VALUES(31,'����̽��','EDETN');"\
	"INSERT INTO "##table "("##id","##name "," ##key ")  VALUES(32,'��ɢ����','MDF');"\
	"INSERT INTO "##table "("##id","##name "," ##key ")  VALUES(33,'��Ϣ����','SIGNALMEAS');"\
	"INSERT INTO "##table "("##id","##name "," ##key ")  VALUES(34,'�ź�ʶ��','MODREC');"\
	"INSERT INTO "##table "("##id","##name "," ##key ")  VALUES(35,'�źŸ澯','SINA');"

#define BOSD_T_RIM_REGION_NO_INIT_DATA(table,no,name) "DELETE FROM " ##table ";"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('000000','��������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('110000','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('120000','�����' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('130000','�ӱ�ʡ' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('130100','ʯ��ׯ��' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('130200','��ɽ��' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('130300','�ػʵ���' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('130400','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('130500','��̨��' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('130600','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('130700','�żҿ���' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('130800','�е���' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('130900','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('131000','�ȷ���' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('131100','��ˮ��' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('140000','ɽ��ʡ' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('140100','̫ԭ��' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('140200','��ͬ��' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('140300','��Ȫ��' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('140400','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('140500','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('140600','˷����' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('140700','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('140800','�˳���' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('140900','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('141000','�ٷ���' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('141100','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('150000','���ɹ�������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('150100','���ͺ�����' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('150200','��ͷ��' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('150300','�ں���' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('150400','�����' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('150500','ͨ����' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('150600','������˹��' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('150700','���ױ�����' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('150800','�����׶���' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('150900','�����첼��' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('152200','�˰���' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('152500','���ֹ�����' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('152900','��������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('210000','����ʡ' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('210100','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('210200','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('210300','��ɽ��' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('210400','��˳��' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('210500','��Ϫ��' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('210600','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('210700','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('210800','Ӫ����' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('210900','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('211000','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('211100','�̽���' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('211200','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('211300','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('211400','��«����' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('220000','����ʡ' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('220100','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('220200','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('220300','��ƽ��' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('220400','��Դ��' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('220500','ͨ����' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('220600','��ɽ��' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('220700','��ԭ��' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('220800','�׳���' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('222400','�ӱ߳�����������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('230000','������ʡ' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('230100','��������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('230200','���������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('230300','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('230400','�׸���' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('230500','˫Ѽɽ��' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('230600','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('230700','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('230800','��ľ˹��' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('230900','��̨����' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('231000','ĵ������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('231100','�ں���' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('231200','�绯��' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('232700','���˰������(�Ӹ����)' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('310000','�Ϻ���' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('320000','����ʡ' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('320100','�Ͼ���' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('320200','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('320300','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('320400','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('320500','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('320600','��ͨ��' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('320700','���Ƹ���' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('320800','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('320900','�γ���' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('321000','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('321100','����' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('321200','̩����' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('321300','��Ǩ��' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('330000','�㽭ʡ' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('330100','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('330200','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('330300','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('330400','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('330500','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('330600','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('330700','����' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('330800','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('330900','��ɽ��' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('331000','̨����' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('331100','��ˮ��' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('340000','����ʡ' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('340100','�Ϸ���' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('340200','�ߺ���' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('340300','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('340400','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('340500','��ɽ��' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('340600','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('340700','ͭ����' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('340800','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('341000','��ɽ��' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('341100','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('341200','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('341300','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('341400','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('341500','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('341600','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('341700','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('341800','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('350000','����ʡ' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('350100','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('350200','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('350300','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('350400','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('350500','Ȫ����' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('350600','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('350700','��ƽ��' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('350800','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('350900','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('360000','����ʡ' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('360100','�ϲ���' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('360200','��������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('360300','Ƽ����' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('360400','�Ž���' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('360500','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('360600','ӥ̶��' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('360700','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('360800','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('360900','�˴���' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('361000','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('361100','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('370000','ɽ��ʡ' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('370100','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('370200','�ൺ��' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('370300','�Ͳ���' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('370400','��ׯ��' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('370500','��Ӫ��' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('370600','��̨��' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('370700','Ϋ����' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('370800','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('370900','̩����' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('371000','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('371100','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('371200','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('371300','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('371400','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('371500','�ĳ���' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('371600','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('371700','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('410000','����ʡ' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('410100','֣����' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('410200','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('410300','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('410400','ƽ��ɽ��' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('410500','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('410600','�ױ���' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('410700','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('410800','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('410900','�����' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('411000','�����' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('411100','�����' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('411200','����Ͽ��' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('411300','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('411400','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('411500','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('411600','�ܿ���' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('411700','פ�����' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('420000','����ʡ' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('420100','�人��' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('420200','��ʯ��' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('420300','ʮ����' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('420500','�˲���' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('420600','�差��' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('420700','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('420800','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('420900','Т����' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('421000','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('421100','�Ƹ���' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('421200','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('421300','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('422800','��ʩ������������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('430000','����ʡ' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('430100','��ɳ��' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('430200','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('430300','��̶��' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('430400','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('430500','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('430600','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('430700','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('430800','�żҽ���' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('430900','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('431000','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('431100','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('431200','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('431300','¦����' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('433100','��������������������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('440000','�㶫ʡ' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('440100','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('440200','�ع���' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('440300','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('440400','�麣��' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('440500','��ͷ��' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('440600','��ɽ��' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('440700','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('440800','տ����' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('440900','ï����' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('441200','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('441300','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('441400','÷����' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('441500','��β��' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('441600','��Դ��' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('441700','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('441800','��Զ��' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('441900','��ݸ��' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('442000','��ɽ��' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('445100','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('445200','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('445300','�Ƹ���' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('450000','����׳��������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('450100','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('450200','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('450300','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('450400','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('450500','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('450600','���Ǹ���' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('450700','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('450800','�����' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('450900','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('451000','��ɫ��' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('451100','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('451200','�ӳ���' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('451300','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('451400','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('460000','����ʡ' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('460100','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('460200','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('500000','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('502100','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('502200','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('502300','ǭ����' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('510000','�Ĵ�ʡ' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('510100','�ɶ���' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('510300','�Թ���' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('510400','��֦����' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('510500','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('510600','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('510700','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('510800','��Ԫ��' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('510900','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('511000','�ڽ���' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('511100','��ɽ��' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('511300','�ϳ���' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('511400','üɽ��' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('511500','�˱���' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('511600','�㰲��' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('511700','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('511800','�Ű���' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('511900','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('512000','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('513200','���Ӳ���Ǽ��������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('513300','���β���������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('513400','��ɽ����������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('520000','����ʡ' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('520100','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('520200','����ˮ��' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('520300','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('520400','��˳��' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('522200','ͭ�ʵ���' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('522300','ǭ���ϲ���������������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('522400','�Ͻڵ���' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('522600','ǭ�������嶱��������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('522700','ǭ�ϲ���������������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('530000','����ʡ' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('530100','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('530300','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('530400','��Ϫ��' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('530500','��ɽ��' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('530600','��ͨ��' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('530700','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('530800','�ն���' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('530900','�ٲ���' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('532300','��������������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('532500','��ӹ���������������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('532600','��ɽ׳������������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('532800','��˫���ɴ���������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('532900','�������������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('533100','�º���徰����������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('533300','ŭ��������������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('533400','�������������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('540000','����������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('540100','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('542100','��������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('542200','ɽ�ϵ���' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('542300','�տ������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('542400','��������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('542500','�������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('542600','��֥����' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('610000','����ʡ' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('610100','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('610200','ͭ����' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('610300','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('610400','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('610500','μ����' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('610600','�Ӱ���' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('610700','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('610800','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('610900','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('611000','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('620000','����ʡ' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('620100','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('620200','��������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('620300','�����' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('620400','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('620500','��ˮ��' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('620600','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('620700','��Ҵ��' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('620800','ƽ����' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('620900','��Ȫ��' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('621000','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('621100','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('621200','¤����' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('622900','���Ļ���������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('623000','���ϲ���������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('630000','�ຣʡ' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('630100','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('632100','��������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('632200','��������������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('632300','���ϲ���������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('632500','���ϲ���������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('632600','�������������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('632700','��������������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('632800','�����ɹ������������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('640000','���Ļ���������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('640100','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('640200','ʯ��ɽ��' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('640300','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('640400','��ԭ��' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('640500','������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('650000','�½�ά���������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('650100','��³ľ����' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('650200','����������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('650800','ʯ������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('652100','��³������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('652200','���ܵ���' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('652300','��������������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('652700','���������ɹ�������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('652800','���������ɹ�������' );"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('652900','�����յ���');"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('653000','�������տ¶�����������');"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('653100','��ʲ����');"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('653200','�������');"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('654000','���������������');"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('654200','���ǵ���');"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('654300','����̩����');"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('654400','������');"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('710000','̨��ʡ');"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('810000','����ر�������');"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('820000','�����ر�������');"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('632900','���ľ');"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('633000','����');"\
	"INSERT INTO "##table "("##no","##name ") VALUES ('411800','��Դ��');"
