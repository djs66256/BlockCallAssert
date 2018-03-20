//
//  BlockCallAssert.h
//  BlockCallAssert
//
//  Created by hzduanjiashun on 2018/3/20.
//  Copyright © 2018年 Daniel. All rights reserved.
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
