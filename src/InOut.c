/* XDS v2.51: Copyright (c) 1999-2003 Excelsior, LLC. All Rights Reserved. */
/* "@(#)InOut.c May 10 15:23:34 2005" */
/* Generated by XDS Modula-2 to ANSI C v4.20 translator */

#define X2C_int32
#define X2C_index32
#ifndef InOut_H_
#include "InOut.h"
#endif
#define InOut_C_
#ifndef STextIO_H_
#include "STextIO.h"
#endif
#ifndef SWholeIO_H_
#include "SWholeIO.h"
#endif
#ifndef WholeStr_H_
#include "WholeStr.h"
#endif
#ifndef ConvTypes_H_
#include "ConvTypes.h"
#endif
#ifndef SIOResult_H_
#include "SIOResult.h"
#endif
#ifndef IOConsts_H_
#include "IOConsts.h"
#endif
#ifndef StdChans_H_
#include "StdChans.h"
#endif
#ifndef IOChan_H_
#include "IOChan.h"
#endif
#ifndef ChanConsts_H_
#include "ChanConsts.h"
#endif
#ifndef StreamFile_H_
#include "StreamFile.h"
#endif
#ifndef Strings_H_
#include "Strings.h"
#endif

X2C_BOOLEAN InOut_Done;
X2C_CHAR InOut_termCH;
static X2C_CARD8 res;

static IOChan_ChanId inp0;

static IOChan_ChanId out;


static void Open(X2C_CHAR prompt[], X2C_CARD32 prompt_len, X2C_CHAR defext[],
                 X2C_CARD32 defext_len, X2C_CARD8 flags,
                IOChan_ChanId * cid)
{
   X2C_CHAR name[128];
   X2C_CARD8 ores;
   X2C_CARD32 i;
   X2C_PCOPY((void **)&prompt,prompt_len);
   X2C_PCOPY((void **)&defext,defext_len);
   InOut_Done = 0;
   if (StdChans_InChan()==StdChans_StdInChan()) {
      STextIO_WriteString(prompt, prompt_len);
      InOut_ReadString(name, 128ul);
      STextIO_WriteLn();
      i = X2C_LENGTH(name,128ul);
      if (i>0ul && name[i-1ul]=='.') {
         if (!Strings_CanAppendAll(X2C_LENGTH(defext,defext_len), name,
                128ul)) goto label;
         Strings_Append(defext, defext_len, name, 128ul);
      }
      StreamFile_Open(cid, name, 128ul, flags, &ores);
      InOut_Done = ores==ChanConsts_opened;
   }
   label:;
   X2C_PFREE(prompt);
   X2C_PFREE(defext);
} /* end Open() */


extern void InOut_OpenInput(X2C_CHAR defext[], X2C_CARD32 defext_len)
{
   IOChan_ChanId i;
   X2C_PCOPY((void **)&defext,defext_len);
   Open("Input> ", 8ul, defext, defext_len, 0x1u, &i);
   if (InOut_Done) {
      inp0 = i;
      StdChans_SetInChan(inp0);
   }
   X2C_PFREE(defext);
} /* end OpenInput() */


extern void InOut_OpenOutput(X2C_CHAR defext[], X2C_CARD32 defext_len)
{
   IOChan_ChanId o;
   X2C_PCOPY((void **)&defext,defext_len);
   Open("Output> ", 9ul, defext, defext_len, 0x6u, &o);
   if (InOut_Done) {
      out = o;
      StdChans_SetOutChan(out);
   }
   X2C_PFREE(defext);
} /* end OpenOutput() */


extern void InOut_CloseInput(void)
{
   InOut_Done = 0;
   if (inp0!=StdChans_StdInChan()) {
      if (inp0==StdChans_InChan()) StdChans_SetInChan(StdChans_StdInChan());
      StreamFile_Close(&inp0);
      inp0 = StdChans_StdInChan();
      InOut_Done = 1;
   }
} /* end CloseInput() */


extern void InOut_CloseOutput(void)
{
   InOut_Done = 0;
   if (out!=StdChans_StdOutChan()) {
      if (out==StdChans_OutChan()) {
         StdChans_SetOutChan(StdChans_StdOutChan());
      }
      StreamFile_Close(&out);
      out = StdChans_StdOutChan();
      InOut_Done = 1;
   }
} /* end CloseOutput() */


extern void InOut_Read(X2C_CHAR * v)
{
   STextIO_ReadChar(v);
   res = SIOResult_ReadResult();
   InOut_Done = res!=IOConsts_endOfInput;
   if (InOut_Done) {
      if (res==IOConsts_endOfLine) {
         STextIO_SkipLine();
         *v = '\012';
      }
   }
   else *v = 0;
} /* end Read() */


extern void InOut_ReadString(X2C_CHAR s[], X2C_CARD32 s_len)
{
   X2C_CHAR c;
   X2C_CARD32 i;
   InOut_Read(&c);
   while (InOut_Done && c==' ') InOut_Read(&c);
   i = 0ul;
   while (InOut_Done && (X2C_CARD8)c>' ') {
      if (i<s_len-1) {
         s[i] = c;
         ++i;
      }
      InOut_Read(&c);
   }
   InOut_termCH = c;
   s[i] = 0;
   InOut_Done = i>0ul;
} /* end ReadString() */


extern void InOut_ReadCard(X2C_CARD32 * v)
{
   X2C_CHAR s[64];
   X2C_CARD8 r;
   InOut_ReadString(s, 64ul);
   InOut_Done = SIOResult_ReadResult()==IOConsts_allRight;
   if (InOut_Done) {
      WholeStr_StrToCard(s, 64ul, v, &r);
      InOut_Done = r==ConvTypes_strAllRight;
   }
} /* end ReadCard() */


extern void InOut_ReadInt(X2C_INT32 * v)
{
   X2C_CHAR s[64];
   X2C_CARD8 r;
   InOut_ReadString(s, 64ul);
   InOut_Done = SIOResult_ReadResult()==IOConsts_allRight;
   if (InOut_Done) {
      WholeStr_StrToInt(s, 64ul, v, &r);
      InOut_Done = r==ConvTypes_strAllRight;
   }
} /* end ReadInt() */


extern void InOut_Write(X2C_CHAR c)
{
   STextIO_WriteChar(c);
} /* end Write() */


extern void InOut_WriteLn(void)
{
   STextIO_WriteLn();
} /* end WriteLn() */


extern void InOut_WriteString(X2C_CHAR s[], X2C_CARD32 s_len)
{
   STextIO_WriteString(s, s_len);
} /* end WriteString() */


extern void InOut_WriteCard(X2C_CARD32 v, X2C_CARD32 w)
{
   SWholeIO_WriteCard(v, w);
} /* end WriteCard() */


extern void InOut_WriteInt(X2C_INT32 v, X2C_CARD32 w)
{
   SWholeIO_WriteInt(v, w);
} /* end WriteInt() */


static void WriteBase(X2C_CARD32 v, X2C_CARD32 w, X2C_CARD32 base)
{
   X2C_CARD32 d;
   X2C_CARD32 i;
   X2C_CHAR a[11];
   i = 0ul;
   do {
      d = v%base;
      if (d<10ul) a[i] = (X2C_CHAR)(d+48ul);
      else a[i] = (X2C_CHAR)((d+65ul)-10ul);
      v = v/base;
      ++i;
   } while (v);
   while (w>i) {
      STextIO_WriteChar(' ');
      --w;
   }
   do {
      --i;
      STextIO_WriteChar(a[i]);
   } while (i);
} /* end WriteBase() */


extern void InOut_WriteOct(X2C_CARD32 v, X2C_CARD32 w)
{
   WriteBase(v, w, 8ul);
} /* end WriteOct() */


extern void InOut_WriteHex(X2C_CARD32 v, X2C_CARD32 w)
{
   WriteBase(v, w, 16ul);
} /* end WriteHex() */


extern void InOut_BEGIN(void)
{
   static int InOut_init = 0;
   if (InOut_init) return;
   InOut_init = 1;
   Strings_BEGIN();
   StreamFile_BEGIN();
   StdChans_BEGIN();
   SIOResult_BEGIN();
   WholeStr_BEGIN();
   SWholeIO_BEGIN();
   STextIO_BEGIN();
   inp0 = StdChans_StdInChan();
   out = StdChans_StdOutChan();
}

