/* XDS v2.60: Copyright (c) 1999-2011 Excelsior, LLC. All Rights Reserved. */
/* "@(#)SRawIO.c Feb  3 14:31:18 2012" */
/* Generated by XDS Modula-2 to ANSI C v4.20 translator */

#define X2C_int32
#define X2C_index32
#ifndef SRawIO_H_
#include "SRawIO.h"
#endif
#define SRawIO_C_
#ifndef IOChan_H_
#include "IOChan.h"
#endif
#ifndef StdChans_H_
#include "StdChans.h"
#endif
#ifndef RawIO_H_
#include "RawIO.h"
#endif


extern void SRawIO_Read(X2C_LOC to[], X2C_CARD32 to_len)
{
   RawIO_Read(StdChans_InChan(), to, to_len);
} /* end Read() */


extern void SRawIO_Write(X2C_LOC from[], X2C_CARD32 from_len)
{
   X2C_PCOPY((void **)&from,from_len);
   RawIO_Write(StdChans_OutChan(), from, from_len);
   X2C_PFREE(from);
} /* end Write() */


extern void SRawIO_BEGIN(void)
{
   static int SRawIO_init = 0;
   if (SRawIO_init) return;
   SRawIO_init = 1;
   RawIO_BEGIN();
   StdChans_BEGIN();
   IOChan_BEGIN();
}

