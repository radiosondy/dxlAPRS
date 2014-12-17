/* XDS v2.60: Copyright (c) 1999-2011 Excelsior, LLC. All Rights Reserved. */
/* "@(#)StreamFile.c Feb  3 14:31:18 2012" */
/* Generated by XDS Modula-2 to ANSI C v4.20 translator */

#define X2C_int32
#define X2C_index32
#ifndef StreamFile_H_
#include "StreamFile.h"
#endif
#define StreamFile_C_
#ifndef IOChan_H_
#include "IOChan.h"
#endif
#ifndef ChanConsts_H_
#include "ChanConsts.h"
#endif
#ifndef IOLink_H_
#include "IOLink.h"
#endif
#ifndef xDevData_H_
#include "xDevData.h"
#endif
#ifndef xlibOS_H_
#include "xlibOS.h"
#endif
#ifndef xmRTS_H_
#include "xmRTS.h"
#endif

X2C_CARD8 StreamFile_read = 0x1U;
X2C_CARD8 StreamFile_write = 0x2U;
X2C_CARD8 StreamFile_old = 0x4U;
X2C_CARD8 StreamFile_text = 0x8U;
X2C_CARD8 StreamFile_raw = 0x10U;
static IOLink_DeviceId did;

static X2C_CARD8 StreamFile_rw = 0x3U;


extern void StreamFile_Open(IOChan_ChanId * cid, X2C_CHAR name[],
                X2C_CARD32 name_len, X2C_CARD8 flags, X2C_CARD8 * res)
{
   X2C_OSFHANDLE f;
   X2C_CARD32 tags;
   xDevData_FileName fn;
   X2C_BOOLEAN exi;
   X2C_PCOPY((void **)&name,name_len);
   if ((0x2U & flags)==0) flags |= 0x1U;
   if ((0x1U & flags)) flags |= 0x4U;
   if ((0x10U & flags)==0) flags |= 0x8U;
   xDevData_MakeName(&fn, name, name_len, res);
   if (*res) goto label;
   IOLink_MakeChan(did, cid);
   if (*cid==IOChan_InvalidChan()) {
      xDevData_UnMakeName(&fn);
      *res = ChanConsts_outOfChans;
      goto label;
   }
   if ((flags&0x3U)==0x3U) tags = 0x3UL;
   else if ((0x1U & flags)) tags = 0x1UL;
   else {
      exi = X2C_Exists((X2C_pCHAR)fn->Adr);
      if (exi) {
         if ((0x4U & flags)==0) {
            *res = ChanConsts_fileExists;
            xDevData_UnMakeName(&fn);
            IOLink_UnMakeChan(did, cid);
            goto label;
         }
         tags = 0x2UL;
      }
      else tags = 0x6UL;
   }
   if ((0x8U & flags)) tags = tags|0x8UL;
   if ((0x10U & flags)) tags = tags|0x10UL;
   *res = X2C_fOpen(&f, fn->Adr, tags);
   if (*res) {
      xDevData_UnMakeName(&fn);
      IOLink_UnMakeChan(did, cid);
      goto label;
   }
   xDevData_Open(IOLink_DeviceTablePtrValue(*cid, did, IOChan_notAvailable, "\
", 1ul), f, fn, flags, xDevData_bmDefault, res);
   if (*res) {
      X2C_fClose(&f);
      xDevData_UnMakeName(&fn);
      IOLink_UnMakeChan(did, cid);
   }
   label:;
   X2C_PFREE(name);
} /* end Open() */


extern X2C_BOOLEAN StreamFile_IsStreamFile(IOChan_ChanId cid)
{
   return IOLink_IsDevice(cid, did);
} /* end IsStreamFile() */


extern void StreamFile_Close(IOChan_ChanId * cid)
{
   IOLink_DeviceTablePtr x;
   xDevData_DevData f;
   X2C_OSFHANDLE cf;
   x = IOLink_DeviceTablePtrValue(*cid, did, IOChan_notAvailable, "", 1ul);
   f = xDevData_GetDevData(x);
   cf = f->cf;
   xDevData_Close(x);
   X2C_fClose(&cf);
   IOLink_UnMakeChan(did, cid);
} /* end Close() */


extern void StreamFile_BEGIN(void)
{
   static int StreamFile_init = 0;
   if (StreamFile_init) return;
   StreamFile_init = 1;
   xDevData_BEGIN();
   IOLink_BEGIN();
   IOChan_BEGIN();
   ChanConsts_BEGIN();
   xDevData_GetDID(&did);
}

