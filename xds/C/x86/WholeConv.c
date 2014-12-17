/* XDS v2.60: Copyright (c) 1999-2011 Excelsior, LLC. All Rights Reserved. */
/* "@(#)WholeConv.c Feb  3 14:30:28 2012" */
/* Generated by XDS Modula-2 to ANSI C v4.20 translator */

#define X2C_int32
#define X2C_index32
#ifndef WholeConv_H_
#include "WholeConv.h"
#endif
#define WholeConv_C_
#ifndef ConvTypes_H_
#include "ConvTypes.h"
#endif
#ifndef CharClass_H_
#include "CharClass.h"
#endif
#ifndef WholeStr_H_
#include "WholeStr.h"
#endif
#ifndef EXCEPTIONS_H_
#include "EXCEPTIONS.h"
#endif

static EXCEPTIONS_ExceptionSource source;

#define WholeConv_ok ConvTypes_strAllRight


static void raise0(void)
{
   EXCEPTIONS_RAISE(source, 0UL, "WholeConv.Exception", 20ul);
} /* end raise() */

static void Inv(X2C_CHAR, X2C_CARD8 *, ConvTypes_ScanState *);


static void Inv(X2C_CHAR ch, X2C_CARD8 * class0, ConvTypes_ScanState * next)
{
   ch = ch;
   *class0 = ConvTypes_invalid;
   *next = (ConvTypes_ScanState)Inv;
} /* end Inv() */

static void W(X2C_CHAR, X2C_CARD8 *, ConvTypes_ScanState *);


static void W(X2C_CHAR ch, X2C_CARD8 * class0, ConvTypes_ScanState * next)
{
   if (CharClass_IsNumeric(ch)) {
      *class0 = ConvTypes_valid;
      *next = (ConvTypes_ScanState)W;
   }
   else {
      *class0 = ConvTypes_terminator;
      *next = (ConvTypes_ScanState)Inv;
   }
} /* end W() */

static void S(X2C_CHAR, X2C_CARD8 *, ConvTypes_ScanState *);


static void S(X2C_CHAR ch, X2C_CARD8 * class0, ConvTypes_ScanState * next)
{
   if (CharClass_IsNumeric(ch)) {
      *class0 = ConvTypes_valid;
      *next = (ConvTypes_ScanState)W;
   }
   else {
      *class0 = ConvTypes_invalid;
      *next = (ConvTypes_ScanState)Inv;
   }
} /* end S() */


extern void WholeConv_ScanInt(X2C_CHAR ch, X2C_CARD8 * class0,
                ConvTypes_ScanState * next)
{
   if (CharClass_IsWhiteSpace(ch)) {
      *class0 = ConvTypes_padding;
      *next = (ConvTypes_ScanState)WholeConv_ScanInt;
   }
   else if (ch=='-' || ch=='+') {
      *class0 = ConvTypes_valid;
      *next = (ConvTypes_ScanState)S;
   }
   else if (CharClass_IsNumeric(ch)) {
      *class0 = ConvTypes_valid;
      *next = (ConvTypes_ScanState)W;
   }
   else {
      *class0 = ConvTypes_invalid;
      *next = (ConvTypes_ScanState)Inv;
   }
} /* end ScanInt() */


extern X2C_CARD8 WholeConv_FormatInt(X2C_CHAR s[], X2C_CARD32 s_len)
{
   X2C_INT32 n;
   X2C_CARD8 res;
   X2C_CARD8 WholeConv_FormatInt_ret;
   X2C_PCOPY((void **)&s,s_len);
   WholeStr_StrToInt(s, s_len, &n, &res);
   WholeConv_FormatInt_ret = res;
   X2C_PFREE(s);
   return WholeConv_FormatInt_ret;
} /* end FormatInt() */


extern X2C_INT32 WholeConv_ValueInt(X2C_CHAR s[], X2C_CARD32 s_len)
{
   X2C_INT32 n;
   X2C_CARD8 res;
   X2C_INT32 WholeConv_ValueInt_ret;
   X2C_PCOPY((void **)&s,s_len);
   n = 0L;
   WholeStr_StrToInt(s, s_len, &n, &res);
   if (res) raise0();
   WholeConv_ValueInt_ret = n;
   X2C_PFREE(s);
   return WholeConv_ValueInt_ret;
} /* end ValueInt() */


extern X2C_CARD32 WholeConv_LengthInt(X2C_INT32 int0)
{
   X2C_CARD32 n;
   n = 0UL;
   if (int0<0L) {
      n = 1UL;
      if (int0==X2C_min_longint) int0 = X2C_max_longint;
      else int0 = -int0;
   }
   do {
      ++n;
      int0 = X2C_DIV(int0,10L);
   } while (int0);
   return n;
} /* end LengthInt() */


extern void WholeConv_ScanCard(X2C_CHAR ch, X2C_CARD8 * class0,
                ConvTypes_ScanState * next)
{
   if (CharClass_IsWhiteSpace(ch)) {
      *class0 = ConvTypes_padding;
      *next = (ConvTypes_ScanState)WholeConv_ScanCard;
   }
   else if (CharClass_IsNumeric(ch)) {
      *class0 = ConvTypes_valid;
      *next = (ConvTypes_ScanState)W;
   }
   else {
      *class0 = ConvTypes_invalid;
      *next = (ConvTypes_ScanState)Inv;
   }
} /* end ScanCard() */


extern X2C_CARD8 WholeConv_FormatCard(X2C_CHAR s[], X2C_CARD32 s_len)
{
   X2C_CARD32 n;
   X2C_CARD8 res;
   X2C_CARD8 WholeConv_FormatCard_ret;
   X2C_PCOPY((void **)&s,s_len);
   WholeStr_StrToCard(s, s_len, &n, &res);
   WholeConv_FormatCard_ret = res;
   X2C_PFREE(s);
   return WholeConv_FormatCard_ret;
} /* end FormatCard() */


extern X2C_CARD32 WholeConv_ValueCard(X2C_CHAR s[], X2C_CARD32 s_len)
{
   X2C_CARD32 n;
   X2C_CARD8 res;
   X2C_CARD32 WholeConv_ValueCard_ret;
   X2C_PCOPY((void **)&s,s_len);
   n = 0UL;
   WholeStr_StrToCard(s, s_len, &n, &res);
   if (res) raise0();
   WholeConv_ValueCard_ret = n;
   X2C_PFREE(s);
   return WholeConv_ValueCard_ret;
} /* end ValueCard() */


extern X2C_CARD32 WholeConv_LengthCard(X2C_CARD32 card)
{
   X2C_CARD32 n;
   n = 0UL;
   do {
      ++n;
      card = card/10UL;
   } while (card);
   return n;
} /* end LengthCard() */


extern X2C_BOOLEAN WholeConv_IsWholeConvException(void)
{
   return EXCEPTIONS_IsCurrentSource(source);
} /* end IsWholeConvException() */


extern void WholeConv_BEGIN(void)
{
   static int WholeConv_init = 0;
   if (WholeConv_init) return;
   WholeConv_init = 1;
   EXCEPTIONS_BEGIN();
   WholeStr_BEGIN();
   CharClass_BEGIN();
   ConvTypes_BEGIN();
   EXCEPTIONS_AllocateSource(&source);
}

