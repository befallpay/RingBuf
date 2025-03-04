#ifndef __RING_BUF__H
#define __RING_BUF__H


//------------------------------EXTERN_FUNCTION_START--------------------------------//

enum
{
	// 0: 动态申请。缓存由模块负责申请和释放。
	RB_FLAG_ALLOC = 0,
	// 1: 用户传入。缓存由用户提供
	RB_FLAG_USER,
	// 2: 全结构。数据结构和数据缓存在同个连续空间（适用于共享内存）
	RB_FLAG_FLAT
};

typedef struct {
	// 组织类型
	unsigned short	vFlag;
	// 元素大小
	unsigned short	vSize;
	// 元素总数
	unsigned int	vCount;
	// 读指针
	unsigned int	vRead;
	// 写指针
	unsigned int	vWrite;
	// 缓存空间
	union{
		unsigned char	*szBuf;
		// 对应"RB_FLAG_FLAT"风格
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
