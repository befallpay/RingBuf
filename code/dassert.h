#ifndef __DASSERT_H
#define __DASSERT_H


#include <stdio.h>
#include <stdarg.h>


#define DA_BUF_SIZE				1024

#define _LF_STR_				"\r\n"
#define _LF_TAB_				"\r\n\t"


// ------------------基础调试信息输出------------------------

#define dprintf(fmt_s, ...)		printf(fmt_s, ##__VA_ARGS__)
// strTip为NULL时会报warning，改为传空字串
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


// 格式化输出：带函数名，换行
#define da_printf(...)   \
            ({ \
                dprintf("%s-- ", __FUNCTION__); \
                dprintf(__VA_ARGS__);  \
                dprintf(_LF_STR_);  \
            })
// hex输出：带函数名，换行
#define da_printh(szAddr, vLen)   dprinth(1, __FUNCTION__, szAddr, vLen, 1)


// ------------------函数返回值处理宏------------------------

#define RetPrintf(...)		da_printf(__VA_ARGS__)

// 声明并设置默认错误码
#define RET_START			int ret=0;

// ------------------不附带任何调试信息的接口集------------------------

// exp为true,说明发生错误,则跳转
#define rjudge(exp, err)    do{ \
                                if(exp){  \
                                    ret = err;  \
                                    goto LB_END;    \
                                }   \
                            }while(0)
// 拆解:设置默认错误码
#define rset(err)       ret = err;
// 拆解:直接跳转
#define rjump()         goto LB_END
// 拆解:设置错误码并跳转
#define rend(err)       do{ \
                            ret = err;    \
                            goto LB_END;    \
                        }while(0)

// 返回
#define R_END       LB_END:     \
                        return ret;
// 拆解:结束标签(可以用于void返回值类型的函数)
#define RET_LAB     LB_END:
// 拆解:返回码
#define retno       (ret)
// 拆解:返回
#define RET_RET     return ret;

// 拆解:通用的返回值判断
#define RetErr()    (ret != 0)
// 拆解:返回值打印
#define RetPrint()  RetPrintf("ret:%d", ret)


// ------------------带默认调试信息的接口集------------------------

#define rdjudge(exp, err)   do{ \
                                if(exp){  \
                                    ret = err;    \
                                    RetPrintf(" %d", ret);  \
                                    goto LB_END;    \
                                }   \
                            }while(0)
// 拆解:设置错误码并跳转
#define rdend(err)      do{ \
                            ret = err;    \
                            RetPrintf(" %d", ret);  \
                            goto LB_END;    \
                        }while(0)
// 返回
#define RD_END      LB_END:     \
                        if(RetErr())    \
                            RetPrintf(" %d", ret);  \
                        return ret;
// 拆解:打印错误信息
#define RD_LAB      LB_END:     \
                        if(RetErr())    \
                            RetPrintf(" %d", ret);

// ------------------带错误处理的接口集------------------------

// 自定义的错误处理: 死循环提示
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
// 拆解:设置错误码并跳转
#define rlend(err)	do{ \
                        ret = err;    \
                        ErrDeal(ret);  \
                        goto LB_END;    \
                    }while(0)
// 返回
#define RL_END      LB_END:     \
                        if(RetErr())    \
                            ErrDeal(ret);  \
                        return ret;
// 拆解:默认错误处理
#define RL_LAB      LB_END:     \
                        if(RetErr())    \
                            ErrDeal(ret);


// ------------------标准断言------------------------

// 标准断言函数实现
#define dassert(judge)  do{ \
                            if(!(judge))    \
                                ErrDeal();  \
                        }while(0)


#endif
