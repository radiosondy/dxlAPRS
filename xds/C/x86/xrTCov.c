/* XDS v2.60: Copyright (c) 1999-2011 Excelsior, LLC. All Rights Reserved. */
/* "@(#)xrTCov.c Feb  3 14:30:28 2012" */
/* Generated by XDS Modula-2 to ANSI C v4.20 translator */

#define X2C_int32
#define X2C_index32
#ifndef xrTCov_H_
#include "xrTCov.h"
#endif
#define xrTCov_C_
#ifndef ProgEnv_H_
#include "ProgEnv.h"
#endif
#ifndef xmRTS_H_
#include "xmRTS.h"
#endif
#ifndef xrFName_H_
#include "xrFName.h"
#endif
#ifndef xlibOS_H_
#include "xlibOS.h"
#endif
#ifndef xrtsOS_H_
#include "xrtsOS.h"
#endif





xrTCov_TTestConditionTypeNames xrTCov_TestConditionNames = {"file\0\0\0\0\0\0\\
0\0\0\0\0\0\0","C1_proc\0\0\0\0\0\0\0\0\0\0","C1_proc_module\0\0\0","C1_proc_\
finally\0\0","C1_if\0\0\0\0\0\0\0\0\0\0\0\0","C1_if_else\0\0\0\0\0\0\0","C1_i\
f_elsif\0\0\0\0\0\0","C1_if_end\0\0\0\0\0\0\0\0","C1_case\0\0\0\0\0\0\0\0\0\0\
","C1_case_else\0\0\0\0\0","C1_case_end\0\0\0\0\0\0","C1_o2with\0\0\0\0\0\0\0\\
0","C1_o2with_else\0\0\0","C1_o2with_end\0\0\0\0","C1_loop_while\0\0\0\0","C1\
_loop_while_end","C1_loop_repeat\0\0\0",{'C','1','_','l','o','o','p','_','r',
                'e','p','e','a','t','_','e','n','d'},"C1_loop_for\0\0\0\0\0\0\
","C1_loop_for_end\0\0","C1_loop_loop\0\0\0\0\0","C1_loop_loop_end\0","Itr_lo\
op_while\0\0\0","Itr_loop_repeat\0\0","Itr_loop_for\0\0\0\0\0","Itr_loop_loop\\
0\0\0\0"};
static xrTCov_TModuleInfoPtr RegisteredModuleList;

static void X2C_EXIT_TESTCOVERAGE(void);


extern void xrTCov_RegistryModule(xrTCov_TModuleInfoPtr module,
                X2C_CARD32 modtime, X2C_CHAR modname[],
                X2C_CARD32 modname_len, struct xrTCov_TSourceRef srcrefs[],
                X2C_CARD32 srcrefs_len, X2C_INT32 counters[],
                X2C_CARD32 counters_len)
{
   if (module->count) return;
   if (RegisteredModuleList==0) X2C_atexit(X2C_EXIT_TESTCOVERAGE);
   module->count = (X2C_INT32)((srcrefs_len-1)+1UL);
   module->modtime = modtime;
   module->modname = (X2C_ADDRESS)modname;
   module->srcrefs = (X2C_ADDRESS)srcrefs;
   module->counters = (X2C_ADDRESS)counters;
   memset(module->counters,(X2C_LOC)0U,(X2C_CARD32)(4L*module->count));
   module->next = RegisteredModuleList;
   RegisteredModuleList = module;
} /* end RegistryModule() */


extern void xrTCov_IncreaseCounter(X2C_INT32 index, X2C_INT32 counters[],
                X2C_CARD32 counters_len)
{
   if (counters[index]<X2C_max_longint) ++counters[index];
} /* end IncreaseCounter() */


extern void xrTCov_IncreaseCounterExt(X2C_INT32 index, X2C_INT32 counters[],
                X2C_CARD32 counters_len, X2C_CARD32 modtime,
                xrTCov_TModuleInfoPtr module, X2C_CHAR modname[],
                X2C_CARD32 modname_len, struct xrTCov_TSourceRef srcrefs[],
                X2C_CARD32 srcrefs_len)
{
   if ((((module->count==0L || module->modtime!=modtime)
                || module->modname!=(X2C_ADDRESS)modname)
                || module->srcrefs!=(X2C_ADDRESS)srcrefs)
                || module->counters!=(X2C_ADDRESS)counters) {
      xrTCov_RegistryModule(module, modtime, modname, modname_len, srcrefs,
                srcrefs_len, counters, counters_len);
   }
   xrTCov_IncreaseCounter(index, counters, counters_len);
} /* end IncreaseCounterExt() */


extern void xrTCov_InitIterationLocal(X2C_INT32 * local_counter)
{
   *local_counter = 0L;
} /* end InitIterationLocal() */


extern void xrTCov_IncreaseIterationCounte(X2C_INT32 index,
                X2C_INT32 * local_counter, X2C_INT32 counters[],
                X2C_CARD32 counters_len)
{
   if (*local_counter<X2C_max_longint) {
      ++*local_counter;
      if (*local_counter>counters[index]) ++counters[index];
   }
} /* end IncreaseIterationCounter() */

#define xrTCov_malloc X2C_malloc

#define xrTCov_free X2C_free

#define xrTCov_strcmp X2C_STRCMP_PROC


static void Append(X2C_CHAR s[], X2C_CARD32 s_len, X2C_CHAR d[],
                X2C_CARD32 d_len)
{
   X2C_CARD32 len;
   X2C_CARD32 i;
   X2C_CARD32 pos;
   X2C_PCOPY((void **)&s,s_len);
   pos = X2C_LENGTH(d,d_len);
   len = X2C_LENGTH(s,s_len);
   if (pos+len>(d_len-1)+1UL) len = ((d_len-1)+1UL)-pos;
   i = 0UL;
   while (i<len) {
      d[pos] = s[i];
      ++i;
      ++pos;
   }
   if (pos<=d_len-1) d[pos] = 0;
   X2C_PFREE(s);
} /* end Append() */


static xrTCov_TModuleInfoPtr NewModuleInfo(X2C_INT32 count,
                const X2C_CHAR modname[], X2C_CARD32 modname_len)
{
   xrTCov_TModuleInfoPtr module;
   if (count<0L) return 0;
   module = (xrTCov_TModuleInfoPtr)
                X2C_malloc(sizeof(struct xrTCov_TModuleInfo));
   memset((X2C_ADDRESS)module,(X2C_LOC)0U,sizeof(struct xrTCov_TModuleInfo));
   module->modname = X2C_malloc(X2C_LENGTH(modname,modname_len)+1UL);
   module->srcrefs = X2C_malloc(sizeof(struct xrTCov_TSourceRef)*(X2C_CARD32)
                count);
   module->counters = X2C_malloc(4UL*(X2C_CARD32)count);
   module->count = count;
   X2C_COPY(modname,modname_len,(xrTCov_TModuleName)module->modname,32001u);
   return module;
} /* end NewModuleInfo() */


static void DisposeModule(xrTCov_TModuleInfoPtr * module)
{
   if (*module==0) return;
   X2C_free((*module)->modname, X2C_LENGTH((xrTCov_TModuleName)(*module)->modname,
                32001ul)+1UL);
   X2C_free((*module)->srcrefs, sizeof(struct xrTCov_TSourceRef)*(X2C_CARD32)(*module)->count);
   X2C_free((*module)->counters, 4UL*(X2C_CARD32)(*module)->count);
   X2C_free((X2C_ADDRESS)*module, sizeof(struct xrTCov_TModuleInfo));
   *module = 0;
} /* end DisposeModule() */

#define xrTCov_StdOutS X2C_StdOutS

#define xrTCov_StdOutD X2C_StdOutD

#define xrTCov_StdOutLn X2C_StdOutN

#define xrTCov_MAX_MessageLen 41

enum TMessage {xrTCov_msgFileOpen, 
   xrTCov_msgMakeBackup, 
   xrTCov_msgIncompatibleModuleIn, 
   xrTCov_msgIncorrectModuleInfo, 
   xrTCov_msgInconsistencyModuleI, 
   xrTCov_msgInvalidFileName, 
   xrTCov_msgUpdate, 
   xrTCov_msgDifferentModifyTime};


typedef X2C_CHAR TMessageText[8][42];

static TMessageText xrTCov_MessageText = {"File open error \0\0\0\0\0\0\0\0\0\\
0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0","Backup creation error \0\0\0\0\0\0\0\0\0\0\\
0\0\0\0\0\0\0\0\0","Incompatible module logs \0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\
","Incorrect module log \0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0","Inconsiste\
ncy module logs \0\0\0\0\0\0\0\0\0\0\0\0\0\0\0","Invalid file name \0\0\0\0\0\\
0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0","Update: \0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\\
0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0","Different source code modification time\\
0\0"};

#define xrTCov_MessagePrefix "#RTS Test Coverage: "

#define xrTCov_MergeError "merge informatiom error: "

#define xrTCov_EmptyPrefix "                    "

static X2C_INT32 ErrorCnt;

static TMessageText _cnst = {"File open error \0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\\
0\0\0\0\0\0\0\0\0\0","Backup creation error \0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\\
0\0\0","Incompatible module logs \0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0","Incorrect\
 module log \0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0","Inconsistency module l\
ogs \0\0\0\0\0\0\0\0\0\0\0\0\0\0\0","Invalid file name \0\0\0\0\0\0\0\0\0\0\0\\
0\0\0\0\0\0\0\0\0\0\0\0","Update: \0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\\
0\0\0\0\0\0\0\0\0\0\0\0","Different source code modification time\0\0"};

static void Error(X2C_CARD8 msg, const X2C_CHAR str[], X2C_CARD32 str_len,
                X2C_INT32 index)
{
   X2C_StdOutN();
   X2C_StdOutS("#RTS Test Coverage: ", 1UL);
   X2C_StdOutS("merge informatiom error: ", 1UL);
   X2C_StdOutS(str, 1UL);
   X2C_StdOutN();
   X2C_StdOutS("                    ", 1UL);
   X2C_StdOutS(_cnst[msg], 1UL);
   if (index>=0L) {
      X2C_StdOutS(" at index ", 1UL);
      X2C_StdOutD((X2C_CARD32)index, 1UL);
   }
   X2C_StdOutN();
   ++ErrorCnt;
} /* end Error() */


static void Message(X2C_CARD8 msg, const X2C_CHAR str[], X2C_CARD32 str_len)
{
   X2C_StdOutN();
   X2C_StdOutS("#RTS Test Coverage: ", 1UL);
   X2C_StdOutS(_cnst[msg], 1UL);
   X2C_StdOutS(str, 1UL);
   X2C_StdOutN();
} /* end Message() */

typedef X2C_CHAR TNumberBuf[12];

static X2C_OSFILE xrTCov_BADFILE = 0;

#define xrTCov_LogFileExt ".tc"

#define xrTCov_BackUpExt ".tcb"

#define xrTCov_MAXPATHLEN 1024

static X2C_BOOLEAN UseCurrentDir;


static void ReplaceExtension(X2C_CHAR name[], X2C_CARD32 name_len,
                const X2C_CHAR ext[], X2C_CARD32 ext_len)
{
   struct xrFName_Format f;
   xrFName_X2C_ParseFileName(name, name_len, &f);
   if (f.ok0) {
      if (f.extPos>0UL) name[f.extPos-1UL] = 0;
      Append(ext, ext_len, name, name_len);
   }
   else Error(xrTCov_msgInvalidFileName, name, name_len, -1L);
} /* end ReplaceExtension() */


static void GetInfoFileName(X2C_CHAR name[], X2C_CARD32 name_len)
{
   name[0UL] = 0;
   ProgEnv_String("TCSOUT", 7ul, name, name_len);
   if (name[0UL]==0) {
      ProgEnv_ProgramName(name, name_len);
      if (UseCurrentDir) {
         xrFName_X2C_ExtractBaseName(name, name_len, name, name_len);
         Append(".tc", 4ul, name, name_len);
      }
      else ReplaceExtension(name, name_len, ".tc", 4ul);
   }
} /* end GetInfoFileName() */


extern void xrTCov_MakeBackUp(X2C_CHAR fname[], X2C_CARD32 fname_len)
{
   X2C_CHAR newfname[1024];
   if (UseCurrentDir) {
      xrFName_X2C_ExtractBaseName(fname, fname_len, newfname, 1024ul);
      Append(".tcb", 5ul, newfname, 1024ul);
   }
   else {
      X2C_COPY(fname,fname_len,newfname,1024u);
      ReplaceExtension(newfname, 1024ul, ".tcb", 5ul);
   }
   if (X2C_Exists(newfname)) X2C_Remove(newfname);
   if (X2C_Rename((X2C_pCHAR)fname, newfname)) {
      Error(xrTCov_msgMakeBackup, newfname, 1024ul, -1L);
   }
} /* end MakeBackUp() */


static X2C_OSFILE OpenFile(const X2C_CHAR name[], X2C_CARD32 name_len)
{
   X2C_OSFILE file;
   if (X2C_FileOpenRead(&file, name)) {
      Error(xrTCov_msgFileOpen, name, name_len, -1L);
      file = 0;
   }
   return file;
} /* end OpenFile() */


static X2C_OSFILE CreateFile(const X2C_CHAR name[], X2C_CARD32 name_len)
{
   X2C_OSFILE file;
   if (X2C_FileOpenWrite(&file, name)) {
      Error(xrTCov_msgFileOpen, name, name_len, -1L);
      file = 0;
   }
   return file;
} /* end CreateFile() */


static void CloseFile(X2C_OSFILE * file)
{
   if (*file) {
      X2C_FileClose(*file);
      *file = 0;
   }
} /* end CloseFile() */

#define xrTCov_TAB "\011"

#define xrTCov_CR "\015"

#define xrTCov_LF "\012"

#define xrTCov_COMMENT "%"

#define xrTCov_SPACE " "

#define xrTCov_SPACES "        "

#define xrTCov_NEWLINE "\012"

#define xrTCov_StringSIZE 1024

#define xrTCov_ReadBufSIZE 3072

typedef X2C_CHAR TString[1025];

struct FILERD;


struct FILERD {
   X2C_OSFILE file;
   X2C_CHAR buf[3073];
   X2C_CARD32 len;
   X2C_CARD32 pos;
};


static void outS(X2C_OSFILE file, const X2C_CHAR str[], X2C_CARD32 str_len,
                X2C_CARD32 len)
{
   if (len>0UL) X2C_FileWrite(file, (X2C_ADDRESS)str, &len);
} /* end outS() */


static void writeLn(X2C_OSFILE file)
{
   outS(file, "\012", 2ul, 1UL);
} /* end writeLn() */


static void writeS(X2C_OSFILE file, const X2C_CHAR str[], X2C_CARD32 str_len,
                 X2C_CARD32 width)
{
   X2C_CARD32 len;
   len = X2C_LENGTH(str,str_len);
   outS(file, str, str_len, len);
   if (width>len) {
      while (width-len>8UL) {
         outS(file, "        ", 9ul, 8UL);
         len += 8UL;
      }
      if (width>len) outS(file, "        ", 9ul, width-len);
   }
} /* end writeS() */


static void writeDec(X2C_OSFILE file, X2C_INT32 number, X2C_CARD32 width)
{
   TNumberBuf buf;
   X2C_CARD32 len;
   len = 0UL;
   X2C_DecToStr(buf, &len, (X2C_CARD32)number);
   outS(file, buf, 12ul, len);
   if (width>len) {
      while (width-len>8UL) {
         outS(file, "        ", 9ul, 8UL);
         len += 8UL;
      }
      if (width>len) outS(file, "        ", 9ul, width-len);
   }
} /* end writeDec() */


static void AttachFILERD(struct FILERD * rfile, X2C_OSFILE file)
{
   rfile->file = file;
   rfile->pos = 3072UL;
   rfile->len = 3072UL;
} /* end AttachFILERD() */


static X2C_BOOLEAN inS(struct FILERD * file, X2C_CHAR str[],
                X2C_CARD32 str_len)
{
   X2C_CARD32 i;
   X2C_CHAR last_ch;
   str[0UL] = 0;
   i = 0UL;
   while (i<str_len-1) {
      if (file->pos>=file->len) {
         file->pos = 0UL;
         last_ch = file->buf[file->len-1UL];
         if (file->len<3072UL) return 0;
         if (X2C_FileRead(file->file, (X2C_ADDRESS)file->buf,
                &file->len)==0 && file->len==0UL) {
            if (i==0UL) return 0;
            else {
               str[i] = 0;
               return 1;
            }
         }
         if (last_ch=='\015' && file->buf[file->pos]=='\012') ++file->pos;
      }
      if (file->buf[file->pos]=='\015') {
         str[i] = 0;
         ++file->pos;
         if (file->buf[file->pos]==0) return 1;
         ++file->pos;
         return 1;
      }
      if (file->buf[file->pos]=='\012') {
         str[i] = 0;
         ++file->pos;
         return 1;
      }
      str[i] = file->buf[file->pos];
      ++i;
      ++file->pos;
   }
   X2C_TRAP(X2C_RETURN_TRAP);
   return 0;
} /* end inS() */


static X2C_BOOLEAN readS(struct FILERD * file, X2C_CHAR str[],
                X2C_CARD32 str_len)
{
   X2C_BOOLEAN res;
   do {
      res = inS(file, str, str_len);
   } while (!(!res || str[0UL]!='%'));
   return res;
} /* end readS() */

struct TModuleInfoHeader;


struct TModuleInfoHeader {
   X2C_INT32 count;
   X2C_CARD32 modtime;
   TString modname;
};


static X2C_BOOLEAN FindField(X2C_CHAR str[], X2C_CARD32 str_len,
                X2C_CHAR start_sym, X2C_CHAR end_sym, X2C_CARD32 * start_pos,
                 X2C_CARD32 * end_pos)
{
   X2C_CARD32 str_len0;
   X2C_BOOLEAN FindField_ret;
   X2C_PCOPY((void **)&str,str_len);
   str_len0 = X2C_LENGTH(str,str_len);
   if (start_sym==0) {
      while (*start_pos<str_len0 && str[*start_pos]==' ') ++*start_pos;
   }
   else {
      while (*start_pos<str_len0 && str[*start_pos]!=start_sym) ++*start_pos;
      ++*start_pos;
   }
   if (*start_pos==str_len0) {
      FindField_ret = 0;
      goto label;
   }
   *end_pos = *start_pos;
   while (*end_pos<str_len0 && str[*end_pos]!=end_sym) ++*end_pos;
   --*end_pos;
   if (*start_pos>*end_pos) {
      FindField_ret = 0;
      goto label;
   }
   FindField_ret = 1;
   label:;
   X2C_PFREE(str);
   return FindField_ret;
} /* end FindField() */

static xrTCov_TTestConditionTypeNames _cnst0 = {"file\0\0\0\0\0\0\0\0\0\0\0\0\\
0","C1_proc\0\0\0\0\0\0\0\0\0\0","C1_proc_module\0\0\0",
                "C1_proc_finally\0\0","C1_if\0\0\0\0\0\0\0\0\0\0\0\0","C1_if_\
else\0\0\0\0\0\0\0","C1_if_elsif\0\0\0\0\0\0","C1_if_end\0\0\0\0\0\0\0\0","C1\
_case\0\0\0\0\0\0\0\0\0\0","C1_case_else\0\0\0\0\0",
                "C1_case_end\0\0\0\0\0\0","C1_o2with\0\0\0\0\0\0\0\0","C1_o2w\
ith_else\0\0\0","C1_o2with_end\0\0\0\0","C1_loop_while\0\0\0\0","C1_loop_whil\
e_end","C1_loop_repeat\0\0\0",{'C','1','_','l','o','o','p','_','r','e','p',
                'e','a','t','_','e','n','d'},"C1_loop_for\0\0\0\0\0\0","C1_lo\
op_for_end\0\0","C1_loop_loop\0\0\0\0\0","C1_loop_loop_end\0","Itr_loop_while\\
0\0\0","Itr_loop_repeat\0\0","Itr_loop_for\0\0\0\0\0",
                "Itr_loop_loop\0\0\0\0"};

static X2C_BOOLEAN StrToType(const X2C_CHAR str[], X2C_CARD32 str_len,
                X2C_CARD32 * pos, X2C_CARD8 * type)
{
   X2C_CARD32 start_pos;
   X2C_CARD8 itype;
   start_pos = *pos;
   if (!FindField(str, str_len, '@', ' ', &start_pos, pos)) return 0;
   for (itype = xrTCov_cr_File;; itype++) {
      if (X2C_STRCMP_PROC((X2C_pVOID)xrTCov_TestConditionNames[itype],
                X2C_LENGTH(_cnst0[itype],18ul), (X2C_pVOID) &str[start_pos],
                (*pos-start_pos)+1UL)==0) {
         *type = itype;
         ++*pos;
         return 1;
      }
      if (itype==xrTCov_cr_Itr_Loop) break;
   } /* end for */
   return 0;
} /* end StrToType() */


static X2C_BOOLEAN StrToCounter(const X2C_CHAR str[], X2C_CARD32 str_len,
                X2C_CARD32 * pos, X2C_INT32 * count)
{
   X2C_CARD32 start_pos;
   X2C_CARD32 i;
   X2C_CARD32 tmp;
   start_pos = *pos;
   if (!FindField(str, str_len, '=', ' ', &start_pos, pos)) return 0;
   *count = 0L;
   tmp = *pos;
   i = start_pos;
   if (i<=tmp) for (;; i++) {
      *count =  *count*10L+(X2C_INT32)((X2C_CARD32)(X2C_CARD8)str[i]-48UL);
      if (i==tmp) break;
   } /* end for */
   ++*pos;
   return 1;
} /* end StrToCounter() */


static X2C_BOOLEAN StrToName(const X2C_CHAR str[], X2C_CARD32 str_len,
                X2C_CARD32 * pos, X2C_CHAR name[], X2C_CARD32 name_len)
{
   X2C_CARD32 start_pos;
   X2C_CARD32 len;
   X2C_CARD32 i;
   X2C_CARD32 tmp;
   start_pos = *pos;
   if (!FindField(str, str_len, '\"', '\"', &start_pos, pos)) return 0;
   len = *pos-start_pos;
   tmp = len;
   i = 0UL;
   if (i<=tmp) for (;; i++) {
      name[i] = str[start_pos+i];
      if (i==tmp) break;
   } /* end for */
   name[len+1UL] = 0;
   *pos += 2UL;
   return 1;
} /* end StrToName() */


static X2C_BOOLEAN StrToModTime(const X2C_CHAR str[], X2C_CARD32 str_len,
                X2C_CARD32 * pos, X2C_CARD32 * time0)
{
   X2C_CARD32 start_pos;
   X2C_CARD32 i;
   X2C_CARD32 tmp;
   start_pos = *pos;
   if (!FindField(str, str_len, 0, ' ', &start_pos, pos)) return 0;
   if (str[*pos]==']') --*pos;
   *time0 = 0UL;
   tmp = *pos;
   i = start_pos;
   if (i<=tmp) for (;; i++) {
      *time0 =  *time0*10UL+(X2C_CARD32)((X2C_CARD32)(X2C_CARD8)str[i]-48UL);
      if (i==tmp) break;
   } /* end for */
   ++*pos;
   return 1;
} /* end StrToModTime() */


static X2C_BOOLEAN StrToIndex(const X2C_CHAR str[], X2C_CARD32 str_len,
                X2C_CARD32 * pos, X2C_INT32 * index)
{
   X2C_CARD32 start_pos;
   X2C_CARD32 i;
   X2C_CARD32 tmp;
   start_pos = *pos;
   if (!FindField(str, str_len, 0, ' ', &start_pos, pos)) return 0;
   *index = 0L;
   tmp = *pos;
   i = start_pos;
   if (i<=tmp) for (;; i++) {
      *index =  *index*10L+(X2C_INT32)((X2C_CARD32)(X2C_CARD8)str[i]-48UL);
      if (i==tmp) break;
   } /* end for */
   ++*pos;
   return 1;
} /* end StrToIndex() */


static X2C_BOOLEAN StrToLine(const X2C_CHAR str[], X2C_CARD32 str_len,
                X2C_CARD32 * pos, X2C_INT32 * line)
{
   X2C_CARD32 start_pos;
   X2C_CARD32 i;
   X2C_CARD32 tmp;
   start_pos = *pos;
   if (!FindField(str, str_len, 0, '.', &start_pos, pos)) return 0;
   *line = 0L;
   tmp = *pos;
   i = start_pos;
   if (i<=tmp) for (;; i++) {
      *line =  *line*10L+(X2C_INT32)((X2C_CARD32)(X2C_CARD8)str[i]-48UL);
      if (i==tmp) break;
   } /* end for */
   ++*pos;
   return 1;
} /* end StrToLine() */


static X2C_BOOLEAN StrToCol(const X2C_CHAR str[], X2C_CARD32 str_len,
                X2C_CARD32 * pos, X2C_INT32 * col)
{
   X2C_CARD32 start_pos;
   X2C_CARD32 i;
   X2C_CARD32 tmp;
   start_pos = *pos;
   if (!FindField(str, str_len, '.', ' ', &start_pos, pos)) return 0;
   if (str[*pos]==']') --*pos;
   *col = 0L;
   tmp = *pos;
   i = start_pos;
   if (i<=tmp) for (;; i++) {
      *col =  *col*10L+(X2C_INT32)((X2C_CARD32)(X2C_CARD8)str[i]-48UL);
      if (i==tmp) break;
   } /* end for */
   ++*pos;
   return 1;
} /* end StrToCol() */


static X2C_BOOLEAN StrToModuleInfoHeader(const X2C_CHAR str[],
                X2C_CARD32 str_len, struct TModuleInfoHeader * header)
{
   X2C_CARD32 pos;
   X2C_CARD8 type;
   pos = 0UL;
   if ((((StrToType(str, str_len, &pos,
                &type) && type==xrTCov_cr_File) && StrToCounter(str, str_len,
                 &pos, &header->count)) && StrToName(str, str_len, &pos,
                header->modname, 1025ul)) && StrToModTime(str, str_len, &pos,
                 &header->modtime)) return 1;
   else return 0;
   return 0;
} /* end StrToModuleInfoHeader() */


static X2C_BOOLEAN Search(X2C_CARD32 len, X2C_CHAR name[],
                X2C_CARD32 name_len, xrTCov_TModuleInfoPtr * module)
{
   while (*module) {
      if (X2C_STRCMP_PROC((X2C_pVOID)(*module)->modname,
                X2C_LENGTH((xrTCov_TModuleName)(*module)->modname,32001ul),
                (X2C_pVOID)name, len)==0) return 1;
      *module = (*module)->next;
   }
   return 0;
} /* end Search() */


static xrTCov_TModuleInfoPtr FindModuleInfoByName(X2C_CHAR name[],
                X2C_CARD32 name_len, xrTCov_TModuleInfoPtr regModuleList,
                xrTCov_TModuleInfoPtr unregModuleList)
{
   xrTCov_TModuleInfoPtr module;
   X2C_CARD32 len;
   xrTCov_TModuleInfoPtr FindModuleInfoByName_ret;
   X2C_PCOPY((void **)&name,name_len);
   len = X2C_LENGTH(name,name_len);
   module = regModuleList;
   if (Search(len, name, name_len, &module)) {
      FindModuleInfoByName_ret = module;
      goto label;
   }
   module = unregModuleList;
   if (Search(len, name, name_len, &module)) {
      FindModuleInfoByName_ret = module;
      goto label;
   }
   FindModuleInfoByName_ret = 0;
   label:;
   X2C_PFREE(name);
   return FindModuleInfoByName_ret;
} /* end FindModuleInfoByName() */


static X2C_BOOLEAN IsComptibleLogs(xrTCov_TModuleInfoPtr module,
                const struct TModuleInfoHeader header)
{
   if (module->modtime!=header.modtime) {
      Error(xrTCov_msgDifferentModifyTime, header.modname, 1025ul, -1L);
   }
   else if (module->count!=header.count) {
      Error(xrTCov_msgIncompatibleModuleIn, header.modname, 1025ul, -1L);
   }
   else return 1;
   return 0;
} /* end IsComptibleLogs() */


static X2C_INT32 MaxCounter(X2C_INT32 count1, X2C_INT32 count2)
{
   if (count1>count2) return count1;
   else return count2;
   return 0;
} /* end MaxCounter() */


static void MergeModuleInfo(struct FILERD * file,
                xrTCov_TModuleInfoPtr module)
{
   X2C_INT32 i;
   TString str;
   X2C_CARD32 pos;
   X2C_INT32 index;
   X2C_CARD8 type;
   X2C_INT32 count;
   TString name;
   X2C_INT32 end_line;
   X2C_INT32 line;
   X2C_INT32 end_col;
   X2C_INT32 col;
   X2C_CARD32 modname_len;
   X2C_INT32 tmp;
   modname_len = X2C_LENGTH((xrTCov_TModuleName)module->modname,32001ul);
   tmp = module->count-1L;
   i = 0L;
   if (i<=tmp) for (;; i++) {
      pos = 0UL;
      if ((((((((readS(file, str, 1025ul) && StrToIndex(str, 1025ul, &pos,
                &index)) && StrToType(str, 1025ul, &pos,
                &type)) && StrToCounter(str, 1025ul, &pos,
                &count)) && StrToName(str, 1025ul, &pos, name,
                1025ul)) && StrToLine(str, 1025ul, &pos,
                &line)) && StrToCol(str, 1025ul, &pos,
                &col)) && StrToLine(str, 1025ul, &pos,
                &end_line)) && StrToCol(str, 1025ul, &pos, &end_col)) {
         if (((((index==i && line==((xrTCov_TSourceRefs)module->srcrefs)
                [i].line) && col==((xrTCov_TSourceRefs)module->srcrefs)
                [i].col) && end_line==((xrTCov_TSourceRefs)module->srcrefs)
                [i].end_line) && end_col==((xrTCov_TSourceRefs)
                module->srcrefs)[i].end_col) && X2C_STRCMP_PROC((X2C_pVOID)
                module->modname, modname_len, (X2C_pVOID)name,
                X2C_LENGTH(name,1025ul))==0) {
            ((xrTCov_TCounters)module->counters)[i] = MaxCounter(count,
                ((xrTCov_TCounters)module->counters)[i]);
         }
         else {
            Error(xrTCov_msgInconsistencyModuleI,
                (xrTCov_TModuleName)module->modname, 32001ul, i);
            return;
         }
      }
      else {
         Error(xrTCov_msgIncorrectModuleInfo,
                (xrTCov_TModuleName)module->modname, 32001ul, i);
         return;
      }
      if (i==tmp) break;
   } /* end for */
} /* end MergeModuleInfo() */


static void ReadUnRegisteredModuleInfo(struct FILERD * file,
                const struct TModuleInfoHeader header,
                xrTCov_TModuleInfoPtr * unregModuleList)
{
   xrTCov_TModuleInfoPtr module;
   X2C_CARD32 modname_len;
   X2C_INT32 i;
   TString str;
   X2C_CARD32 pos;
   X2C_INT32 index;
   X2C_CARD8 type;
   X2C_INT32 count;
   TString name;
   X2C_INT32 end_line;
   X2C_INT32 line;
   X2C_INT32 end_col;
   X2C_INT32 col;
   X2C_INT32 tmp;
   module = NewModuleInfo(header.count, header.modname, 1025ul);
   if (module==0) return;
   module->modtime = header.modtime;
   modname_len = X2C_LENGTH((xrTCov_TModuleName)module->modname,32001ul);
   tmp = module->count-1L;
   i = 0L;
   if (i<=tmp) for (;; i++) {
      pos = 0UL;
      if ((((((((readS(file, str, 1025ul) && StrToIndex(str, 1025ul, &pos,
                &index)) && StrToType(str, 1025ul, &pos,
                &type)) && StrToCounter(str, 1025ul, &pos,
                &count)) && StrToName(str, 1025ul, &pos, name,
                1025ul)) && StrToLine(str, 1025ul, &pos,
                &line)) && StrToCol(str, 1025ul, &pos,
                &col)) && StrToLine(str, 1025ul, &pos,
                &end_line)) && StrToCol(str, 1025ul, &pos, &end_col)) {
         if (X2C_STRCMP_PROC((X2C_pVOID)module->modname, modname_len,
                (X2C_pVOID)name, X2C_LENGTH(name,1025ul))==0) {
            ((xrTCov_TSourceRefs)module->srcrefs)[index].type = type;
            ((xrTCov_TSourceRefs)module->srcrefs)[index].line = line;
            ((xrTCov_TSourceRefs)module->srcrefs)[index].col = col;
            ((xrTCov_TSourceRefs)module->srcrefs)[index].end_line = end_line;
            ((xrTCov_TSourceRefs)module->srcrefs)[index].end_col = end_col;
            ((xrTCov_TCounters)module->counters)[index] = count;
         }
         else {
            Error(xrTCov_msgInconsistencyModuleI,
                (xrTCov_TModuleName)module->modname, 32001ul, i);
            DisposeModule(&module);
            return;
         }
      }
      else {
         Error(xrTCov_msgIncorrectModuleInfo,
                (xrTCov_TModuleName)module->modname, 32001ul, i);
         DisposeModule(&module);
         return;
      }
      if (i==tmp) break;
   } /* end for */
   module->next = *unregModuleList;
   *unregModuleList = module;
} /* end ReadUnRegisteredModuleInfo() */


static void ReadStatistics(X2C_OSFILE file,
                xrTCov_TModuleInfoPtr regModuleList,
                xrTCov_TModuleInfoPtr * unregModuleList)
{
   struct FILERD rfile;
   TString str;
   struct TModuleInfoHeader modheader;
   xrTCov_TModuleInfoPtr module;
   AttachFILERD(&rfile, file);
   while (readS(&rfile, str, 1025ul)) {
      if (StrToModuleInfoHeader(str, 1025ul, &modheader)) {
         module = FindModuleInfoByName(modheader.modname, 1025ul,
                regModuleList, *unregModuleList);
         if (module) {
            if (IsComptibleLogs(module, modheader)) {
               MergeModuleInfo(&rfile, module);
            }
         }
         else ReadUnRegisteredModuleInfo(&rfile, modheader, unregModuleList);
      }
   }
} /* end ReadStatistics() */


extern void xrTCov_ReadTCFile(X2C_CHAR fname[], X2C_CARD32 fname_len,
                xrTCov_TModuleInfoPtr regModuleList,
                xrTCov_TModuleInfoPtr * unregModuleList)
{
   X2C_OSFILE file;
   file = OpenFile(fname, fname_len);
   if (file) {
      ReadStatistics(file, regModuleList, unregModuleList);
      CloseFile(&file);
   }
} /* end ReadTCFile() */


static void WriteModuleModifyTime(X2C_OSFILE file, X2C_CARD32 time0)
{
   struct X2C_TimeStruct res;
   struct X2C_TimeStruct ini;
   writeDec(file, (X2C_INT32)time0, 0UL);
   writeS(file, " ", 2ul, 1UL);
   X2C_GetTime(&ini);
   ini.year = 1970UL;
   ini.month = 1UL;
   ini.day = 1UL;
   ini.hour = 0UL;
   ini.min0 = 0UL;
   ini.sec = 0UL;
   ini.fracs = 0UL;
   X2C_TimeSecAdd(ini, time0, &res);
   writeDec(file, (X2C_INT32)res.day, 1UL);
   writeS(file, ".", 2ul, 1UL);
   writeDec(file, (X2C_INT32)res.month, 1UL);
   writeS(file, ".", 2ul, 1UL);
   writeDec(file, (X2C_INT32)res.year, 1UL);
   writeS(file, " ", 2ul, 1UL);
   writeDec(file, (X2C_INT32)res.hour, 1UL);
   writeS(file, ":", 2ul, 1UL);
   writeDec(file, (X2C_INT32)res.min0, 1UL);
} /* end WriteModuleModifyTime() */


static void WriteModuleHeader(X2C_OSFILE file, xrTCov_TModuleInfoPtr module)
{
   writeS(file, "     @", 7ul, 1UL);
   writeS(file, "file\0\0\0\0\0\0\0\0\0\0\0\0\0", 18ul, 18UL);
   writeS(file, " =", 3ul, 1UL);
   writeDec(file, module->count, 8UL);
   writeS(file, " [\"", 4ul, 1UL);
   writeS(file, (xrTCov_TModuleName)module->modname, 32001ul, 1UL);
   writeS(file, "\" ", 3ul, 1UL);
   WriteModuleModifyTime(file, module->modtime);
   writeS(file, "]", 2ul, 1UL);
   writeLn(file);
} /* end WriteModuleHeader() */


static void WriteModule(X2C_OSFILE file, xrTCov_TModuleInfoPtr module)
{
   X2C_INT32 i;
   X2C_INT32 tmp;
   WriteModuleHeader(file, module);
   tmp = module->count-1L;
   i = 0L;
   if (i<=tmp) for (;; i++) {
      writeDec(file, i, 4UL);
      writeS(file, " @", 3ul, 1UL);
      writeS(file, _cnst0[((xrTCov_TSourceRefs)module->srcrefs)[i].type],
                18ul, 18UL);
      writeS(file, " =", 3ul, 1UL);
      writeDec(file, ((xrTCov_TCounters)module->counters)[i], 8UL);
      writeS(file, " [\"", 4ul, 1UL);
      writeS(file, (xrTCov_TModuleName)module->modname, 32001ul, 1UL);
      writeS(file, "\" ", 3ul, 1UL);
      writeDec(file, ((xrTCov_TSourceRefs)module->srcrefs)[i].line, 1UL);
      writeS(file, ".", 2ul, 1UL);
      writeDec(file, ((xrTCov_TSourceRefs)module->srcrefs)[i].col, 1UL);
      writeS(file, " ", 2ul, 1UL);
      writeDec(file, ((xrTCov_TSourceRefs)module->srcrefs)[i].end_line, 1UL);
      writeS(file, ".", 2ul, 1UL);
      writeDec(file, ((xrTCov_TSourceRefs)module->srcrefs)[i].end_col, 1UL);
      writeS(file, "]", 2ul, 1UL);
      writeLn(file);
      if (i==tmp) break;
   } /* end for */
} /* end WriteModule() */


extern void xrTCov_WriteTCFile(X2C_CHAR fname[], X2C_CARD32 fname_len,
                xrTCov_TModuleInfoPtr regModuleList,
                xrTCov_TModuleInfoPtr * unregModuleList)
{
   X2C_OSFILE file;
   xrTCov_TModuleInfoPtr tmp;
   file = CreateFile(fname, fname_len);
   if (file==0) return;
   while (regModuleList) {
      WriteModule(file, regModuleList);
      regModuleList = regModuleList->next;
   }
   while (*unregModuleList) {
      WriteModule(file, *unregModuleList);
      tmp = *unregModuleList;
      *unregModuleList = (*unregModuleList)->next;
      DisposeModule(&tmp);
   }
   CloseFile(&file);
} /* end WriteTCFile() */


extern void xrTCov_CancelTCFileOperation(xrTCov_TModuleInfoPtr * unregModuleList)
                
{
   xrTCov_TModuleInfoPtr tmp;
   while (*unregModuleList) {
      tmp = *unregModuleList;
      *unregModuleList = (*unregModuleList)->next;
      DisposeModule(&tmp);
   }
} /* end CancelTCFileOperation() */


extern void X2C_INIT_TESTCOVERAGE(void)
{
   RegisteredModuleList = 0;
} /* end X2C_INIT_TESTCOVERAGE() */


static void X2C_EXIT_TESTCOVERAGE(void)
{
   X2C_CHAR fname[1024];
   xrTCov_TModuleInfoPtr unregModuleList;
   if (RegisteredModuleList==0) return;
   ErrorCnt = 0L;
   unregModuleList = 0;
   UseCurrentDir = 0;
   GetInfoFileName(fname, 1024ul);
   if (X2C_Exists(fname)) {
      xrTCov_ReadTCFile(fname, 1024ul, RegisteredModuleList,
                &unregModuleList);
      xrTCov_MakeBackUp(fname, 1024ul);
   }
   xrTCov_WriteTCFile(fname, 1024ul, RegisteredModuleList, &unregModuleList);
   Message(xrTCov_msgUpdate, fname, 1024ul);
} /* end X2C_EXIT_TESTCOVERAGE() */


extern void xrTCov_EnumFileLines(X2C_CHAR fname[], X2C_CARD32 fname_len,
                xrTCov_FILE_LINE_ITERATOR action)
{
   X2C_OSFILE file;
   struct FILERD rfile;
   TString str;
   file = OpenFile(fname, fname_len);
   if (file) {
      AttachFILERD(&rfile, file);
      while (inS(&rfile, str, 1025ul)) action(str, 1025ul);
      CloseFile(&file);
   }
} /* end EnumFileLines() */


extern void xrTCov_BEGIN(void)
{
   static int xrTCov_init = 0;
   if (xrTCov_init) return;
   xrTCov_init = 1;
   if (sizeof(xrTCov_TTestConditionTypeNames)!=468) X2C_ASSERT(0);
   if (sizeof(TMessageText)!=336) X2C_ASSERT(0);
   if (sizeof(TNumberBuf)!=12) X2C_ASSERT(0);
   if (sizeof(TString)!=1025) X2C_ASSERT(0);
   xrFName_BEGIN();
   ProgEnv_BEGIN();
}

