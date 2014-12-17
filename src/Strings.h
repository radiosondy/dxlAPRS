/* XDS v2.51: Copyright (c) 1999-2003 Excelsior, LLC. All Rights Reserved. */
/* Generated by XDS Modula-2 to ANSI C v4.20 translator */

#ifndef Strings_H_
#define Strings_H_
#ifndef X2C_H_
#include "X2C.h"
#endif

typedef X2C_CHAR Strings_String1[1];

extern X2C_CARD32 Strings_Length(X2C_CHAR [], X2C_CARD32);

extern void Strings_Assign(X2C_CHAR [], X2C_CARD32, X2C_CHAR [], X2C_CARD32);

extern void Strings_Extract(X2C_CHAR [], X2C_CARD32, X2C_CARD32, X2C_CARD32,
                X2C_CHAR [], X2C_CARD32);

extern void Strings_Delete(X2C_CHAR [], X2C_CARD32, X2C_CARD32, X2C_CARD32);

extern void Strings_Insert(X2C_CHAR [], X2C_CARD32, X2C_CARD32, X2C_CHAR [],
                X2C_CARD32);

extern void Strings_Replace(X2C_CHAR [], X2C_CARD32, X2C_CARD32, X2C_CHAR [],
                 X2C_CARD32);

extern void Strings_Append(X2C_CHAR [], X2C_CARD32, X2C_CHAR [], X2C_CARD32);

extern void Strings_Concat(X2C_CHAR [], X2C_CARD32, X2C_CHAR [], X2C_CARD32,
                X2C_CHAR [], X2C_CARD32);

extern X2C_BOOLEAN Strings_CanAssignAll(X2C_CARD32, X2C_CHAR [], X2C_CARD32);

extern X2C_BOOLEAN Strings_CanExtractAll(X2C_CARD32, X2C_CARD32, X2C_CARD32,
                X2C_CHAR [], X2C_CARD32);

extern X2C_BOOLEAN Strings_CanDeleteAll(X2C_CARD32, X2C_CARD32, X2C_CARD32);

extern X2C_BOOLEAN Strings_CanInsertAll(X2C_CARD32, X2C_CARD32, X2C_CHAR [],
                X2C_CARD32);

extern X2C_BOOLEAN Strings_CanReplaceAll(X2C_CARD32, X2C_CARD32, X2C_CHAR [],
                 X2C_CARD32);

extern X2C_BOOLEAN Strings_CanAppendAll(X2C_CARD32, X2C_CHAR [], X2C_CARD32);

extern X2C_BOOLEAN Strings_CanConcatAll(X2C_CARD32, X2C_CARD32, X2C_CHAR [],
                X2C_CARD32);

enum Strings_CompareResults {Strings_less, Strings_equal0, Strings_greater};


extern X2C_CARD8 Strings_Compare(X2C_CHAR [], X2C_CARD32, X2C_CHAR [],
                X2C_CARD32);

extern X2C_BOOLEAN Strings_Equal(X2C_CHAR [], X2C_CARD32, X2C_CHAR [],
                X2C_CARD32);

extern void Strings_FindNext(X2C_CHAR [], X2C_CARD32, X2C_CHAR [],
                X2C_CARD32, X2C_CARD32, X2C_BOOLEAN *, X2C_CARD32 *);

extern void Strings_FindPrev(X2C_CHAR [], X2C_CARD32, X2C_CHAR [],
                X2C_CARD32, X2C_CARD32, X2C_BOOLEAN *, X2C_CARD32 *);

extern void Strings_FindDiff(X2C_CHAR [], X2C_CARD32, X2C_CHAR [],
                X2C_CARD32, X2C_BOOLEAN *, X2C_CARD32 *);

extern void Strings_Capitalize(X2C_CHAR [], X2C_CARD32);


extern void Strings_BEGIN(void);


#endif /* Strings_H_ */
