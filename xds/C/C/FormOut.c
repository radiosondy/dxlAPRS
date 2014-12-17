/* XDS v2.60: Copyright (c) 1999-2011 Excelsior, LLC. All Rights Reserved. */
/* "@(#)FormOut.c Feb  3 14:31:18 2012" */
/* Generated by XDS Modula-2 to ANSI C v4.20 translator */

#define X2C_int32
#define X2C_index32
#ifndef FormOut_H_
#include "FormOut.h"
#endif
#define FormOut_C_
#ifndef XReal_H_
#include "XReal.h"
#endif
#ifndef Strings_H_
#include "Strings.h"
#endif

#define FormOut_CAP_DIG "0123456789ABCDEF"

#define FormOut_SML_DIG "0123456789abcdef"

#define FormOut_MAX_DIG 14

typedef X2C_CHAR * StrPtr;

struct Buffer;


struct Buffer {
   X2C_INT32 wd1;
   X2C_INT32 wd2;
   X2C_INT32 wd3;
   X2C_CARD16 flags;
   X2C_CHAR buf[256];
   X2C_INT32 cnt;
   X2C_INT32 ptr;
};

typedef X2C_CHAR LineSep[4];

static LineSep defaultNL;

static LineSep defaultTL;


static void appString(struct Buffer * b, StrPtr str, X2C_INT32 high)
{
   X2C_INT32 i;
   b->ptr = 0L;
   b->cnt = 0L;
   i = 0L;
   if ((0x100U & b->flags)) {
      while ((i<=high && str[i]) && b->wd3>0L) {
         ++i;
         --b->wd3;
      }
      if (i>high || str[i]==0) return;
   }
   if ((0x80U & b->flags)) {
      while (((b->cnt<=255L && i<=high) && str[i]) && b->wd2>0L) {
         b->buf[b->cnt] = str[i];
         ++b->cnt;
         ++i;
         --b->wd2;
      }
   }
   else {
      while ((b->cnt<=255L && i<=high) && str[i]) {
         b->buf[b->cnt] = str[i];
         ++b->cnt;
         ++i;
      }
   }
} /* end appString() */


static void appChar(struct Buffer * b, X2C_CHAR ch)
{
   b->ptr = 0L;
   b->cnt = 0L;
   if ((0x80U & b->flags)==0) b->wd2 = 1L;
   while (b->wd2>0L && b->cnt<255L) {
      b->buf[b->cnt] = ch;
      ++b->cnt;
      --b->wd2;
   }
} /* end appChar() */

static void appNum(struct Buffer *, X2C_INT32);


static void appNum(struct Buffer * b, X2C_INT32 n)
{
   if (X2C_DIV(n,10L)>0L) appNum(b, X2C_DIV(n,10L));
   b->buf[b->cnt] = (X2C_CHAR)((X2C_CARD32)X2C_MOD(n,10L)+48UL);
   ++b->cnt;
} /* end appNum() */


static void appSet(struct Buffer * b, X2C_CARD32 set)
{
   X2C_INT32 Cou;
   X2C_INT32 i;
   X2C_BOOLEAN Emp;
   Cou = 0L;
   Emp = 1;
   b->cnt = 1L;
   b->ptr = 0L;
   b->buf[0U] = '{';
   i = 0L;
   do {
      if (X2C_IN(i,32,set)) ++Cou;
      else if (Cou>0L) {
         if (!Emp) {
            b->buf[b->cnt] = ',';
            ++b->cnt;
         }
         if (Cou==1L) appNum(b, i-1L);
         else {
            appNum(b, i-Cou);
            b->buf[b->cnt] = '.';
            ++b->cnt;
            b->buf[b->cnt] = '.';
            ++b->cnt;
            appNum(b, i-1L);
         }
         Cou = 0L;
         Emp = 0;
      }
      ++i;
   } while (i<=32L);
   b->buf[b->cnt] = '}';
   ++b->cnt;
} /* end appSet() */


static void append_string(struct Buffer * b, const X2C_CHAR s[],
                X2C_CARD32 s_len)
{
   X2C_INT32 i;
   i = (X2C_INT32)(X2C_LENGTH(s,s_len)-1UL);
   while (b->wd2>0L || i>=0L) {
      --b->ptr;
      if (i<0L) b->buf[b->ptr] = '0';
      else b->buf[b->ptr] = s[i];
      --b->wd2;
      --i;
   }
} /* end append_string() */

#define FormOut_min_int "2147483648"


static void appInt(struct Buffer * b, X2C_INT32 int0)
{
   X2C_INT32 j;
   X2C_BOOLEAN sig;
   sig = int0<0L;
   b->cnt = 256L;
   b->ptr = b->cnt;
   if ((0x80U & b->flags)==0) b->wd2 = 1L;
   if (b->ptr-b->wd2<1L) b->wd2 = b->ptr-1L;
   if (int0==X2C_min_longint) append_string(b, "2147483648", 11ul);
   else {
      if (sig) int0 = labs(int0);
      while (int0 || b->wd2>0L) {
         j = labs(X2C_MOD(int0,10L));
         --b->ptr;
         b->buf[b->ptr] = (X2C_CHAR)(j+48L);
         int0 = X2C_DIV(int0,10L);
         --b->wd2;
      }
   }
   if ((0x4U & b->flags) && (0x40U & b->flags)) {
      j = b->cnt-b->wd1;
      if (j<0L) j = 0L;
      if ((0x12U&b->flags)!=0U || sig) ++j;
      while (j<b->ptr) {
         --b->ptr;
         b->buf[b->ptr] = '0';
      }
   }
   if ((0x10U & b->flags)) {
      --b->ptr;
      if (sig) b->buf[b->ptr] = '-';
      else b->buf[b->ptr] = ' ';
   }
   else if ((0x2U & b->flags)) {
      --b->ptr;
      if (sig) b->buf[b->ptr] = '-';
      else b->buf[b->ptr] = '+';
   }
   else if (sig) {
      --b->ptr;
      b->buf[b->ptr] = '-';
   }
} /* end appInt() */


static void appCar(struct Buffer * b, X2C_CARD32 card, X2C_CARD32 n,
                X2C_CHAR base)
{
   X2C_INT32 j;
   X2C_CHAR dig[17];
   if ((0x20U & b->flags)) strncpy(dig,"0123456789ABCDEF",17u);
   else strncpy(dig,"0123456789abcdef",17u);
   j = 1L;
   b->cnt = 256L;
   b->ptr = b->cnt;
   if ((0x8U & b->flags) && base) {
      --b->ptr;
      b->buf[b->ptr] = base;
   }
   if ((0x80U & b->flags)==0) b->wd2 = 1L;
   if (b->ptr-b->wd2<1L) b->wd2 = b->ptr-1L;
   while (b->wd2>0L || card) {
      --b->ptr;
      b->buf[b->ptr] = dig[card%n];
      card = card/n;
      --b->wd2;
   }
   if ((0x4U & b->flags) && (0x40U & b->flags)) {
      j = b->cnt-b->wd1;
      if (j<0L) j = 0L;
      while (j<b->ptr) {
         --b->ptr;
         b->buf[b->ptr] = '0';
      }
   }
} /* end appCar() */


static void float_sig(const XReal_STR s, struct Buffer * b)
{
   if (s[0UL]=='-') return;
   if ((0x2U & b->flags)) {
      b->buf[0U] = '+';
      b->cnt = 1L;
   }
   else if ((0x10U & b->flags)) {
      b->buf[0U] = ' ';
      b->cnt = 1L;
   }
} /* end float_sig() */


static void appFloatF(struct Buffer * b, X2C_LONGREAL real_val)
{
   XReal_STR s;
   X2C_INT32 i;
   X2C_INT32 place;
   b->ptr = 0L;
   b->cnt = 0L;
   if ((0x80U & b->flags)==0) place = 6L;
   else place = b->wd2;
   XReal_to_fixed(real_val, place, 14L, s);
   float_sig(s, b);
   i = 0L;
   while (s[i]) {
      b->buf[b->cnt] = s[i];
      ++i;
      ++b->cnt;
   }
   if ((0x8U & b->flags)==0) {
      while (b->buf[b->cnt-1L]=='0') --b->cnt;
      if (b->buf[b->cnt-1L]=='.') --b->cnt;
   }
} /* end appFloatF() */


static void appFloatE(struct Buffer * b, X2C_LONGREAL real_val)
{
   XReal_STR s;
   X2C_CHAR exp0;
   X2C_INT32 i;
   b->ptr = 0L;
   b->cnt = 0L;
   if ((0x80U & b->flags)==0) b->wd2 = 6L;
   if ((0x20U & b->flags)) exp0 = 'E';
   else exp0 = 'e';
   XReal_to_float(real_val, b->wd2+1L, 1L, 14L, exp0, (0x8U & b->flags)!=0,
                1, s);
   float_sig(s, b);
   i = 0L;
   while (s[i]) {
      b->buf[b->cnt] = s[i];
      ++i;
      ++b->cnt;
   }
} /* end appFloatE() */


static void appFloatG(struct Buffer * b, X2C_LONGREAL real_val)
{
   X2C_LONGREAL r;
   X2C_INT32 e;
   b->cnt = 0L;
   r = fabs(real_val);
   e = 0L;
   while (r>=10.0 && X2C_DIVL(r,10.0)!=r) {
      ++e;
      r = X2C_DIVL(r,10.0);
   }
   while (r<1.0 && r*10.0!=r) {
      --e;
      r = r*10.0;
   }
   if (e<-4L || e>=b->wd2) {
      --b->wd2;
      appFloatE(b, real_val);
   }
   else {
      b->wd2 = (b->wd2-e)-1L;
      appFloatF(b, real_val);
   }
} /* end appFloatG() */

union Arg;


union Arg {
   X2C_CARD32 card;
   X2C_INT32 int0;
   X2C_ADDRESS adr;
   X2C_CARD32 set;
};

typedef union Arg * Args;

#define FormOut_max 512


static void get_arg(X2C_INT32 * acnt, Args args, union Arg * a)
{
   *a = args[*acnt];
   ++*acnt;
} /* end get_arg() */


static X2C_INT32 get_arg_int(Args args, X2C_INT32 * acnt)
{
   ++*acnt;
   return args[*acnt-1L].int0;
} /* end get_arg_int() */


static X2C_BOOLEAN appStr(X2C_CHAR out[513], X2C_ADDRESS p,
                FormOut_write_proc write0, X2C_INT32 * ocnt, Args args,
                X2C_INT32 arglen, X2C_INT32 * acnt, struct Buffer * b,
                union Arg val)
{
   StrPtr str;
   X2C_INT32 i;
   X2C_INT32 high;
   b->cnt = 0L;
   b->ptr = 0L;
   if (*acnt>=arglen || get_arg_int(args, acnt)) return 1;
   if (*acnt>=arglen) return 1;
   high = get_arg_int(args, acnt);
   if (high<=0L) return 0;
   str = (StrPtr)val.adr;
   if (str==0) return 0;
   if ((0x800U & b->flags)) return 0;
   if ((0x140U&b->flags)==0U) {
      if ((0x80U & b->flags) && b->wd2<=high) {
         if (b->wd2<=0L) return 0;
         high = b->wd2-1L;
      }
      i = 0L;
      for (;;) {
         if (i>high) break;
         if (str[i]==0) {
            if (i==0L) return 0;
            high = i-1L;
            break;
         }
         ++i;
      }
      if (*ocnt>0L) {
         write0(p, out, 513ul, *ocnt);
         *ocnt = 0L;
      }
      write0(p, str, 32768ul, high+1L);
      return 0;
   }
   appString(b, str, high);
   return 0;
} /* end appStr() */


static void Next(X2C_CHAR * ch, X2C_CHAR fmt[], X2C_CARD32 fmt_len,
                X2C_INT32 * fcnt)
{
   if (*fcnt<fmt_len) {
      *ch = fmt[*fcnt];
      ++*fcnt;
   }
   else *ch = 0;
} /* end Next() */


static void width(struct Buffer * b, X2C_INT32 n)
{
   if ((0x100U & b->flags)) b->wd3 = n;
   else if ((0x80U & b->flags)) b->wd2 = n;
   else {
      b->flags |= 0x40U;
      b->wd1 = n;
   }
} /* end width() */


static void scan_format(X2C_INT32 * fcnt, X2C_CHAR fmt[], X2C_CARD32 fmt_len,
                 Args args, X2C_INT32 arglen, X2C_INT32 * acnt,
                X2C_CHAR * ch, struct Buffer * b, X2C_CHAR * base)
{
   X2C_INT32 n;
   X2C_BOOLEAN done;
   b->flags = 0U;
   b->wd1 = -1L;
   b->wd2 = -1L;
   b->wd3 = -1L;
   done = 0;
   do {
      switch ((unsigned)*ch) {
      case '#':
         b->flags |= 0x8U;
         break;
      case '+':
         b->flags |= 0x2U;
         break;
      case '$':
         b->flags |= 0x4U;
         break;
      case ' ':
         b->flags |= 0x10U;
         break;
      case '-':
         b->flags |= 0x1U;
         break;
      case 'L':
         b->flags |= 0x400U;
         break;
      case '|':
         b->flags |= 0x200U;
         break;
      case '~':
         b->flags |= 0x800U;
         break;
      case '.':
         if ((0x80U & b->flags)) b->flags |= 0x100U;
         else b->flags |= 0x80U;
         break;
      case '*':
         if (*acnt<=arglen-1L) width(b, get_arg_int(args, acnt));
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
         n = (X2C_INT32)((X2C_CARD32)(X2C_CARD8)*ch-48UL);
         Next(ch, fmt, fmt_len, fcnt);
         if (((n==0L && '0'<=(X2C_CARD8)*ch) && (X2C_CARD8)*ch<='9')
                && (0x80U & b->flags)==0) b->flags |= 0x4U;
         while ('0'<=(X2C_CARD8)*ch && (X2C_CARD8)*ch<='9') {
            n = (n*10L+(X2C_INT32)(X2C_CARD8)*ch)-48L;
            Next(ch, fmt, fmt_len, fcnt);
         }
         width(b, n);
         --*fcnt;
         break;
      case 'l':
      case 'h':
         break;
      default:;
         *base = *ch;
         done = 1;
         break;
      } /* end switch */
      if (*fcnt<fmt_len) {
         *ch = fmt[*fcnt];
         ++*fcnt;
      }
      else *ch = 0;
   } while (!done);
   if (b->wd1<0L) {
      b->flags &= ~0x40U;
      b->wd1 = 0L;
   }
   if (b->wd2<0L) {
      b->flags &= ~0x80U;
      b->wd2 = 0L;
   }
   if (b->wd3<0L) {
      b->flags &= ~0x100U;
      b->wd3 = 0L;
   }
} /* end scan_format() */


static void write_buf(X2C_CHAR out[513], X2C_ADDRESS p,
                FormOut_write_proc write0, X2C_INT32 * ocnt,
                struct Buffer * b)
{
   while (b->ptr<b->cnt) {
      if (*ocnt>=512L) {
         write0(p, out, 513ul, 512L);
         *ocnt = 0L;
      }
      out[*ocnt] = b->buf[b->ptr];
      ++*ocnt;
      ++b->ptr;
   }
} /* end write_buf() */


static void left_justify(X2C_INT32 * ocnt, FormOut_write_proc write0,
                X2C_ADDRESS p, X2C_CHAR out[513], struct Buffer * b,
                X2C_INT32 spaces)
{
   write_buf(out, p, write0, ocnt, b);
   while (spaces>0L) {
      if (*ocnt>=512L) {
         write0(p, out, 513ul, 512L);
         *ocnt = 0L;
      }
      out[*ocnt] = ' ';
      ++*ocnt;
      --spaces;
   }
} /* end left_justify() */


static void right_justify(X2C_CHAR out[513], X2C_ADDRESS p,
                FormOut_write_proc write0, X2C_INT32 * ocnt,
                struct Buffer * b, X2C_INT32 spaces)
{
   while (spaces>0L) {
      if (*ocnt>=512L) {
         write0(p, out, 513ul, 512L);
         *ocnt = 0L;
      }
      out[*ocnt] = ' ';
      ++*ocnt;
      --spaces;
   }
   write_buf(out, p, write0, ocnt, b);
} /* end right_justify() */


static void center(X2C_CHAR out[513], X2C_ADDRESS p,
                FormOut_write_proc write0, X2C_INT32 * ocnt,
                struct Buffer * b, X2C_INT32 spaces)
{
   X2C_INT32 tail;
   tail = X2C_DIV(spaces,2L);
   spaces -= tail;
   while (spaces>0L) {
      if (*ocnt>=512L) {
         write0(p, out, 513ul, 512L);
         *ocnt = 0L;
      }
      out[*ocnt] = ' ';
      ++*ocnt;
      --spaces;
   }
   write_buf(out, p, write0, ocnt, b);
   while (tail>0L) {
      if (*ocnt>=512L) {
         write0(p, out, 513ul, 512L);
         *ocnt = 0L;
      }
      out[*ocnt] = ' ';
      ++*ocnt;
      --tail;
   }
} /* end center() */


static X2C_BOOLEAN get_real(Args args, X2C_INT32 arglen, X2C_INT32 * acnt,
                X2C_LONGREAL * real_val)
{
   if (*acnt>=arglen) return 1;
   ++*acnt;
   *real_val = *(X2C_LONGREAL *)(X2C_ADDRESS) &args[*acnt-2L];
   return 0;
} /* end get_real() */


static X2C_BOOLEAN SubFormat(X2C_INT32 * ocnt, FormOut_write_proc write0,
                X2C_ADDRESS p, X2C_CHAR out[513], X2C_CHAR * ch,
                X2C_INT32 * acnt, X2C_INT32 arglen, Args args,
                X2C_CHAR fmt[], X2C_CARD32 fmt_len, X2C_INT32 * fcnt)
{
   struct Buffer b;
   X2C_INT32 spaces;
   X2C_LONGREAL real_val;
   X2C_CHAR base;
   union Arg val;
   scan_format(fcnt, fmt, fmt_len, args, arglen, acnt, ch, &b, &base);
   if (*acnt>=arglen) return 1;
   get_arg(acnt, args, &val);
   if ((X2C_CARD8)base>='A' && (X2C_CARD8)base<='Z') b.flags |= 0x20U;
   switch ((unsigned)base) {
   case 'i':
   case 'd':
      if ((0x800U & b.flags)==0) appInt(&b, val.int0);
      break;
   case 'x':
   case 'X':
      if ((0x800U & b.flags)==0) appCar(&b, val.card, 16UL, 'H');
      break;
   case 'o':
      if ((0x800U & b.flags)==0) appCar(&b, val.card, 8UL, 'B');
      break;
   case 'b':
      if ((0x800U & b.flags)==0) appCar(&b, val.card, 2UL, 0);
      break;
   case 'u':
      if ((0x800U & b.flags)==0) appCar(&b, val.card, 10UL, 0);
      break;
   case 'f':
   case 'F':
      if (get_real(args, arglen, acnt, &real_val)) return 1;
      if ((0x800U & b.flags)==0) appFloatF(&b, real_val);
      break;
   case 'e':
   case 'E':
      if (get_real(args, arglen, acnt, &real_val)) return 1;
      if ((0x800U & b.flags)==0) appFloatE(&b, real_val);
      break;
   case 'g':
   case 'G':
      if (get_real(args, arglen, acnt, &real_val)) return 1;
      if ((0x800U & b.flags)==0) appFloatG(&b, real_val);
      break;
   case 's':
      if (appStr(out, p, write0, ocnt, args, arglen, acnt, &b, val)) {
         return 1;
      }
      break;
   case 'c':
      if (255UL<val.card) return 1;
      if ((0x800U & b.flags)==0) appChar(&b, (X2C_CHAR)val.card);
      break;
   case '{':
      if (*ch=='}') {
         Next(ch, fmt, fmt_len, fcnt);
         if ((0x800U & b.flags)==0) appSet(&b, val.set);
      }
      else {
         --*acnt;
         return 1;
      }
      break;
   default:;
      --*acnt;
      return 1;
   } /* end switch */
   if ((0x800U & b.flags)) return 0;
   if ((0x40U & b.flags)) {
      spaces = (b.wd1-b.cnt)+b.ptr;
      if ((0x1U & b.flags)) left_justify(ocnt, write0, p, out, &b, spaces);
      else if ((0x200U & b.flags)) center(out, p, write0, ocnt, &b, spaces);
      else right_justify(out, p, write0, ocnt, &b, spaces);
   }
   else if (b.cnt>b.ptr) write_buf(out, p, write0, ocnt, &b);
   return 0;
} /* end SubFormat() */


static X2C_BOOLEAN Format(X2C_CHAR fmt[], X2C_CARD32 fmt_len, Args args,
                X2C_INT32 arglen, X2C_CHAR * ch, X2C_CHAR out[513],
                X2C_ADDRESS p, FormOut_write_proc write0, X2C_INT32 * ocnt,
                X2C_INT32 * acnt, X2C_INT32 * fcnt)
{
   X2C_INT32 a;
   X2C_INT32 f;
   f = *fcnt;
   a = *acnt;
   if (SubFormat(ocnt, write0, p, out, ch, acnt, arglen, args, fmt, fmt_len,
                fcnt)) {
      *fcnt = f;
      *acnt = a;
      *ch = fmt[*fcnt-1L];
      return 1;
   }
   return 0;
} /* end Format() */


static X2C_CHAR write_NL(X2C_ADDRESS p, FormOut_write_proc write0,
                X2C_INT32 * ocnt, X2C_CHAR out[513], const X2C_CHAR nl[],
                X2C_CARD32 nl_len)
{
   X2C_INT32 i;
   i = 0L;
   while (nl[i+1L]) {
      out[*ocnt] = nl[i];
      ++i;
      ++*ocnt;
      if (*ocnt>=511L) {
         write0(p, out, 513ul, *ocnt);
         *ocnt = 0L;
      }
   }
   return nl[i];
} /* end write_NL() */


extern void FormOut_format(X2C_ADDRESS p, FormOut_write_proc write0,
                X2C_CHAR fmt[], X2C_CARD32 fmt_len, X2C_CHAR linesep,
                X2C_ADDRESS xxx, X2C_CARD32 size)
{
   Args args;
   X2C_INT32 arglen;
   X2C_INT32 acnt;
   X2C_CHAR out[513];
   X2C_INT32 fcnt;
   X2C_INT32 ocnt;
   X2C_CHAR ch;
   args = (Args)xxx;
   arglen = (X2C_INT32)(size/sizeof(union Arg));
   ch = fmt[0UL];
   fcnt = 1L;
   ocnt = 0L;
   acnt = 0L;
   for (;;) {
      switch ((unsigned)ch) {
      case '\\':
         if (fcnt<fmt_len) {
            ch = fmt[fcnt];
            ++fcnt;
         }
         else ch = 0;
         if (ocnt>=511L) {
            write0(p, out, 513ul, ocnt);
            ocnt = 0L;
         }
         if (ch=='n') {
            if (linesep=='0') {
               ch = write_NL(p, write0, &ocnt, out, defaultNL, 4ul);
            }
            else if (linesep=='2') {
               ch = write_NL(p, write0, &ocnt, out, defaultTL, 4ul);
            }
            else if (linesep=='1') {
               out[ocnt] = '\015';
               ++ocnt;
               ch = '\012';
            }
            else ch = linesep;
         }
         else if (ch=='r') ch = '\015';
         else if (ch=='l') ch = '\012';
         else if (ch=='f') ch = '\014';
         else if (ch=='t') ch = '\011';
         else if (ch=='e') ch = '\033';
         else if (ch=='g') ch = '\007';
         else if (ch=='\\') ch = '\\';
         else {
            if (ch==0) {
               out[ocnt] = '\\';
               ++ocnt;
               goto loop_exit;
            }
            out[ocnt] = '\\';
            ++ocnt;
         }
         out[ocnt] = ch;
         ++ocnt;
         Next(&ch, fmt, fmt_len, &fcnt);
         break;
      case '%':
         if (fcnt<fmt_len) {
            ch = fmt[fcnt];
            ++fcnt;
         }
         else ch = 0;
         if (ocnt>=512L) {
            write0(p, out, 513ul, 512L);
            ocnt = 0L;
         }
         if (ch==0) {
            out[ocnt] = '%';
            ++ocnt;
            goto loop_exit;
         }
         if (ch=='%') {
            out[ocnt] = '%';
            ++ocnt;
            Next(&ch, fmt, fmt_len, &fcnt);
         }
         else if (Format(fmt, fmt_len, args, arglen, &ch, out, p, write0,
                &ocnt, &acnt, &fcnt)) {
            out[ocnt] = '%';
            ++ocnt;
         }
         break;
      case 0:
         goto loop_exit;
      default:;
         if (ocnt>=512L) {
            write0(p, out, 513ul, 512L);
            ocnt = 0L;
         }
         out[ocnt] = ch;
         ++ocnt;
         if (fcnt>=fmt_len) goto loop_exit;
         ch = fmt[fcnt];
         ++fcnt;
         break;
      } /* end switch */
   }
   loop_exit:;
   if (ocnt>0L) write0(p, out, 513ul, ocnt);
} /* end format() */


extern void FormOut_LineSeparator(X2C_CHAR nl[], X2C_CARD32 nl_len)
{
   X2C_COPY(nl,nl_len,defaultNL,4u);
} /* end LineSeparator() */


extern void FormOut_TextSeparator(X2C_CHAR nl[], X2C_CARD32 nl_len)
{
   X2C_COPY(nl,nl_len,defaultTL,4u);
} /* end TextSeparator() */


extern void FormOut_BEGIN(void)
{
   static int FormOut_init = 0;
   if (FormOut_init) return;
   FormOut_init = 1;
   if (sizeof(LineSep)!=4) X2C_ASSERT(0);
   Strings_BEGIN();
   XReal_BEGIN();
   defaultNL[0U] = '\012';
   defaultNL[1U] = 0;
   memcpy(defaultTL,defaultNL,4u);
}

