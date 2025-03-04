#include "dutil.h"
#include "RingBuf.h"


#define rb_printf(fmt_s, ...)       printf(fmt_s, ##__VA_ARGS__)

int RingBuf_PutChar(CRingBuf *pRb, char vChar)
{
    return RingBuf_Put(pRb, &vChar);
}


int RingBuf_Test1(void)
{
    unsigned char vVaule;
	CRingBuf tRb;
    CRingBuf *pRb = &tRb;

	RingBuf_New(&tRb, NULL, 4, 1);  // Create a ring buffer with size 4

    RingBuf_PutChar(pRb, 'A');
    RingBuf_PutChar(pRb, 'B');
    RingBuf_PutChar(pRb, 'C');
    RingBuf_PutChar(pRb, 'D');

    RingBuf_Get(pRb, &vVaule);
    rb_printf("Get: %c\n", vVaule);  // Should print 'A'
    RingBuf_Get(pRb, &vVaule);
    rb_printf("Get: %c\n", vVaule);  // Should print 'B'

    RingBuf_PutChar(pRb, 'E');  // This will overwrite 'C'

    RingBuf_Get(pRb, &vVaule);
    rb_printf("Get: %c\n", vVaule);  // Should print 'C' (overwritten)
    RingBuf_Get(pRb, &vVaule);
    rb_printf("Get: %c\n", vVaule);  // Should print 'D'
    RingBuf_Get(pRb, &vVaule);
    rb_printf("Get: %c\n", vVaule);  // Should print 'E'

    RingBuf_Del(pRb);

    return 0;
}

int RingBuf_Test2(void)
{
	const char *strIn = "12345678901234567890\n";
	unsigned char szData[512];
	int vOutLen, vInLen;
	int vRes;
    CRingBuf tRb;
    CRingBuf *pRb = &tRb;

	RingBuf_New(&tRb, NULL, 512, 1);

	vInLen = strlen(strIn);
    vRes = RingBuf_Write(pRb, (unsigned char*)strIn, vInLen);
    rb_printf("RingBuf_Write: %d\n", vRes);

	vOutLen = RingBuf_ValidCount(pRb);
	rb_printf("vInLen: %d, vOutLen: %d\n", vInLen, vOutLen);

    vRes = RingBuf_Read(pRb, szData, vOutLen);
    rb_printf("RingBuf_Read: %d\n", vRes);

	vRes = memcmp(strIn, szData, vInLen);
	rb_printf("vRes: %d\n", vRes);

    RingBuf_Del(pRb);

    return 0;
}

int main()
{
    printf("Hello world!\n");

    RingBuf_Test1();
    RingBuf_Test2();

    return 0;
}
