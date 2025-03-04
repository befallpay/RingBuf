#include "dutil.h"
#include "RingBuf.h"


#define rb_printf(...)		    //dprintf(__VA_ARGS__)
#define rb_printh(szBuf, vLen)  //dprinth(1, __FUNCTION__, szBuf, vLen, 1)


int RingBuf_New(CRingBuf *pRb, void *szBuf, int vMemSize, int vItemSize)
{
	RET_START
	int vBufSize;

	rjudge(pRb==NULL, -1);

	memset(pRb, 0, sizeof(CRingBuf));
	pRb->vSize = vItemSize;

	if(szBuf == pRb){
		// 结构和缓存连续分布
		pRb->vFlag = RB_FLAG_FLAT;

		vBufSize = vMemSize-sizeof(CRingBuf)-4;
		pRb->vCount = vBufSize/vItemSize;
	}
	else{
		pRb->vCount = vMemSize/vItemSize;
		if(szBuf == NULL){
			// 自动分配空间
			pRb->vFlag = RB_FLAG_ALLOC;

			vBufSize = pRb->vCount*pRb->vSize;
			pRb->szBuf = malloc(vBufSize);
			rjudge(pRb->szBuf==NULL, -2);

		}
		else{
			// 用户提供缓存
			pRb->vFlag = RB_FLAG_USER;

			pRb->szBuf = szBuf;
		}
	}

	RD_END
}

int RingBuf_Del(CRingBuf *pRb)
{
	RET_START

	rjudge(pRb==NULL, -1);

	if(pRb->vFlag == RB_FLAG_ALLOC){
        if(pRb->szBuf != NULL){
            free(pRb->szBuf);
        }
	}
	memset(pRb, 0, sizeof(CRingBuf));

	RD_END
}

int RingBuf_IsFull(CRingBuf *pRb)
{
	RET_START
	int i;
	unsigned int vBufSize;
	int vFull = 0;

	rjudge(pRb==NULL, -1);

	vBufSize = RingBuf_BufSize(pRb);
	for(i=0; i<pRb->vSize; i++){
		if((pRb->vWrite+i+1) % vBufSize == pRb->vRead){
			vFull = 1;
			break;
		}
	}

RD_LAB
	return vFull;
}

int RingBuf_IsEmpty(CRingBuf *pRb)
{
	RET_START
	int vEmpty = 0;

	rjudge(pRb==NULL, -1);

    if(pRb->vWrite == pRb->vRead){
		vEmpty = 1;
	}

RD_LAB
	return vEmpty;
}

int RingBuf_ValidCount(CRingBuf *pRb)
{
	RET_START
	int vCount = 0;
	unsigned int vRead;
	unsigned int vWrite;
	unsigned int vBufSize;

	rjudge(pRb==NULL, -1);

	vRead = pRb->vRead;
	vWrite = pRb->vWrite;
	vBufSize = RingBuf_BufSize(pRb);
	if(vWrite != vRead){
		while(1){
			if((vRead + 1) % vBufSize == vWrite){
				vCount += 1;
				vCount /= pRb->vSize;
				rb_printf("count: %d\r\n", vCount);
				break;
			}
			else{
				vCount += 1;
				vRead = (vRead + 1) % vBufSize;
			}
		}
	}

RD_LAB
	return vCount;
}

int RingBuf_BufSize(CRingBuf *pRb)
{
	RET_START
	int vBufSize = 0;

	rjudge(pRb==NULL, -1);

	vBufSize = pRb->vCount*pRb->vSize;

RD_LAB
	return vBufSize;
}

int RingBuf_Put(CRingBuf *pRb, const void *pItem)
{
	RET_START
	int i;
	int vBufSize;
	unsigned char *szBuf;
	const unsigned char *szData;

	rjudge(pRb==NULL, -1);
	rjudge(RingBuf_IsFull(pRb) == 1, -2);

	szBuf = (RB_FLAG_FLAT==pRb->vFlag) ? pRb->szData : pRb->szBuf;
	szData = pItem;
	vBufSize = RingBuf_BufSize(pRb);
	for(i=0; i<pRb->vSize; i++){
		szBuf[pRb->vWrite] = szData[i];
		pRb->vWrite = (pRb->vWrite + 1) % vBufSize;
	}

	RD_END
}

int RingBuf_Get(CRingBuf *pRb, void *pItem)
{
	RET_START
	int i;
	int vBufSize;
	unsigned char *szBuf;
	unsigned char *szData;

	rjudge(pRb==NULL, -1);
	rjudge(RingBuf_IsEmpty(pRb) == 1, -2);

	szBuf = (RB_FLAG_FLAT==pRb->vFlag) ? pRb->szData : pRb->szBuf;
	szData = pItem;
	vBufSize = RingBuf_BufSize(pRb);
	for(i=0; i<pRb->vSize; i++){
		szData[i] = szBuf[pRb->vRead];
		pRb->vRead = (pRb->vRead + 1) % vBufSize;
		//rb_printf("_%02x_", *pChar);
	}

	R_END
}

int RingBuf_Write(CRingBuf *pRb, const void *szData, int vCount)
{
	int i;
	int vRes;
	const unsigned char *szBuf = szData;

	for(i=0; i<vCount; i++)
	{
		vRes = RingBuf_Put(pRb, szBuf+i*pRb->vSize);
		if(vRes != 0){
			break;
		}
	}
	//rb_printh(szBuf, vLen);

	return i;
}

int RingBuf_Read(CRingBuf *pRb, void *szData, int vCount)
{
	int i;
	int vRes;
	unsigned char *szBuf = szData;

	for(i=0; i<vCount; i++)
	{
		vRes = RingBuf_Get(pRb, szBuf+i*pRb->vSize);
		if(vRes != 0){
			break;
		}
	}
	//rb_printh(szBuf, vLen);

	return i;
}

