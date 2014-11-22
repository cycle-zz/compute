/***************************************************************************************************

Scalable Compute Library - $SC_VERSION_TAG$ <$SC_ID_TAG$>

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

#ifndef __SC_VALUES_H__
#define __SC_VALUES_H__

/**************************************************************************************************/

#include "platform.h"
#include "types.h"
#include "status.h"

/**************************************************************************************************/

SC_EXTERN_C_BEGIN

/**************************************************************************************************/

extern SC_API_EXPORT sc_value
scCreateValue(sc_session, sc_type type);

extern SC_API_EXPORT sc_value
scCreateBoolValue(sc_session, sc_bool data);

extern SC_API_EXPORT sc_value
scCreateCharValue(sc_session, sc_char data);

extern SC_API_EXPORT sc_value
scCreateUCharValue(sc_session, sc_uchar data);

extern SC_API_EXPORT sc_value
scCreateShortValue(sc_session, sc_short data);

extern SC_API_EXPORT sc_value
scCreateUShortValue(sc_session, sc_ushort data);

extern SC_API_EXPORT sc_value
scCreateIntValue(sc_session, sc_int data);

extern SC_API_EXPORT sc_value
scCreateUIntValue(sc_session, sc_uint data);

extern SC_API_EXPORT sc_value
scCreateLongValue(sc_session, sc_long data);

extern SC_API_EXPORT sc_value
scCreateULongValue(sc_session, sc_ulong data);

extern SC_API_EXPORT sc_value
scCreateFloatValue(sc_session, sc_float data);

extern SC_API_EXPORT sc_value
scCreateDoubleValue(sc_session, sc_double data);

extern SC_API_EXPORT sc_value
scCreateSymbolValue(sc_session, sc_symbol data);

extern SC_API_EXPORT sc_status
scReleaseValue(sc_value);

extern SC_API_EXPORT sc_status
scSetBoolValue(sc_value value, sc_bool data);

extern SC_API_EXPORT sc_bool
scGetBoolValue(sc_value value, sc_status* status);

extern SC_API_EXPORT sc_status
scSetCharValue(sc_value value, sc_char data);

extern SC_API_EXPORT sc_char
scGetCharValue(sc_value value, sc_status* status);

extern SC_API_EXPORT sc_status
scSetUCharValue(sc_value value, sc_uchar data);

extern SC_API_EXPORT sc_uchar
scGetUCharValue(sc_value value, sc_status* status);

extern SC_API_EXPORT sc_status
scSetShortValue(sc_value value, sc_short data);

extern SC_API_EXPORT sc_short
scGetShortValue(sc_value value, sc_status* status);

extern SC_API_EXPORT sc_status
scSetUShortValue(sc_value value, sc_ushort data);

extern SC_API_EXPORT sc_ushort
scGetUShortValue(sc_value value, sc_status* status);

extern SC_API_EXPORT sc_status
scSetIntValue(sc_value value, sc_int data);

extern SC_API_EXPORT sc_int
scGetIntValue(sc_value value, sc_status* status);

extern SC_API_EXPORT sc_status
scSetUIntValue(sc_value value, sc_uint data);

extern SC_API_EXPORT sc_uint
scGetUIntValue(sc_value value, sc_status* status);

extern SC_API_EXPORT sc_status
scSetLongValue(sc_value value, sc_long data);

extern SC_API_EXPORT sc_long
scGetLongValue(sc_value value, sc_status* status);

extern SC_API_EXPORT sc_status
scSetULongValue(sc_value value, sc_ulong data);

extern SC_API_EXPORT sc_ulong
scGetULongValue(sc_value value, sc_status* status);

extern SC_API_EXPORT sc_status
scSetFloatValue(sc_value value, sc_float data);

extern SC_API_EXPORT sc_float
scGetFloatValue(sc_value value, sc_status* status);

extern SC_API_EXPORT sc_status
scSetDoubleValue(sc_value value, sc_double data);

extern SC_API_EXPORT sc_double
scGetDoubleValue(sc_value value, sc_status* status);

extern SC_API_EXPORT sc_status
scSetSymbolValue(sc_value value, sc_symbol data);

extern SC_API_EXPORT sc_symbol
scGetSymbolValue(sc_value value, sc_status* status);

extern SC_API_EXPORT sc_type
scGetValueType(sc_value, sc_status* status);

extern SC_API_EXPORT sc_bool
scIsValueEqual(sc_value a, sc_value b, sc_status* status);

/**************************************************************************************************/

SC_EXTERN_C_END

#endif /* __SC_VALUES_H__ */
