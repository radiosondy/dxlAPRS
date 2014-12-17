/* XDS v2.60: Copyright (c) 1999-2011 Excelsior, LLC. All Rights Reserved. */
/* "@(#)TextIO.c Feb  3 14:30:28 2012" */
/* Generated by XDS Modula-2 to ANSI C v4.20 translator */

#define X2C_int32
#define X2C_index32
#ifndef TextIO_H_
#include "TextIO.h"
#endif
#define TextIO_C_
#ifndef IOChan_H_
#include "IOChan.h"
#endif
#ifndef IOConsts_H_
#include "IOConsts.h"
#endif
#ifndef CharClass_H_
#include "CharClass.h"
#endif

#define TextIO_ok IOConsts_allRight


extern void TextIO_ReadChar(IOChan_ChanId cid, X2C_CHAR * ch)
{
   X2C_CARD32 n;
   *ch = 0;
   IOChan_TextRead(cid, (X2C_ADDRESS)ch, 1UL, &n);
} /* end ReadChar() */


extern void TextIO_ReadRestLine(IOChan_ChanId cid, X2C_CHAR s[],
                X2C_CARD32 s_len)
{
   X2C_CARD32 n;
   X2C_CARD8 res;
   X2C_CHAR ch;
   IOChan_TextRead(cid, (X2C_ADDRESS)s, (s_len-1)+1UL, &n);
   res = IOChan_ReadResult(cid);
   if (n<=s_len-1) s[n] = 0;
   else if (res==IOConsts_allRight) {
      do {
         IOChan_TextRead(cid, (X2C_ADDRESS) &ch, 1UL, &n);
      } while (IOChan_ReadResult(cid)==IOConsts_allRight);
      IOChan_SetReadResult(cid, IOConsts_outOfRange);
   }
} /* end ReadRestLine() */


extern void TextIO_ReadString(IOChan_ChanId cid, X2C_CHAR s[],
                X2C_CARD32 s_len)
{
   X2C_CARD32 n;
   IOChan_TextRead(cid, (X2C_ADDRESS)s, (s_len-1)+1UL, &n);
   if (n<=s_len-1) s[n] = 0;
} /* end ReadString() */


extern void TextIO_ReadToken(IOChan_ChanId cid, X2C_CHAR s[],
                X2C_CARD32 s_len)
{
   X2C_CARD8 res;
   X2C_CHAR ch;
   X2C_CARD32 i;
   X2C_BOOLEAN valid;
   do {
      IOChan_Look(cid, &ch, &res);
      if (res==IOConsts_allRight) {
         valid = !CharClass_IsWhiteSpace(ch);
         IOChan_Skip(cid);
      }
   } while (!(res!=IOConsts_allRight || valid));
   i = 0UL;
   while ((res==IOConsts_allRight && valid) && i<=s_len-1) {
      s[i] = ch;
      ++i;
      IOChan_Look(cid, &ch, &res);
      valid = !CharClass_IsWhiteSpace(ch);
      if (res==IOConsts_allRight && valid) IOChan_Skip(cid);
   }
   if (i<=s_len-1) {
      s[i] = 0;
      if (i>0UL) IOChan_SetReadResult(cid, IOConsts_allRight);
      return;
   }
   if (res==IOConsts_allRight && valid) {
      IOChan_SetReadResult(cid, IOConsts_outOfRange);
      return;
   }
   IOChan_SetReadResult(cid, res);
} /* end ReadToken() */


extern void TextIO_SkipLine(IOChan_ChanId cid)
{
   X2C_CARD8 res;
   X2C_CHAR ch;
   do {
      TextIO_ReadChar(cid, &ch);
      res = IOChan_ReadResult(cid);
   } while (res==IOConsts_allRight);
   if (res==IOConsts_endOfLine) IOChan_Skip(cid);
} /* end SkipLine() */


extern void TextIO_WriteChar(IOChan_ChanId cid, X2C_CHAR ch)
{
   X2C_CHAR a[1];
   a[0U] = ch;
   IOChan_TextWrite(cid, (X2C_ADDRESS)a, 1UL);
} /* end WriteChar() */


extern void TextIO_WriteLn(IOChan_ChanId cid)
{
   IOChan_WriteLn(cid);
} /* end WriteLn() */


extern void TextIO_WriteString(IOChan_ChanId cid, X2C_CHAR s[],
                X2C_CARD32 s_len)
{
   IOChan_TextWrite(cid, (X2C_ADDRESS)s, X2C_LENGTH(s,s_len));
} /* end WriteString() */


extern void TextIO_BEGIN(void)
{
   static int TextIO_init = 0;
   if (TextIO_init) return;
   TextIO_init = 1;
   CharClass_BEGIN();
   IOConsts_BEGIN();
   IOChan_BEGIN();
}

