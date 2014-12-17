/* XDS v2.60: Copyright (c) 1999-2011 Excelsior, LLC. All Rights Reserved. */
/* "@(#)LowReal.c Feb  3 14:30:28 2012" */
/* Generated by XDS Modula-2 to ANSI C v4.20 translator */

#define X2C_int32
#define X2C_index32
#ifndef LowReal_H_
#include "LowReal.h"
#endif
#define LowReal_C_
#ifndef EXCEPTIONS_H_
#include "EXCEPTIONS.h"
#endif
#include "xMath.h"

static EXCEPTIONS_ExceptionSource source;


static void raise0(void)
{
   EXCEPTIONS_RAISE(source, 0UL, "LowReal.lowException", 21ul);
} /* end raise() */


extern X2C_INT32 LowReal_exponent(X2C_REAL x)
{
   int i;
   X2C_LONGREAL z;
   z = X2C_frexp((X2C_LONGREAL)x, &i);
   return i;
} /* end exponent() */


extern X2C_REAL LowReal_fraction(X2C_REAL x)
{
   int i;
   return (X2C_REAL)X2C_frexp((X2C_LONGREAL)x, &i);
} /* end fraction() */


extern X2C_REAL LowReal_sign(X2C_REAL x)
{
   if (x<0.0f) return (-1.0f);
   else if (x==0.0f) return 0.0f;
   else return 1.0f;
   return 0;
} /* end sign() */


extern X2C_REAL LowReal_succ(X2C_REAL x)
{
   return x+LowReal_ulp(x);
} /* end succ() */


extern X2C_REAL LowReal_ulp(X2C_REAL x)
{
   X2C_CARD32 * pb;
   pb = (X2C_CARD32 *) &x;
   *pb |= 0x1UL;
   return (X2C_REAL)fabs(x-LowReal_trunc(x, 22L));
} /* end ulp() */


extern X2C_REAL LowReal_pred(X2C_REAL x)
{
   return x-LowReal_ulp(x);
} /* end pred() */


extern X2C_REAL LowReal_intpart(X2C_REAL x)
{
   X2C_LONGREAL z;
   X2C_LONGREAL y;
   z = X2C_modf((X2C_LONGREAL)x, &y);
   return (X2C_REAL)y;
} /* end intpart() */


extern X2C_REAL LowReal_fractpart(X2C_REAL x)
{
   X2C_LONGREAL y;
   return (X2C_REAL)X2C_modf((X2C_LONGREAL)x, &y);
} /* end fractpart() */


extern X2C_REAL LowReal_scale(X2C_REAL x, X2C_INT32 n)
{
   return (X2C_REAL)X2C_ldexp((X2C_LONGREAL)x, n);
} /* end scale() */


extern X2C_REAL LowReal_trunc(X2C_REAL x, X2C_INT32 n)
{
   X2C_CARD32 * pb;
   if (n<=0L) raise0();
   if (n>=23L) return x;
   pb = (X2C_CARD32 *) &x;
   *pb = *pb&~(X2C_SET(0U,(23L-n)-1L,32));
   return x;
} /* end trunc() */


extern X2C_REAL LowReal_round(X2C_REAL x, X2C_INT32 n)
{
   return LowReal_trunc(x, n);
} /* end round() */


extern X2C_REAL LowReal_synthesize(X2C_INT32 n, X2C_REAL x)
{
   return (X2C_REAL)X2C_ldexp((X2C_LONGREAL)x, n);
} /* end synthesize() */


extern void LowReal_setMode(X2C_CARD8 m)
{
   X2C_CARD32 r;
   r = X2C_controlfp((X2C_CARD32)m, (X2C_CARD32)0x3FUL);
} /* end setMode() */


extern X2C_CARD8 LowReal_currentMode(void)
{
   return (X2C_CARD8)X2C_controlfp(0UL, 0UL);
} /* end currentMode() */


extern X2C_BOOLEAN LowReal_IsLowException(void)
{
   return EXCEPTIONS_IsCurrentSource(source);
} /* end IsLowException() */


extern void LowReal_BEGIN(void)
{
   static int LowReal_init = 0;
   if (LowReal_init) return;
   LowReal_init = 1;
   if (sizeof(X2C_CARD8)!=1) X2C_ASSERT(0);
   EXCEPTIONS_BEGIN();
   EXCEPTIONS_AllocateSource(&source);
}

