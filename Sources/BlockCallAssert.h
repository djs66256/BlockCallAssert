//
// BSD 2-Clause License
//
// Copyright (c) 2018, Daniel (djs66256@163.com)
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice, this
// list of conditions and the following disclaimer.
//
// * Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following disclaimer in the documentation
// and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
//         SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//

#ifndef BlockCallAssert_h
#define BlockCallAssert_h

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif
    
extern void *block_call_assert_wrap_block(void *orig_blk, char *message);

typedef void (*block_call_assert_expcetion_handler_t)(const char *);
extern void block_call_assert_set_exception_handler(block_call_assert_expcetion_handler_t handler);

#define BLOCK_CALL_ASSERT(x) ({                 \
    typeof ((x)) blk = x;                       \
    char *message = (char *)malloc(512);        \
    memset(message, 0, 512);                    \
    sprintf(message, "(%s:%d %s)", __FILE__, __LINE__, __FUNCTION__); \
    typeof (blk) ret = (__bridge_transfer typeof(blk))block_call_assert_wrap_block((__bridge void *)blk, message); \
    free(message);                              \
    ret;                                        \
})

#ifdef __cplusplus
} // end extern "C"
#endif
    
#endif /* BlockCallAssert_h */
