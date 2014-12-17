/* XDS v2.60: Copyright (c) 1999-2011 Excelsior, LLC. All Rights Reserved. */
/* "@(#)In.c Feb  3 14:31:17 2012" */
/* Generated by XDS Oberon-2 to ANSI C v4.20 translator */

#define X2C_int32
#define X2C_index32
#ifndef In_H_
#include "In.h"
#endif
#define In_C_
#ifndef CharClass_H_
#include "CharClass.h"
#endif
#ifndef IOChan_H_
#include "IOChan.h"
#endif
#ifndef IOConsts_H_
#include "IOConsts.h"
#endif
#ifndef StdChans_H_
#include "StdChans.h"
#endif

extern struct X2C_MD_STR In_desc;
static X2C_CHAR In_EOL = 0xAU ;

X2C_BOOLEAN In_Done;
static IOChan_ChanId inp0;


extern void In_Open(void)
{
   In_Done = 1;
   inp0 = StdChans_InChan();
} /* end Open() */


static void Skip(void)
{
   X2C_CARD8 res;
   X2C_CHAR ch;
   if (In_Done) {
      for (;;) {
         IOChan_Look(inp0, &ch, &res);
         switch ((unsigned)res) {
         case IOConsts_allRight:
            if (CharClass_IsWhiteSpace(ch)) IOChan_Skip(inp0);
            else goto loop_exit;
            break;
         case IOConsts_endOfLine:
            IOChan_Skip(inp0);
            break;
         case IOConsts_endOfInput:
            In_Done = 0;
            goto loop_exit;
         default:
            X2C_TRAP(X2C_CASE_TRAP);
         } /* end switch */
      }
      loop_exit:;
   }
} /* end Skip() */


extern void In_Char(X2C_CHAR * ch)
{
   X2C_CARD8 res;
   if (In_Done) {
      IOChan_Look(inp0, ch, &res);
      switch ((unsigned)res) {
      case IOConsts_allRight:
         IOChan_Skip(inp0);
         break;
      case IOConsts_endOfLine:
         IOChan_Skip(inp0);
         *ch = '\012';
         break;
      case IOConsts_endOfInput:
         In_Done = 0;
         *ch = 0x0U ;
         break;
      default:
         X2C_TRAP(X2C_CASE_TRAP);
      } /* end switch */
   }
} /* end Char() */


extern void In_String(X2C_CHAR str[], X2C_CARD32 str_len)
{
   X2C_CHAR ch;
   X2C_INT32 i;
   Skip();
   if (In_Done) {
      In_Char(&ch);
      if (ch!='\"') In_Done = 0;
      else {
         i = 0L;
         In_Char(&ch);
         while (In_Done && ch!='\"') {
            if ((X2C_CARD8)ch<' ' || i>=str_len) {
               In_Done = 0;
               return;
            }
            str[i] = ch;
            ++i;
            In_Char(&ch);
         }
         if (In_Done && i==0L) In_Done = 0;
         if (i<str_len) str[i] = 0x0U ;
      }
   }
} /* end String() */


extern void In_Name(X2C_CHAR name[], X2C_CARD32 name_len)
{
   X2C_CARD8 res;
   X2C_CHAR ch;
   X2C_INT32 i;
   Skip();
   if (In_Done) {
      i = 0L;
      for (;;) {
         IOChan_Look(inp0, &ch, &res);
         if (res!=IOConsts_allRight || CharClass_IsWhiteSpace(ch)) break;
         IOChan_Skip(inp0);
         if (i<name_len-1L) {
            name[i] = ch;
            ++i;
         }
      }
      if (i<name_len) name[i] = 0x0U ;
      In_Done = i && i<name_len;
   }
} /* end Name() */


static void ReadInt(X2C_INT32 * num)
{
   X2C_CHAR string[64];
   X2C_INT16 n;
   X2C_INT16 k;
   X2C_INT16 i;
   X2C_BOOLEAN hex;
   X2C_CHAR dig;
   X2C_CHAR ch;
   X2C_INT32 val;
   X2C_CARD8 res;
   X2C_INT16 tmp;
   *num = 0L;
   val = 0L;
   hex = 0;
   i = 0;
   for (;;) {
      IOChan_Look(inp0, &ch, &res);
      if (res!=IOConsts_allRight) break;
      ch = X2C_CAP(ch);
      if ((X2C_CARD8)ch>='0' && (X2C_CARD8)ch<='9') {
         dig = (X2C_CHAR)((X2C_INT16)(X2C_CARD8)ch-48);
      }
      else if ((X2C_CARD8)ch>='A' && (X2C_CARD8)ch<='F') {
         dig = (X2C_CHAR)(((X2C_INT16)(X2C_CARD8)ch-65)+10);
         hex = 1;
      }
      else break;
      IOChan_Skip(inp0);
      if (i>0 || ch!='0') {
         if (i<64) {
            string[i] = dig;
            ++i;
         }
      }
   }
   if (res==IOConsts_allRight && ch=='H') {
      IOChan_Skip(inp0);
      tmp = i-1;
      k = 0;
      if (k<=tmp) for (;; k++) {
         n = (X2C_INT16)(X2C_CARD8)string[k];
         if (val<=X2C_DIV(X2C_max_longint-(X2C_INT32)n,16L)) {
            val = val*16L+(X2C_INT32)n;
         }
         else {
            In_Done = 0;
            return;
         }
         if (k==tmp) break;
      } /* end for */
      *num = val;
   }
   else if (hex) {
      In_Done = 0;
      *num = 0L;
   }
   else {
      tmp = i-1;
      k = 0;
      if (k<=tmp) for (;; k++) {
         n = (X2C_INT16)(X2C_CARD8)string[k];
         if (val<=X2C_DIV(X2C_max_longint-(X2C_INT32)n,10L)) {
            val = val*10L+(X2C_INT32)n;
         }
         else {
            In_Done = 0;
            return;
         }
         if (k==tmp) break;
      } /* end for */
      *num = val;
   }
} /* end ReadInt() */


static X2C_BOOLEAN IsNumeric(void)
{
   X2C_CARD8 res;
   X2C_CHAR ch;
   IOChan_Look(inp0, &ch, &res);
   return res==IOConsts_allRight && CharClass_IsNumeric(ch);
} /* end IsNumeric() */


extern void In_Int(X2C_INT16 * n)
{
   X2C_INT32 x;
   Skip();
   if (In_Done) {
      if (IsNumeric()) {
         ReadInt(&x);
         if (In_Done && x<=32767L) *n = (X2C_INT16)x;
         else In_Done = 0;
      }
      else In_Done = 0;
   }
} /* end Int() */


extern void In_LongInt(X2C_INT32 * n)
{
   Skip();
   if (In_Done) {
      if (IsNumeric()) ReadInt(n);
      else In_Done = 0;
   }
} /* end LongInt() */


static void Ten(X2C_INT32 e, X2C_BOOLEAN * ovf, X2C_LONGREAL * t)
{
   X2C_LONGREAL pow0;
   *ovf = 0;
   *t = 1.0;
   pow0 = 10.0;
   while (e) {
      if ((e&1)) {
         if (X2C_DIVL(X2C_max_longreal,pow0)<=*t) {
            *ovf = 1;
            return;
         }
         *t =  *t*pow0;
      }
      e = X2C_DIV(e,2L);
      if (X2C_DIVL(X2C_max_longreal,pow0)>=pow0) pow0 = pow0*pow0;
      else {
         if (e) *ovf = 1;
         return;
      }
   }
} /* end Ten() */

static X2C_CARD32 In_final = 0x26UL;


static void ReadReal(X2C_LONGREAL * real, X2C_BOOLEAN * long0)
{
   X2C_INT16 n;
   X2C_INT16 s;
   X2C_INT32 e;
   X2C_LONGREAL p;
   X2C_LONGREAL t;
   X2C_LONGREAL r;
   X2C_BOOLEAN neg;
   X2C_CHAR ch;
   X2C_CARD8 res;
   X2C_BOOLEAN ovf;
   *long0 = 0;
   neg = 0;
   ovf = 0;
   e = 0L;
   s = 0;
   r = 0.0;
   p = 1.0;
   for (;;) {
      IOChan_Look(inp0, &ch, &res);
      if (res!=IOConsts_allRight) break;
      switch ((unsigned)ch) {
      case '+':
      case '-':
         if (s!=3) goto loop_exit;
         neg = ch=='-';
         s = 4;
         break;
      case '.':
         if (s!=1) goto loop_exit;
         e = 0L;
         s = 2;
         break;
      case 'E':
      case 'D':
         if (s!=2) {
            s = 0;
            goto loop_exit;
         }
         *long0 = ch=='D';
         s = 3;
         break;
      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
         n = (X2C_INT16)(X2C_CARD8)ch-48;
         switch (s) {
         case 0:
         case 1:
            if (r>=X2C_DIVL(X2C_max_longreal-(X2C_LONGREAL)n,10.0)) ovf = 1;
            if (!ovf) r = r*10.0+(X2C_LONGREAL)n;
            s = 1;
            break;
         case 2:
            if (!ovf) {
               p = X2C_DIVL(p,10.0);
               r = r+p*(X2C_LONGREAL)n;
            }
            break;
         case 3:
         case 4:
         case 5:
            if (e>=X2C_DIV(X2C_max_longint-(X2C_INT32)n,10L)) ovf = 1;
            if (!ovf) e = e*10L+(X2C_INT32)n;
            s = 5;
            break;
         default:;
            goto loop_exit;
         } /* end switch */
         break;
      default:;
         goto loop_exit;
      } /* end switch */
      IOChan_Skip(inp0);
   }
   loop_exit:;
   if (!ovf && X2C_IN((X2C_INT32)s,32,0x26UL)) {
      Ten(e, &ovf, &t);
      if (ovf) {
         In_Done = 0;
         r = 0.0;
      }
      else {
         if (neg) r = X2C_DIVL(r,t);
         else if (X2C_DIVL(X2C_max_longreal,t)<=r) {
            In_Done = 0;
            r = 0.0;
         }
         else r = r*t;
         *real = r;
      }
   }
   else {
      In_Done = 0;
      *real = 0.0;
   }
} /* end ReadReal() */


extern void In_Real(X2C_REAL * x)
{
   X2C_LONGREAL lr;
   X2C_REAL max0;
   X2C_BOOLEAN long0;
   Skip();
   if (In_Done) {
      if (IsNumeric()) {
         ReadReal(&lr, &long0);
         max0 = X2C_max_real;
         if ((In_Done && !long0) && lr<=(X2C_LONGREAL)max0) {
            *x = (X2C_REAL)lr;
         }
         else In_Done = 0;
      }
      else In_Done = 0;
   }
} /* end Real() */


extern void In_LongReal(X2C_LONGREAL * x)
{
   X2C_BOOLEAN long0;
   Skip();
   if (In_Done) {
      if (IsNumeric()) ReadReal(x, &long0);
      else In_Done = 0;
   }
} /* end LongReal() */


static void * In_offs[] = {
   X2C_OFS_END
};
static X2C_PROC In_cmds[] = { In_Open,0 };
static X2C_CHAR * In_cnms[] = { "Open",0 };
struct X2C_MD_STR In_desc = {
  0, 0, "In",In_offs,In_cmds,In_cnms,0
};

extern void In_BEGIN(void)
{
   static int In_init = 0;
   if (In_init) return;
   In_init = 1;
   StdChans_BEGIN();
   IOConsts_BEGIN();
   IOChan_BEGIN();
   CharClass_BEGIN();
   X2C_MODULE(&In_desc);
   inp0 = StdChans_InChan();
}

