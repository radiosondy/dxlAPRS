/* XDS v2.60: Copyright (c) 1999-2011 Excelsior, LLC. All Rights Reserved. */
/* Generated by XDS Oberon-2 to ANSI C v4.20 translator */

#ifndef O2Strings_H_
#define O2Strings_H_
#ifndef X2C_H_
#include "X2C.h"
#endif

extern X2C_INT16 O2Strings_Length(X2C_CHAR [], X2C_CARD32);

extern void O2Strings_Insert(X2C_CHAR [], X2C_CARD32, X2C_INT16, X2C_CHAR [],
                 X2C_CARD32);

extern void O2Strings_Append(X2C_CHAR [], X2C_CARD32, X2C_CHAR [],
                X2C_CARD32);

extern void O2Strings_Delete(X2C_CHAR [], X2C_CARD32, X2C_INT16, X2C_INT16);

extern void O2Strings_Replace(X2C_CHAR [], X2C_CARD32, X2C_INT16,
                X2C_CHAR [], X2C_CARD32);

extern void O2Strings_Extract(X2C_CHAR [], X2C_CARD32, X2C_INT16, X2C_INT16,
                X2C_CHAR [], X2C_CARD32);

extern X2C_INT16 O2Strings_Pos(X2C_CHAR [], X2C_CARD32, X2C_CHAR [],
                X2C_CARD32, X2C_INT16);

extern void O2Strings_Cap(X2C_CHAR [], X2C_CARD32);


extern void O2Strings_BEGIN(void);


#endif /* O2Strings_H_ */
