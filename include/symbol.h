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

#ifndef __CE_SYMBOL_H__
#define __CE_SYMBOL_H__

/**************************************************************************************************/

#include "compute.h"
 
/**************************************************************************************************/

CE_EXTERN_C_BEGIN

/**************************************************************************************************/

extern CE_API_EXPORT ce_symbol
ceCreateSymbol(ce_session session, const char* name, size_t length);

extern CE_API_EXPORT void 
ceReleaseSymbol(ce_symbol symbol);

extern CE_API_EXPORT const char*
ceGetSymbolName(ce_symbol symbol);

extern CE_API_EXPORT ce_bool
ceIsSymbolNameEqual(ce_symbol a, ce_symbol b);

extern CE_API_EXPORT ce_uint
ceGetSymbolHash(ce_symbol symbol);

extern CE_API_EXPORT size_t
ceGetSymbolLength(ce_symbol symbol);

/**************************************************************************************************/

CE_EXTERN_C_END

#endif /* __CE_SYMBOL_H__ */
