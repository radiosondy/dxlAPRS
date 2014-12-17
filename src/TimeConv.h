/* XDS v2.51: Copyright (c) 1999-2003 Excelsior, LLC. All Rights Reserved. */
/* Generated by XDS Modula-2 to ANSI C v4.20 translator */

#ifndef TimeConv_H_
#define TimeConv_H_
#ifndef X2C_H_
#include "X2C.h"
#endif
#ifndef SysClock_H_
#include "SysClock.h"
#endif

typedef struct SysClock_DateTime TimeConv_DateTime;

extern X2C_INT32 TimeConv_Compare(struct SysClock_DateTime,
                struct SysClock_DateTime);

extern X2C_CARD32 TimeConv_SubDateDays(struct SysClock_DateTime,
                struct SysClock_DateTime);

extern X2C_CARD32 TimeConv_SubDateSecs(struct SysClock_DateTime,
                struct SysClock_DateTime);

extern void TimeConv_AddDateDays(struct SysClock_DateTime, X2C_CARD32,
                struct SysClock_DateTime *);

extern void TimeConv_AddDateSecs(struct SysClock_DateTime, X2C_CARD32,
                struct SysClock_DateTime *);

extern X2C_CARD32 TimeConv_TheDayNumber(struct SysClock_DateTime);

extern X2C_CARD32 TimeConv_TheFractionNumber(struct SysClock_DateTime);

extern X2C_CARD32 TimeConv_WeekDay(struct SysClock_DateTime);

extern X2C_CARD32 TimeConv_millisecs(void);

extern X2C_CARD32 TimeConv_time(void);

extern void TimeConv_unpack(struct SysClock_DateTime *, X2C_CARD32);

extern void TimeConv_pack(struct SysClock_DateTime, X2C_CARD32 *);

extern X2C_CARD32 TimeConv_weekday0(X2C_CARD32);


extern void TimeConv_BEGIN(void);


#endif /* TimeConv_H_ */
