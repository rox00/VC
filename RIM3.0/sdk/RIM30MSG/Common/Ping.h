//********************************************************************
// PING.H �ļ�ע��
// �ļ��� ��: PING.H
// �ļ�·�� : E:\PROJECT\RIM3.0\SRC\SDK\RIM30MSG/
// ���� ����: ����
// ����ʱ�� : 2015/4/23 11:55
// �ļ����� : 
//*********************************************************************

#if !defined(__PING_H__)
#define __PING_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <process.h>
#include <assert.h>
#include <WINSOCK2.h>
#pragma comment( lib, "Ws2_32" )

#include <string>
using namespace std;

#define SEND_SIZE 32 
#define PACKET_SIZE 4096
#define ICMP_ECHO 8
#define ICMP_ECHOREPLY 0

struct icmp
{
	unsigned char 			icmp_type;
	unsigned char 			icmp_code;
	unsigned short 			icmp_cksum;
	unsigned short 			icmp_id;
	unsigned short 			icmp_seq;
	unsigned long			icmp_data; 
};

struct ip
{
	unsigned char 			ip_hl:4;       
	unsigned char 			ip_v:4;        
	unsigned char 			ip_tos;           
	unsigned short 			ip_len;         
	unsigned short 			ip_id;          
	unsigned short 			ip_off;         
	unsigned char 			ip_ttl;           
	unsigned char 			ip_p;          
	unsigned short			ip_sum;         
	unsigned long 			ip_src;
	unsigned long 			ip_dst;  
};

class CPing  
{
public:
	CPing::CPing():sockfd_(INVALID_SOCKET)
	{
	}

	bool					ping(const string & str_IP, const unsigned short n_try, string & str_error);
private:	
	SOCKET			   		sockfd_;
	unsigned char	   		sendpacket_[PACKET_SIZE];
	unsigned char	   		recvpacket_[PACKET_SIZE];
	struct sockaddr_in 		dest_addr_;
	struct sockaddr_in 		from_addr_;
	unsigned short			cal_chksum(unsigned short *addr,int len);

};


inline bool CPing::ping(const string & str_IP, const unsigned short n_try, string & str_error)
{
	assert(!str_IP.empty());
	assert(n_try != 0);

	int	n_pid	= 0;
	int timeout = 0;
	struct protoent *protocol = NULL;
	WSADATA			wsaData;

	struct hostent *host= NULL;
	char FAR *pstr_addr	= NULL;
	int fromlen			= sizeof(from_addr_);
	int nlen			= 0;
	struct ip *ip		= NULL;
	struct icmp *icmp	= NULL;
	bool b_return		= false;
	unsigned short i	= 0;

	if(WSAStartup(0x1010,&wsaData)!=0){
		str_error = "����WSAStartup()��ʼ�����绷��ʧ��";
		return false;
	}
	if( (protocol=getprotobyname ("icmp") )==NULL){ 
		str_error = "����getprotobyname()��ȡ\"icmp\"Э����Ϣʧ��";
		goto LabelEnd;
	}
	if((sockfd_=socket(AF_INET,SOCK_RAW,protocol->p_proto)) == INVALID_SOCKET){   
		int a = WSAGetLastError();
		str_error = "����ԭ���׽���ʧ��";
		AfxMessageBox(str_error.c_str());
		goto LabelEnd;
	}

	timeout = 2000;
	if(setsockopt(sockfd_,SOL_SOCKET,SO_RCVTIMEO,(char*)&timeout,sizeof(timeout))<0){
		int a = WSAGetLastError();
		str_error = "�����׽��ֽ��ճ�ʱʧ��";
		goto LabelEnd;
	}
	if(setsockopt(sockfd_,SOL_SOCKET,SO_SNDTIMEO,(char*)&timeout,sizeof(timeout))<0){
		str_error = "�����׽��ַ��ͳ�ʱʧ��";
		goto LabelEnd;
	}
	n_pid = _getpid();//!< ������ʵ�������߳�ID

	memset(&dest_addr_, 0, sizeof(dest_addr_));
	dest_addr_.sin_family=AF_INET; 
	if(host = gethostbyname(str_IP.c_str())){
		memcpy( (char *)&dest_addr_.sin_addr,host->h_addr,host->h_length);
	}

	for(i = 0; i < n_try; ++i){
		//���ݴ��
		nlen=8 + SEND_SIZE;
		icmp = (struct icmp*)sendpacket_;
		icmp->icmp_type		= ICMP_ECHO;
		icmp->icmp_code		= 0;
		icmp->icmp_cksum	= 0;
		icmp->icmp_seq		= i;
		icmp->icmp_id		= n_pid;
		icmp->icmp_data		= GetTickCount();
		icmp->icmp_cksum	= cal_chksum( (unsigned short *)icmp, nlen); /*У���㷨*/
		if(sendto(sockfd_, (const char*)sendpacket_, nlen, 0,
			(struct sockaddr *)&dest_addr_, sizeof(dest_addr_)) < 0){
				//�������ݰ�ʧ��, ������̽
				str_error = "�������ݰ�ʧ��";
				continue;
		}
		if((nlen = recvfrom(sockfd_,(char*)recvpacket_, sizeof(recvpacket_), 0, (struct sockaddr *)&from_addr_,&fromlen)) < 0){
			//�������ݰ�ʧ��
			str_error = "�������ݰ�ʧ��";
			continue;
		}

		//�Ƚ�IP
		//pstr_addr = inet_ntoa(from_addr_.sin_addr);
		//if(string(pstr_addr) != str_IP){
		//���յ����ݰ�IP�뷢�͵�IP����ͬ, ������̽
		//	str_error = "���յ����ݰ�IP�뷢�͵�IP����ͬ";
		//	continue;
		//}

		////IP��ͬ, ������
		//ip = (struct ip *)recvpacket_;
		//icmp = (struct icmp *)(recvpacket_ + (ip->ip_hl * 4));         
		//      //if(icmp->icmp_type != ICMP_ECHO){
		////	str_error = "���յ�����Ϊ��Ԥ������, icmp->icmp_type != ICMP_ECHO";
		////	continue;
		////}
		//      if(icmp->icmp_type != ICMP_ECHOREPLY){
		//	str_error = "���յ�����Ϊ��Ԥ������, icmp->icmp_type != ICMP_ECHOREPLY";

		//          string strOut;
		//          strOut.resize(200, 0);
		//          strOut = strOut.substr(0, sprintf((char *)strOut.c_str(), 
		//              "�յ���icmp->icmp_type == %d, Ԥ��Ϊ: %d, ���߲����",
		//              icmp->icmp_type, ICMP_ECHOREPLY));
		//          OutputDebugString("\n�ͻ���ͨѶ���, CPing:: ");
		//          OutputDebugString(strOut.c_str());
		//          OutputDebugString("\n");

		//	//continue;
		//}
		//      if(icmp->icmp_id != n_pid){
		//	str_error = "���յ�����Ϊ��Ԥ������, icmp->icmp_id != n_pid";

		//          string strOut;
		//          strOut.resize(200, 0);
		//          strOut = strOut.substr(0, sprintf((char *)strOut.c_str(), 
		//              "�յ���icmp->icmp_id == %d, Ԥ��n_pid ==  %d, ���߲����",
		//              icmp->icmp_id, n_pid));
		//          OutputDebugString("\n�ͻ���ͨѶ���, CPing:: ");
		//          OutputDebugString(strOut.c_str());
		//          OutputDebugString("\n");

		//	//continue;
		//}

		b_return = true;
		break;
	}

LabelEnd:
	if(sockfd_ != INVALID_SOCKET){
		closesocket(sockfd_);
		sockfd_ = INVALID_SOCKET;
	}
	//WSACleanup();//���������̷߳�Χ�������������ĵ���, ������Bug, Ӧע��

	if(b_return){
		str_error = "";
	}
	else{
		assert(!str_error.empty());
		if(i ==  n_try){
			str_error = "�����ͼ��" + str_IP + "ͨѶ��ʧ��, ���һ��ʧ��ԭ��: " + str_error;
		}
	}
	return b_return;
}

//this algorithm is referenced from other's 
inline unsigned short CPing::cal_chksum(unsigned short *addr,int len)
{       
	int nleft				= len;
	int sum					= 0;
	unsigned short *w		= addr;
	unsigned short answer	= 0;

	while(nleft>1){ 
		sum+=*w++;
		nleft-=2;
	}
	if( nleft==1){   
		*(unsigned char *)(&answer)=*(unsigned char *)w;
		sum+=answer;
	}

	sum=(sum>>16)+(sum&0xffff);
	sum+=(sum>>16);
	answer=~sum;
	return answer;
}

#endif // !defined(__PING_H__)
