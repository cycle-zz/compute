/***************************************************************************************************

Compute Engine - $CE_VERSION_TAG$ <$CE_ID_TAG$>

Copyright (c) 2010, Derek Gerstmann <derek.gerstmann[|AT|]uwa.edu.au> 
The University of Western Australia. All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.

    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.

    * Neither the name of the <organization> nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

***************************************************************************************************/

#ifndef __CE_VALUES_H__
#define __CE_VALUES_H__

/**************************************************************************************************/

#include "platform.h"
#include "types.h"
#include "status.h"

/**************************************************************************************************/

CE_EXTERN_C_BEGIN

/**************************************************************************************************/

extern CE_API_EXPORT ce_value
ceCreateValue(ce_session, ce_type type);

extern CE_API_EXPORT ce_value
ceCreateBoolValue(ce_session, ce_bool data);

extern CE_API_EXPORT ce_value
ceCreateCharValue(ce_session, ce_char data);

extern CE_API_EXPORT ce_value
ceCreateUCharValue(ce_session, ce_uchar data);

extern CE_API_EXPORT ce_value
ceCreateShortValue(ce_session, ce_short data);

extern CE_API_EXPORT ce_value
ceCreateUShortValue(ce_session, ce_ushort data);

extern CE_API_EXPORT ce_value
ceCreateIntValue(ce_session, ce_int data);

extern CE_API_EXPORT ce_value
ceCreateUIntValue(ce_session, ce_uint data);

extern CE_API_EXPORT ce_value
ceCreateLongValue(ce_session, ce_long data);

extern CE_API_EXPORT ce_value
ceCreateULongValue(ce_session, ce_ulong data);

extern CE_API_EXPORT ce_value
ceCreateFloatValue(ce_session, ce_float data);

extern CE_API_EXPORT ce_value
ceCreateDoubleValue(ce_session, ce_double data);

extern CE_API_EXPORT ce_value
ceCreateSymbolValue(ce_session, ce_symbol data);

extern CE_API_EXPORT ce_status
ceReleaseValue(ce_value);

extern CE_API_EXPORT ce_status
ceSetBoolValue(ce_value value, ce_bool data);

extern CE_API_EXPORT ce_bool
ceGetBoolValue(ce_value value, ce_status* status);

extern CE_API_EXPORT ce_status
ceSetCharValue(ce_value value, ce_char data);

extern CE_API_EXPORT ce_char
ceGetCharValue(ce_value value, ce_status* status);

extern CE_API_EXPORT ce_status
ceSetUCharValue(ce_value value, ce_uchar data);

extern CE_API_EXPORT ce_uchar
ceGetUCharValue(ce_value value, ce_status* status);

extern CE_API_EXPORT ce_status
ceSetShortValue(ce_value value, ce_short data);

extern CE_API_EXPORT ce_short
ceGetShortValue(ce_value value, ce_status* status);

extern CE_API_EXPORT ce_status
ceSetUShortValue(ce_value value, ce_ushort data);

extern CE_API_EXPORT ce_ushort
ceGetUShortValue(ce_value value, ce_status* status);

extern CE_API_EXPORT ce_status
ceSetIntValue(ce_value value, ce_int data);

extern CE_API_EXPORT ce_int
ceGetIntValue(ce_value value, ce_status* status);

extern CE_API_EXPORT ce_status
ceSetUIntValue(ce_value value, ce_uint data);

extern CE_API_EXPORT ce_uint
ceGetUIntValue(ce_value value, ce_status* status);

extern CE_API_EXPORT ce_status
ceSetLongValue(ce_value value, ce_long data);

extern CE_API_EXPORT ce_long
ceGetLongValue(ce_value value, ce_status* status);

extern CE_API_EXPORT ce_status
ceSetULongValue(ce_value value, ce_ulong data);

extern CE_API_EXPORT ce_ulong
ceGetULongValue(ce_value value, ce_status* status);

extern CE_API_EXPORT ce_status
ceSetFloatValue(ce_value value, ce_float data);

extern CE_API_EXPORT ce_float
ceGetFloatValue(ce_value value, ce_status* status);

extern CE_API_EXPORT ce_status
ceSetDoubleValue(ce_value value, ce_double data);

extern CE_API_EXPORT ce_double
ceGetDoubleValue(ce_value value, ce_status* status);

extern CE_API_EXPORT ce_status
ceSetSymbolValue(ce_value value, ce_symbol data);

extern CE_API_EXPORT ce_symbol
ceGetSymbolValue(ce_value value, ce_status* status);

extern CE_API_EXPORT ce_type
ceGetValueType(ce_value, ce_status* status);

extern CE_API_EXPORT ce_bool
ceIsValueEqual(ce_value a, ce_value b, ce_status* status);

/**************************************************************************************************/

CE_EXTERN_C_END

#endif /* __CE_VALUES_H__ */
