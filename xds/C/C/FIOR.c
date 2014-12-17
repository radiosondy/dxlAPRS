/* XDS v2.60: Copyright (c) 1999-2011 Excelsior, LLC. All Rights Reserved. */
/* "@(#)FIOR.c Feb  3 14:31:28 2012" */
/* Generated by XDS Modula-2 to ANSI C v4.20 translator */

#define X2C_int32
#define X2C_index32
#ifndef FIOR_H_
#include "FIOR.h"
#endif
#define FIOR_C_
#ifndef FileName_H_
#include "FileName.h"
#endif
#ifndef FileSys_H_
#include "FileSys.h"
#endif
#ifndef FIO_H_
#include "FIO.h"
#endif
#ifndef IOChan_H_
#include "IOChan.h"
#endif
#ifndef Str_H_
#include "Str.h"
#endif
#ifndef platform_H_
#include "platform.h"
#endif


static X2C_BOOLEAN HasExt(const X2C_CHAR s[], X2C_CARD32 s_len,
                X2C_CARD32 * pos)
{
   X2C_CARD32 i;
   i = X2C_LENGTH(s,s_len);
   for (;;) {
      if (i==0UL) return 0;
      --i;
      if (s[i]==extSep) {
         *pos = i;
         return 1;
      }
      if (s[i]==pathSep || s[i]==drvSep) return 0;
   }
   return 0;
} /* end HasExt() */


extern void FIOR_AddExtension(X2C_CHAR s[], X2C_CARD32 s_len, X2C_CHAR ext[],
                 X2C_CARD32 ext_len)
{
   X2C_CARD32 j;
   X2C_CARD32 i;
   if (HasExt(s, s_len, &i)) return;
   i = X2C_LENGTH(s,s_len);
   j = 0UL;
   if (i<=s_len-1) {
      s[i] = '.';
      ++i;
   }
   while ((i<=s_len-1 && j<=ext_len-1) && ext[j]) {
      s[i] = ext[j];
      ++i;
      ++j;
   }
   if (i<=s_len-1) s[i] = 0;
} /* end AddExtension() */


extern void FIOR_ChangeExtension(X2C_CHAR s[], X2C_CARD32 s_len,
                X2C_CHAR ext[], X2C_CARD32 ext_len)
{
   FIOR_RemoveExtension(s, s_len);
   FIOR_AddExtension(s, s_len, ext, ext_len);
} /* end ChangeExtension() */


extern void FIOR_RemoveExtension(X2C_CHAR s[], X2C_CARD32 s_len)
{
   X2C_CARD32 i;
   if (HasExt(s, s_len, &i)) s[i] = 0;
} /* end RemoveExtension() */


extern X2C_BOOLEAN FIOR_IsExtension(X2C_CHAR s[], X2C_CARD32 s_len,
                X2C_CHAR ext[], X2C_CARD32 ext_len)
{
   X2C_CARD32 j;
   X2C_CARD32 i;
   if (!HasExt(s, s_len, &i)) return ext[0UL]==0;
   ++i;
   j = 0UL;
   for (;;) {
      if (i>s_len-1) return j>ext_len-1 || ext[j]==0;
      if (j>ext_len-1) return s[i]==0;
      if (pl_unix) {
         if (s[i]!=ext[j]) return 0;
      }
      else if (pl_msdos) {
         if (X2C_CAP(s[i])!=X2C_CAP(ext[j])) return 0;
      }
      ++i;
      ++j;
   }
   return 0;
} /* end IsExtension() */


extern void FIOR_ExpandPath(X2C_CHAR path[], X2C_CARD32 path_len,
                X2C_CHAR fullpath[], X2C_CARD32 fullpath_len)
{
   X2C_CHAR temp[1024];
   X2C_CARD8 drive;
   X2C_CARD32 j;
   X2C_CARD32 i;
   if (pl_unix) i = 0UL;
   else if (pl_msdos) {
      if (path[1UL]!=drvSep) {
         drive = FIO_GetDrive();
         i = 0UL;
      }
      else {
         drive = (X2C_CARD8)(((X2C_CARD32)(X2C_CARD8)X2C_CAP(path[0UL])-65UL)
                +1UL);
         i = 2UL;
      }
   }
   if (path[i]==pathSep) X2C_COPY(path,path_len,temp,1024u);
   else if (pl_unix) FileSys_GetCDName(temp, 1024ul);
   else if (pl_msdos) FIO_GetDir(drive, temp, 1024ul);
   if (pl_msdos && i==0UL) {
      Str_Insert(temp, 1024ul, "  ", 3ul, 0UL);
      temp[0U] = (65U+drive)-1U;
      temp[1U] = drvSep;
   }
   i = X2C_LENGTH(temp,1024ul);
   j = 0UL;
   if (path[0UL]!=pathSep && i<=1023UL) {
      temp[i] = pathSep;
      ++i;
   }
   while ((i<=1023UL && j<=path_len-1) && path[j]) {
      temp[i] = path[j];
      ++i;
      ++j;
   }
   if (i<=1023UL) temp[i] = 0;
   X2C_COPY(temp,1024u,fullpath,fullpath_len);
} /* end ExpandPath() */


extern void FIOR_MakePath(X2C_CHAR path[], X2C_CARD32 path_len,
                X2C_CHAR head[], X2C_CARD32 head_len, X2C_CHAR tail[],
                X2C_CARD32 tail_len)
{
   X2C_CARD32 j;
   X2C_CARD32 i;
   FIOR_ExpandPath(head, head_len, path, path_len);
   i = X2C_LENGTH(path,path_len);
   j = 0UL;
   if (i>0UL && path[i-1UL]!=pathSep) {
      if (tail[0UL]!=pathSep && i<=path_len-1) {
         path[i] = pathSep;
         ++i;
      }
   }
   while ((i<=path_len-1 && j<=tail_len-1) && tail[j]) {
      path[i] = tail[j];
      ++i;
      ++j;
   }
   if (i<=path_len-1) path[i] = 0;
} /* end MakePath() */


extern void FIOR_SplitPath(X2C_CHAR path[], X2C_CARD32 path_len,
                X2C_CHAR head[], X2C_CARD32 head_len, X2C_CHAR tail[],
                X2C_CARD32 tail_len)
{
   struct FileName_Format f;
   X2C_CARD32 i;
   FileName_GetFormat(path, path_len, &f);
   i = 0UL;
   while (i<(f.dirPos+f.dirLen)-1UL && i<head_len-1) {
      head[i] = path[f.dirPos+i];
      ++i;
   }
   head[i] = 0;
   i = 0UL;
   while (i<(f.extPos+f.extLen)-1UL && i<tail_len-1) {
      tail[i] = path[f.namePos+i];
      ++i;
   }
   tail[i] = 0;
} /* end SplitPath() */


extern void FIOR_BEGIN(void)
{
   static int FIOR_init = 0;
   if (FIOR_init) return;
   FIOR_init = 1;
   Str_BEGIN();
   IOChan_BEGIN();
   FIO_BEGIN();
   FileSys_BEGIN();
   FileName_BEGIN();
}

