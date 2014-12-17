/* XDS v2.60: Copyright (c) 1999-2011 Excelsior, LLC. All Rights Reserved. */
/* "@(#)M2EXCEPTION.c Feb  3 14:30:28 2012" */
/* Generated by XDS Modula-2 to ANSI C v4.20 translator */

#define X2C_int32
#define X2C_index32
#ifndef M2EXCEPTION_H_
#include "M2EXCEPTION.h"
#endif
#define M2EXCEPTION_C_
#ifndef EXCEPTIONS_H_
#include "EXCEPTIONS.h"
#endif
#ifndef xmRTS_H_
#include "xmRTS.h"
#endif

static X2C_CARD32 M2EXCEPTION_Fault = 14UL;

static EXCEPTIONS_ExceptionSource Source;


extern X2C_CARD8 M2EXCEPTION_M2Exception(void)
{
   X2C_CARD32 n;
   if (!EXCEPTIONS_IsExceptionalExecut()) {
      EXCEPTIONS_RAISE(Source, 14UL, "No current exception", 21ul);
   }
   if (!EXCEPTIONS_IsCurrentSource(Source)) {
      EXCEPTIONS_RAISE(Source, 14UL, "Not Modula-2 exception source", 30ul);
   }
   n = EXCEPTIONS_CurrentNumber(Source);
   if (n<=14UL) return (X2C_CARD8)n;
   EXCEPTIONS_RAISE(Source, 14UL, "Not Modula-2 exception", 23ul);
   X2C_TRAP(X2C_RETURN_TRAP);
   return 0;
} /* end M2Exception() */


extern X2C_BOOLEAN M2EXCEPTION_IsM2Exception(void)
{
   X2C_CARD32 n;
   if (EXCEPTIONS_IsCurrentSource(Source)) {
      n = EXCEPTIONS_CurrentNumber(Source);
      return n<=14UL;
   }
   return 0;
} /* end IsM2Exception() */


extern void M2EXCEPTION_BEGIN(void)
{
   static int M2EXCEPTION_init = 0;
   if (M2EXCEPTION_init) return;
   M2EXCEPTION_init = 1;
   EXCEPTIONS_BEGIN();
   Source = (EXCEPTIONS_ExceptionSource)X2C_rtsSource;
}

