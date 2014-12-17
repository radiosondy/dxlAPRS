/* XDS v2.60: Copyright (c) 1999-2011 Excelsior, LLC. All Rights Reserved. */
/* "@(#)Storage.c Feb  3 14:30:28 2012" */
/* Generated by XDS Modula-2 to ANSI C v4.20 translator */

#define X2C_int32
#define X2C_index32
#ifndef Storage_H_
#include "Storage.h"
#endif
#define Storage_C_
#ifndef EXCEPTIONS_H_
#include "EXCEPTIONS.h"
#endif
#ifndef xmRTS_H_
#include "xmRTS.h"
#endif

static EXCEPTIONS_ExceptionSource source;


extern void Storage_ALLOCATE(X2C_ADDRESS * a, X2C_CARD32 size)
{
   X2C_ALLOCATE(a, size);
} /* end ALLOCATE() */


extern void Storage_DEALLOCATE(X2C_ADDRESS * a, X2C_CARD32 size)
{
   if (*a==0) {
      EXCEPTIONS_RAISE(source, 0UL, "first argument of DEALLOCATE is NIL",
                36ul);
   }
   X2C_DEALLOCATE(a);
} /* end DEALLOCATE() */


extern X2C_BOOLEAN Storage_IsStorageException(void)
{
   return EXCEPTIONS_IsCurrentSource(source);
} /* end IsStorageException() */


extern X2C_CARD8 Storage_StorageException(void)
{
   return (X2C_CARD8)EXCEPTIONS_CurrentNumber(source);
} /* end StorageException() */


extern void Storage_BEGIN(void)
{
   static int Storage_init = 0;
   if (Storage_init) return;
   Storage_init = 1;
   EXCEPTIONS_BEGIN();
   EXCEPTIONS_AllocateSource(&source);
}

