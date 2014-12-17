/* XDS v2.60: Copyright (c) 1999-2011 Excelsior, LLC. All Rights Reserved. */
/* "@(#)TermFile.c Feb  3 14:31:18 2012" */
/* Generated by XDS Modula-2 to ANSI C v4.20 translator */

#define X2C_int32
#define X2C_index32
#ifndef TermFile_H_
#include "TermFile.h"
#endif
#define TermFile_C_
#ifndef IOChan_H_
#include "IOChan.h"
#endif
#ifndef ChanConsts_H_
#include "ChanConsts.h"
#endif
#ifndef IOConsts_H_
#include "IOConsts.h"
#endif
#ifndef xmRTS_H_
#include "xmRTS.h"
#endif
#ifndef xlibOS_H_
#include "xlibOS.h"
#endif
#ifndef xrtsOS_H_
#include "xrtsOS.h"
#endif
#ifndef IOLink_H_
#include "IOLink.h"
#endif
#ifndef platform_H_
#include "platform.h"
#endif

X2C_CARD8 TermFile_read = 0x1U;
X2C_CARD8 TermFile_write = 0x2U;
X2C_CARD8 TermFile_text = 0x8U;
X2C_CARD8 TermFile_raw = 0x10U;
X2C_CARD8 TermFile_echo = 0x40U;
#define TermFile_LF "\012"

#define TermFile_SEP0 "\012"

#define TermFile_SEP1 ""

#define TermFile_BUF_HIGH 255

#define TermFile_BUF_LOW (-8)

#define TermFile_echoFlag ChanConsts_echoFlag

#define TermFile_textFlag ChanConsts_textFlag

static X2C_CHAR buf[264];

static X2C_INT32 blen;

static X2C_INT32 bpos;

static X2C_INT32 epos;

static X2C_BOOLEAN flin;

static X2C_BOOLEAN init;


static void lock0(void)
{
} /* end lock() */


static void unlock0(void)
{
} /* end unlock() */


static void shiftBuf(void)
{
   X2C_INT32 j;
   X2C_INT32 i;
   i = blen;
   j = -1L;
   while (i>=0L && j>=-8L) {
      buf[j+8] = buf[i+8];
      --i;
      --j;
   }
   bpos = 0L;
   epos = -1L;
} /* end shiftBuf() */


static X2C_INT32 next(X2C_BOOLEAN echo)
{
   X2C_INT32 res;
   X2C_CARD32 rd;
   if (bpos>=blen) {
      shiftBuf();
      if (!echo || flin) {
         res = X2C_ttyReadLE((X2C_ADDRESS) &buf[8], 255UL, &rd);
      }
      else res = X2C_ttyReadNE((X2C_ADDRESS) &buf[8], 255UL, &rd);
      if (res) return res;
      blen = (X2C_INT32)rd-1L;
      if (!echo) {
         flin = 1;
         epos = blen;
      }
   }
   else ++bpos;
   return 0L;
} /* end next() */


static X2C_INT32 getc0(IOLink_DeviceTablePtr x, X2C_CHAR * ch)
{
   X2C_INT32 res;
   res = next((0x40U & x->flags)!=0);
   if (res) return res;
   *ch = buf[bpos+8];
   return 0L;
} /* end getc() */


static void ungetc0(IOLink_DeviceTablePtr x, X2C_CHAR ch)
{
   buf[bpos+8] = ch;
   --bpos;
} /* end ungetc() */


static X2C_INT32 doecho(X2C_BOOLEAN do0)
{
   X2C_CHAR ch;
   X2C_INT32 res;
   while (epos<bpos) {
      ++epos;
      if (do0) {
         ch = buf[epos+8];
         res = X2C_ttyWrite((X2C_ADDRESS) &ch, 1UL);
         if (res) return res;
      }
   }
   return 0L;
} /* end doecho() */


static void HardError(IOLink_DeviceTablePtr x, X2C_INT32 res)
{
   X2C_CHAR num[12];
   X2C_CARD32 pos;
   pos = 0UL;
   X2C_DecToStr(num, &pos, (X2C_CARD32)res);
   num[pos] = 0;
   IOLink_RAISEdevException(x->cid, x->did, IOChan_hardDeviceError, num,
                12ul);
} /* end HardError() */


static void SoftError(IOLink_DeviceTablePtr x, X2C_INT32 res)
{
   X2C_CHAR num[12];
   X2C_CARD32 pos;
   pos = 0UL;
   X2C_DecToStr(num, &pos, (X2C_CARD32)res);
   num[pos] = 0;
   IOLink_RAISEdevException(x->cid, x->did, IOChan_softDeviceError, num,
                12ul);
} /* end SoftError() */

static void DoWrite(IOLink_DeviceTablePtr, X2C_ADDRESS, X2C_CARD32);


static void DoWrite(IOLink_DeviceTablePtr x, X2C_ADDRESS a, X2C_CARD32 n)
{
   X2C_INT32 res;
   if (n==0UL) return;
   res = X2C_ttyWrite(a, n);
   if (res) HardError(x, res);
} /* end DoWrite() */

static void WriteLn(IOLink_DeviceTablePtr);


static void WriteLn(IOLink_DeviceTablePtr x)
{
   X2C_CHAR ch;
   ch = '\012';
   DoWrite(x, (X2C_ADDRESS) &ch, 1UL);
} /* end WriteLn() */

static void Look(IOLink_DeviceTablePtr, X2C_CHAR *, X2C_CARD8 *);


static void Look(IOLink_DeviceTablePtr x, X2C_CHAR * ch, X2C_CARD8 * res)
{
   X2C_INT32 r;
   X2C_CHAR cc;
   lock0();
   r = getc0(x, &cc);
   if (r==0L) {
      if (cc=='\012') *res = IOConsts_endOfLine;
      else {
         *ch = cc;
         *res = IOConsts_allRight;
      }
      ungetc0(x, cc);
   }
   else *res = IOConsts_endOfInput;
   unlock0();
   x->result = *res;
} /* end Look() */

static void Skip(IOLink_DeviceTablePtr);


static void Skip(IOLink_DeviceTablePtr x)
{
   X2C_INT32 res;
   X2C_CHAR ch;
   lock0();
   res = getc0(x, &ch);
   if (res) {
      unlock0();
      SoftError(x, res);
   }
   if (ch=='\012') flin = 0;
   res = doecho(0);
   unlock0();
   if (res) SoftError(x, res);
   x->result = IOConsts_allRight;
} /* end Skip() */

static void SkipLook(IOLink_DeviceTablePtr, X2C_CHAR *, X2C_CARD8 *);


static void SkipLook(IOLink_DeviceTablePtr x, X2C_CHAR * ch,
                X2C_CARD8 * res)
{
   Skip(x);
   Look(x, ch, res);
} /* end SkipLook() */

static void TextRead(IOLink_DeviceTablePtr, X2C_ADDRESS, X2C_CARD32,
                X2C_CARD32 *);


static void TextRead(IOLink_DeviceTablePtr x, X2C_ADDRESS a, X2C_CARD32 n,
                X2C_CARD32 * locs)
{
   X2C_INT32 res;
   X2C_CHAR ch;
   lock0();
   *locs = 0UL;
   while (*locs<n) {
      res = getc0(x, &ch);
      if (res) {
         unlock0();
         SoftError(x, res);
      }
      if (ch=='\012') {
         ungetc0(x, ch);
         if (*locs>0UL) x->result = IOConsts_allRight;
         else x->result = IOConsts_endOfLine;
         unlock0();
         return;
      }
      res = doecho(1);
      if (res) {
         unlock0();
         SoftError(x, res);
      }
      *(X2C_CHAR *)a = ch;
      a = (X2C_ADDRESS)((X2C_ADDRESS)a+(X2C_INT32)1UL);
      ++*locs;
   }
   unlock0();
   x->result = IOConsts_allRight;
} /* end TextRead() */

static void RawRead(IOLink_DeviceTablePtr, X2C_ADDRESS, X2C_CARD32,
                X2C_CARD32 *);


static void RawRead(IOLink_DeviceTablePtr x, X2C_ADDRESS a, X2C_CARD32 n,
                X2C_CARD32 * locs)
{
   X2C_INT32 res;
   X2C_CARD32 c;
   X2C_CHAR pch;
   X2C_CHAR ch;
   x->result = IOConsts_allRight;
   *locs = 0UL;
   if (n==0UL) return;
   lock0();
   *locs = 0UL;
   c = 0UL;
   pch = 0;
   while (*locs<n) {
      res = getc0(x, &ch);
      if (res==0L) res = doecho(1);
      if (res) {
         unlock0();
         SoftError(x, res);
      }
      if (((0x8U & x->flags) && ch=='\012' || ch=='\012') || ch==0 && 0) {
         flin = 0;
      }
      *(X2C_CHAR *)a = ch;
      a = (X2C_ADDRESS)((X2C_ADDRESS)a+(X2C_INT32)1UL);
      ++*locs;
   }
   unlock0();
} /* end RawRead() */


static void IniRead(IOLink_DeviceTablePtr x)
{
   if ((0x10U & x->flags)) x->doRawRead = RawRead;
   if ((0x8U & x->flags)) {
      x->doLook = Look;
      x->doSkip = Skip;
      x->doSkipLook = SkipLook;
      x->doTextRead = TextRead;
   }
} /* end IniRead() */


static void IniWrite(IOLink_DeviceTablePtr x)
{
   if ((0x10U & x->flags)) x->doRawWrite = DoWrite;
   if ((0x8U & x->flags)) {
      x->doTextWrite = DoWrite;
      x->doLnWrite = WriteLn;
   }
} /* end IniWrite() */


static X2C_INT32 initDevice(void)
{
   X2C_INT32 res;
   res = X2C_InitTTY();
   if (res) return res;
   bpos = 0L;
   blen = 0L;
   epos = 0L;
   flin = 0;
   init = 1;
   return 0L;
} /* end initDevice() */

static IOLink_DeviceId did;


extern void TermFile_Open(IOChan_ChanId * cid, X2C_CARD8 flags,
                X2C_CARD8 * res)
{
   IOLink_DeviceTablePtr x;
   lock0();
   if (!init) {
      if (initDevice()) {
         *res = ChanConsts_otherProblem;
         unlock0();
         return;
      }
   }
   if ((0x10U & flags)==0) flags |= 0x8U;
   IOLink_MakeChan(did, cid);
   if (*cid==IOChan_InvalidChan()) {
      *res = ChanConsts_outOfChans;
      unlock0();
      return;
   }
   x = IOLink_DeviceTablePtrValue(*cid, did, IOChan_notAvailable, "", 1ul);
   x->flags = flags;
   if ((0x1U & flags)) IniRead(x);
   if ((0x2U & flags)) IniWrite(x);
   unlock0();
   *res = ChanConsts_opened;
} /* end Open() */


extern X2C_BOOLEAN TermFile_IsTermFile(IOChan_ChanId cid)
{
   return IOLink_IsDevice(cid, did);
} /* end IsTermFile() */


extern void TermFile_Close(IOChan_ChanId * cid)
{
   IOLink_UnMakeChan(did, cid);
} /* end Close() */


extern void TermFile_BEGIN(void)
{
   static int TermFile_init = 0;
   if (TermFile_init) return;
   TermFile_init = 1;
   IOLink_BEGIN();
   IOConsts_BEGIN();
   IOChan_BEGIN();
   ChanConsts_BEGIN();
   IOLink_AllocateDeviceId(&did);
   init = 0;
}

