#ifndef __RING_BUF__H
#define __RING_BUF__H


//------------------------------EXTERN_FUNCTION_START--------------------------------//

enum
{
	// 0: ��̬���롣������ģ�鸺��������ͷš�
	RB_FLAG_ALLOC = 0,
	// 1: �û����롣�������û��ṩ
	RB_FLAG_USER,
	// 2: ȫ�ṹ�����ݽṹ�����ݻ�����ͬ�������ռ䣨�����ڹ����ڴ棩
	RB_FLAG_FLAT
};

typedef struct {
	// ��֯����
	unsigned short	vFlag;
	// Ԫ�ش�С
	unsigned short	vSize;
	// Ԫ������
	unsigned int	vCount;
	// ��ָ��
	unsigned int	vRead;
	// дָ��
	unsigned int	vWrite;
	// ����ռ�
	union{
		unsigned char	*szBuf;
		// ��Ӧ"RB_FLAG_FLAT"���
		unsigned char	szData[4];
	};	
}CRingBuf;

int RingBuf_New(CRingBuf *pRb, void *szBuf, int vCount, int vSize);
int RingBuf_Del(CRingBuf *pRb);

int RingBuf_IsFull(CRingBuf *pRb);
int RingBuf_IsEmpty(CRingBuf *pRb);

int RingBuf_ValidCount(CRingBuf *pRb);
int RingBuf_BufSize(CRingBuf *pRb);

int RingBuf_Put(CRingBuf *pRb, const void *pItem);
int RingBuf_Get(CRingBuf *pRb, void *pChar);
int RingBuf_Write(CRingBuf *pRb, const void *szData, int vCount);
int RingBuf_Read(CRingBuf *pRb, void *szData, int vCount);

//------------------------------EXTERN_FUNCTION_END--------------------------------//

#endif
