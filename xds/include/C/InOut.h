/* XDS v2.60: Copyright (c) 1999-2011 Excelsior, LLC. All Rights Reserved. */
/* Generated by XDS Modula-2 to ANSI C v4.20 translator */

#ifndef InOut_H_
#define InOut_H_
#ifndef X2C_H_
#include "X2C.h"
#endif

#define InOut_EOL "\012"

extern X2C_BOOLEAN InOut_Done;

extern X2C_CHAR InOut_termCH;

extern void InOut_OpenInput(X2C_CHAR [], X2C_CARD32);

extern void InOut_OpenOutput(X2C_CHAR [], X2C_CARD32);

extern void InOut_CloseInput(void);

extern void InOut_CloseOutput(void);

extern void InOut_Read(X2C_CHAR *);

extern void InOut_ReadString(X2C_CHAR [], X2C_CARD32);

extern void InOut_ReadCard(X2C_CARD32 *);

extern void InOut_ReadInt(X2C_INT32 *);

extern void InOut_Write(X2C_CHAR);

extern void InOut_WriteLn(void);

extern void InOut_WriteString(X2C_CHAR [], X2C_CARD32);

extern void InOut_WriteCard(X2C_CARD32, X2C_CARD32);

extern void InOut_WriteInt(X2C_INT32, X2C_CARD32);

extern void InOut_WriteOct(X2C_CARD32, X2C_CARD32);

extern void InOut_WriteHex(X2C_CARD32, X2C_CARD32);


extern void InOut_BEGIN(void);


#endif /* InOut_H_ */
