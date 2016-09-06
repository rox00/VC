#pragma once


// CImageConfig

class ImageConfig
{
public:
	virtual ~ImageConfig();

protected:
	ImageConfig();
	static ImageConfig*						m_pImageConfig;				//�ڲ�ʵ��ָ��

public:
	static ImageConfig* 					GetInstance();
	static void 							ReleaseInstance();

	////��zy 2015-05-29 ����	�����湲��
	Image*									m_pImgSizeMid;				//��������ʱ�����м䲿�ֵ�PNGͼƬ
	Image*									m_pImgSizebottom;			//��������ʱ���Ƶײ���PNGͼƬ

	////��zy 2015-05-29 ���� CRIM30Dlg
	Image*									m_pImgChkSel;				//���ڻ����²���ťѡ��״̬
	Image*									m_pImgChkUnSel;				//���ڻ����²���ťδѡ��״̬

	////��zy 2015-05-29 ���� CLogonWndDlg
	Image*									m_pImgLogon;				//��¼����ͼƬ��������������ε��ϲ�

	////��zy 2015-05-29 ���� CCircleDlg
	Image*									m_pImgLogo;					//���ڻ���Բ�����������LogoͼƬ
	Image*									m_pImgCircle;				//���ڻ���Բ�������ͼƬ

	////��zy 2015-05-29 ���� CMainListDlg
	Image*									m_pImgMainList;				//���ڻ��Ƶ�¼������������ϲ���PNGͼƬ

	////��zy 2015-05-29 ���� CDialogSet
	Image*									m_pImageDlgSetBk;		


	////��zy 2015-05-29 ���� CChatBkDlg
	Image*									m_ImageChatBk;				////�û�ѡ��ͼ��
	Image*									m_ImageUserSel;				////�û�ѡ��ͼ��
	Image*									m_ImageMinNormal;			////��С������ͼ��
	Image*									m_ImageMinPush;				////��С����꾭������ʱ��ͼ��
	Image*									m_ImageRestoreNormal;		////��ԭ��ť����ͼ��
	Image*									m_ImageRestorePush;			////��ԭ��ť��꾭������ʱ��ͼ��
	Image*									m_ImageMaxNormal;			////�������ͼ��
	Image*									m_ImageMaxPush;				////�����꾭������ʱ��ͼ��
	Image*									m_ImageCloseNormal;			////�رհ�ť����ͼ��
	Image*									m_ImageClosePush;			////�رհ�ť��꾭������ʱ��ͼ��
	Image*									m_ImageSendNormal;			////���Ͱ�ť����ͼ��
	Image*									m_ImageSendPush;			////���Ͱ�ť��꾭������ʱ��ͼ��
	Image*									m_ImageClose1Normal;		////�رհ�ť����ͼ��
	Image*									m_ImageClose1Push;			////�رհ�ť��꾭������ʱ��ͼ��

	Image*									m_RichImageBk;				////RichEdit����ͼƬ	Ŀǰû��ʹ�õ�
	Image*									m_FileNormal;				////�ļ�ͼ�������ͼ��
	Image*									m_FilePush;					////�ļ�ͼ�갴��ʱ��ͼ��
	Image*									m_DicNormal;				////�ļ���ͼ�������ͼ��
	Image*									m_DicPush;					////�ļ���ͼ�갴��ʱ��ͼ��
	Image*									m_PictureNormal;			////����ͼƬ��ť����ͼ��
	Image*									m_PicturePush;				////����ͼƬ��ť����ʱ��ͼ��

	////��zy 2015-05-29 ���� CChatRichView
	Image*									m_pImgScrollBk;	
	Image*									m_pImgHScrollThumb;
	Image*									m_pImgVScrollThumb;


};


