#ifndef __DASSERT_H
#define __DASSERT_H


#include <stdio.h>
#include <stdarg.h>


#define DA_BUF_SIZE				1024

#define _LF_STR_				"\r\n"
#define _LF_TAB_				"\r\n\t"


// ------------------����������Ϣ���------------------------

#define dprintf(fmt_s, ...)		printf(fmt_s, ##__VA_ARGS__)
// strTipΪNULLʱ�ᱨwarning����Ϊ�����ִ�
#define dprinth(vLf, strTip, szAddr, vLen, vAlign)	\
({	\
	int _iCnt = 0;	\
	int _iRet = 0;	\
    if(strlen(strTip) > 0){	\
        _iRet += dprintf("[%s]", strTip);	\
    }\
    for(_iCnt=0; _iCnt<vLen; _iCnt++){		\
		if(vAlign == 2){	\
			if(_iCnt % 12 == 0 && vLf != 0)	\
				_iRet += dprintf(_LF_TAB_);	\
			_iRet += dprintf("%04x ", *((unsigned short*)szAddr+_iCnt));	\
		}	\
		else if(vAlign == 4){	\
			if(_iCnt % 8 == 0 && vLf != 0)	\
				_iRet += dprintf(_LF_TAB_);	\
			_iRet += dprintf("%08x ", *((unsigned int*)szAddr+_iCnt));	\
		}	\
		else{	\
			if(_iCnt % 16 == 0 && vLf != 0)	\
				_iRet += dprintf(_LF_TAB_);	\
			_iRet += dprintf("%02x ", *((unsigned char*)szAddr+_iCnt));	\
		}	\
	}	\
    _iRet += dprintf(_LF_STR_);	\
	_iRet;	\
})


// ��ʽ���������������������
#define da_printf(...)   \
            ({ \
                dprintf("%s-- ", __FUNCTION__); \
                dprintf(__VA_ARGS__);  \
                dprintf(_LF_STR_);  \
            })
// hex�������������������
#define da_printh(szAddr, vLen)   dprinth(1, __FUNCTION__, szAddr, vLen, 1)


// ------------------��������ֵ�����------------------------

#define RetPrintf(...)		da_printf(__VA_ARGS__)

// ����������Ĭ�ϴ�����
#define RET_START			int ret=0;

// ------------------�������κε�����Ϣ�Ľӿڼ�------------------------

// expΪtrue,˵����������,����ת
#define rjudge(exp, err)    do{ \
                                if(exp){  \
                                    ret = err;  \
                                    goto LB_END;    \
                                }   \
                            }while(0)
// ���:����Ĭ�ϴ�����
#define rset(err)       ret = err;
// ���:ֱ����ת
#define rjump()         goto LB_END
// ���:���ô����벢��ת
#define rend(err)       do{ \
                            ret = err;    \
                            goto LB_END;    \
                        }while(0)

// ����
#define R_END       LB_END:     \
                        return ret;
// ���:������ǩ(��������void����ֵ���͵ĺ���)
#define RET_LAB     LB_END:
// ���:������
#define retno       (ret)
// ���:����
#define RET_RET     return ret;

// ���:ͨ�õķ���ֵ�ж�
#define RetErr()    (ret != 0)
// ���:����ֵ��ӡ
#define RetPrint()  RetPrintf("ret:%d", ret)


// ------------------��Ĭ�ϵ�����Ϣ�Ľӿڼ�------------------------

#define rdjudge(exp, err)   do{ \
                                if(exp){  \
                                    ret = err;    \
                                    RetPrintf(" %d", ret);  \
                                    goto LB_END;    \
                                }   \
                            }while(0)
// ���:���ô����벢��ת
#define rdend(err)      do{ \
                            ret = err;    \
                            RetPrintf(" %d", ret);  \
                            goto LB_END;    \
                        }while(0)
// ����
#define RD_END      LB_END:     \
                        if(RetErr())    \
                            RetPrintf(" %d", ret);  \
                        return ret;
// ���:��ӡ������Ϣ
#define RD_LAB      LB_END:     \
                        if(RetErr())    \
                            RetPrintf(" %d", ret);

// ------------------��������Ľӿڼ�------------------------

// �Զ���Ĵ�����: ��ѭ����ʾ
#define ErrDeal(ErrNo)		do{ \
								RetPrintf("ErrNo: %d", ErrNo); \
								while(1){dsleep(5*1000);}	\
							}while(0)

#define rljudge(exp, err)	do{ \
                                if(exp){    \
                                    ret = err;   \
                                    ErrDeal(ret);   \
                                    goto LB_END;    \
                                }   \
                            }while(0)
// ���:���ô����벢��ת
#define rlend(err)	do{ \
                        ret = err;    \
                        ErrDeal(ret);  \
                        goto LB_END;    \
                    }while(0)
// ����
#define RL_END      LB_END:     \
                        if(RetErr())    \
                            ErrDeal(ret);  \
                        return ret;
// ���:Ĭ�ϴ�����
#define RL_LAB      LB_END:     \
                        if(RetErr())    \
                            ErrDeal(ret);


// ------------------��׼����------------------------

// ��׼���Ժ���ʵ��
#define dassert(judge)  do{ \
                            if(!(judge))    \
                                ErrDeal();  \
                        }while(0)


#endif
