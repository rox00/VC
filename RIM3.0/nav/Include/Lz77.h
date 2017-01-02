//////////////////////////////
// LZ77.h
//////////////////////////////
#pragma once
// ʹ�����Լ��Ķ��з��������ڵ㣬����������
// ÿ�����ѹ�� 65536 �ֽ�����
// ���Ż��汾

#ifndef _WIX_LZ77_COMPRESS_HEADER_001_
#define _WIX_LZ77_COMPRESS_HEADER_001_

// �������ڵ��ֽڴ�С
#define _MAX_WINDOW_SIZE	65536

class CCompress
{
public:
	CCompress() {};
	virtual ~CCompress() {};

public:
	virtual int Compress(BYTE* src, int srclen, BYTE* dest) = 0;
	virtual BOOL Decompress(BYTE* src, int srclen, BYTE* dest) = 0;

protected:
	// tools 

	/////////////////////////////////////////////////////////
	// CopyBitsInAByte : ��һ���ֽڷ�Χ�ڸ���λ��
	// ��������ͬ CopyBits �Ĳ���
	// ˵����
	//		�˺����� CopyBits ���ã����������飬��
	//		�ٶ�Ҫ���Ƶ�λ����һ���ֽڷ�Χ��
	void CopyBitsInAByte(BYTE* memDest, int nDestPos, 
				  BYTE* memSrc, int nSrcPos, int nBits);

	////////////////////////////////////////////////////////
	// CopyBits : �����ڴ��е�λ��
	//		memDest - Ŀ��������
	//		nDestPos - Ŀ����������һ���ֽ��е���ʼλ
	//		memSrc - Դ������
	//		nSrcPos - Դ��������һ���ֽڵ�����ʼλ
	//		nBits - Ҫ���Ƶ�λ��
	//	˵����
	//		��ʼλ�ı�ʾԼ��Ϊ���ֽڵĸ�λ����λ���������ң�
	//		����Ϊ 0��1��... , 7
	//		Ҫ���Ƶ������������������غ�
	void CopyBits(BYTE* memDest, int nDestPos, 
				  BYTE* memSrc, int nSrcPos, int nBits);

	//////////////////////////////////////////////////////////////
	// ��DWORDֵ�Ӹ�λ�ֽڵ���λ�ֽ�����
	void InvertDWord(DWORD* pDW);

	/////////////////////////////////////////////////////////////
	// ����byte�ĵ�iBitλΪaBit
	//		iBit˳��Ϊ��λ���0����������
	void SetBit(BYTE* byte, int iBit, BYTE aBit);

	////////////////////////////////////////////////////////////
	// �õ��ֽ�byte��posλ��ֵ
	//		pos˳��Ϊ��λ���0����������
	BYTE GetBit(BYTE byte, int pos);

	////////////////////////////////////////////////////////////
	// ��λָ��*piByte(�ֽ�ƫ��), *piBit(�ֽ���λƫ��)����numλ
	void MovePos(int* piByte, int* piBit, int num);

	/////////////////////////////////////////////////////////
	// ȡlog2(n)��upper_bound
	int UpperLog2(int n);

	/////////////////////////////////////////////////////////
	// ȡlog2(n)��lower_bound
	int LowerLog2(int n);
};

class CCompressLZ77 : public CCompress
{
public:
	CCompressLZ77();
	virtual ~CCompressLZ77();
public:
	/////////////////////////////////////////////
	// ѹ��һ���ֽ���
	// src - Դ������
	// srclen - Դ�������ֽڳ���, srclen <= 65536
	// dest - ѹ��������������ǰ����srclen�ֽ��ڴ�
	// ����ֵ > 0 ѹ�����ݳ���
	// ����ֵ = 0 �����޷�ѹ��
	// ����ֵ < 0 ѹ�����쳣����
	int Compress(BYTE* src, int srclen, BYTE* dest);

	/////////////////////////////////////////////
	// ��ѹ��һ���ֽ���
	// src - ����ԭʼ���ݵ��ڴ���, srclen <= 65536
	// srclen - Դ�������ֽڳ���
	// dest - ѹ��������
	// ����ֵ - �ɹ����
	BOOL Decompress(BYTE* src, int srclen, BYTE* dest);

protected:

	BYTE* pWnd;
	// ���ڴ�С���Ϊ 64k �����Ҳ�������
	// ÿ�����ֻѹ�� 64k ���ݣ��������Է�����ļ��м俪ʼ��ѹ
	// ��ǰ���ڵĳ���
	int nWndSize;

	// �Ի���������ÿһ��2�ֽڴ�����
	// ������Ϊ�˽��п�������ƥ��
	// ����ķ�������һ��64k��С��ָ������
	// �����±����ζ�Ӧÿһ��2�ֽڴ���(00 00) (00 01) ... (01 00) (01 01) ...
	// ÿһ��ָ��ָ��һ�����������еĽڵ�Ϊ��2�ֽڴ���ÿһ������λ��
	struct STIDXNODE
	{
		WORD off;		// ��src�е�ƫ��
		WORD off2;		// ���ڶ�Ӧ��2�ֽڴ�Ϊ�ظ��ֽڵĽڵ�
						// ָ�� off �� off2 ����Ӧ�˸�2�ֽڴ�
		WORD next;		// ��SortHeap�е�ָ��
	};
	
	WORD SortTable[65536];  // 256 * 256 ָ��SortHeap���±��ָ��

	// ��Ϊ���ڲ�������û��ɾ���ڵ�Ĳ���������
	// �ڵ������SortHeap ����������
	struct STIDXNODE* SortHeap;
	int HeapPos;	// ��ǰ����λ��

	// ��ǰ���λ��(�ֽ�ƫ�Ƽ�λƫ��)
	int CurByte, CurBit;

protected:
	////////////////////////////////////////
	// ���ѹ����
	// code - Ҫ�������
	// bits - Ҫ�����λ��(��isGamma=TRUEʱ��Ч)
	// isGamma - �Ƿ����Ϊ�ñ���
	void _OutCode(BYTE* dest, DWORD code, int bits, BOOL isGamma);

	///////////////////////////////////////////////////////////
	// �ڻ��������в�������
	// nSeekStart - �Ӻδ���ʼƥ��
	// offset, len - ���ڽ��ս������ʾ�ڻ��������ڵ�ƫ�ƺͳ���
	// ����ֵ- �Ƿ�鵽����Ϊ3��3���ϵ�ƥ���ֽڴ�
	BOOL _SeekPhase(BYTE* src, int srclen, int nSeekStart, int* offset, int* len);

	///////////////////////////////////////////////////////////
	// �õ��Ѿ�ƥ����3���ֽڵĴ���λ��offset
	// ����ƥ����ٸ��ֽ�
	inline int _GetSameLen(BYTE* src, int srclen, int nSeekStart, int offset);

	//////////////////////////////////////////
	// ���������һ���n���ֽ�
	inline void _ScrollWindow(int n);

	// �����������һ��2�ֽڴ�
	inline void _InsertIndexItem(int off);

	// ��ʼ���������ͷ��ϴ�ѹ���õĿռ�
	void _InitSortTable();
};



#endif // _WIX_LZW_COMPRESS_HEADER_001_