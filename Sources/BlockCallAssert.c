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

#include "BlockCallAssert.h"
#include <stdlib.h>
#include <string.h>
#include <Block.h>

#if __x86_64__ || __arm__ || __arm64__

#define WRAP_BLOCK_LOG_ENABLE false
#if WRAP_BLOCK_LOG_ENABLE
#define WRAP_BLOCK_LOG(...) printf(__VA_ARGS__)
#else
#define WRAP_BLOCK_LOG(...)
#endif

static block_call_assert_expcetion_handler_t exception_handler;
void block_call_assert_set_exception_handler(block_call_assert_expcetion_handler_t handler) {
    exception_handler = handler;
}

struct Block_layout {
    void *isa;
    volatile int32_t flags; // contains ref count
    int32_t reserved;
    void (*invoke)(void);
    void *descriptor;
    
    // imported variables
    void *block;
    int64_t called;
    char *message;
};

struct Descriptor {
    uintptr_t reserved;
    uintptr_t size;
    void (*copy)(void *dst, const void *src);
    void (*dispose)(const void *);
};

/*
// do we need copy?
void block_call_assert_wrap_copy(void *dst, const void *src) {
    struct Block_layout *dstBlock = (struct Block_layout *)dst;
    struct Block_layout *srcBlock = (struct Block_layout *)src;
    memcpy(dst, src, sizeof(struct Block_layout));
    
    dstBlock->block = Block_copy(srcBlock->block);
    
    size_t len = sizeof(srcBlock->message)*sizeof(char);
    char *buf = (void *)malloc(len);
    memcpy(buf, srcBlock->message, len);
    dstBlock->message = buf;
}
 */

void block_call_assert_wrap_dispose(const void * ptr) {
    struct Block_layout *self = (struct Block_layout *)ptr;
    WRAP_BLOCK_LOG("dispose wrap block! \n");
    if (!((struct Block_layout *)ptr)->called) {
        WRAP_BLOCK_LOG("ERROR: Block must be called at %s!\n", self->message ?: "");
        if (exception_handler) {
            if (self->message) {
                char *buf = (char *)malloc((strlen(self->message) + 64) * sizeof(char));
                sprintf(buf, "ERROR: Block must be called at %s!\n", self->message);
                exception_handler(buf);
                free(buf);
            }
            else {
                exception_handler("ERROR: Block must be called at %s!\n");
            }
        }
    }
    Block_release(self->block);
    if (self->message) free(self->message);
}

static const struct Descriptor descriptor = {
    0,
    sizeof(struct Block_layout),
    NULL,
    block_call_assert_wrap_dispose
};

extern void *_NSConcreteMallocBlock[32];
extern void block_call_assert_wrap_invoke(void *p);

void *block_call_assert_wrap_block(void *orig_blk, char *message) {
    struct Block_layout *block = (struct Block_layout *)malloc(sizeof(struct Block_layout));
    block->isa = _NSConcreteMallocBlock;
    
    enum {
        BLOCK_NEEDS_FREE = (1 << 24),
        BLOCK_HAS_COPY_DISPOSE = (1 << 25),
    };
    const unsigned retainCount = 1;
    
    block->flags = BLOCK_HAS_COPY_DISPOSE | BLOCK_NEEDS_FREE | (retainCount << 1);
    block->reserved = 0;
    block->invoke = (void (*)(void))block_call_assert_wrap_invoke;
    block->descriptor = (void *)&descriptor;
    
    block->block = (void *)Block_copy(orig_blk);
    block->called = 0;
    
    size_t len = strlen(message)*sizeof(char);
    char *buf = (char *)malloc(len);
    memcpy(buf, message, len);
    block->message = buf;
    
    return block;
}
#else
void *block_call_assert_wrap_block(void *orig_blk, char *message) {
    // we don't support i386
    return orig_blk;
}
#endif
