# BlockCallAssert
When callback block is NEVER called, throw an exception.

# Usage

#### Set exception handler

```oc
void exception_log(const char *str) {
    NSLog(@"%s", str);
}
block_call_assert_set_exception_handler(exception_log);
```

or NSException maybe better for you.

```oc
void exception_log(const char *str) {
    [NSException raise:@"BlockNotCallException" format:@"%s", msg];
}
block_call_assert_set_exception_handler(exception_log);
```

#### Wrap your completion blocks

```oc
- (void)doAsyncWithCompletion:(block_t)completionBlock {
  dispatch_async(..., ^{
//      completionBlock(...)
  });
}

[self doAsyncWithCompletion:BLOCK_CALL_ASSERT(^{
    do_after_completion();
    do_clear();
})];
```

Then if you dont call `completionBlock()`, it will handle an exception.

#### Macros

Maybe you need debug env.

```
#if DEBUG
#import <BlockCallAssert/BlockCallAssert.h>
#define YourBlockCallAssert(x) BLOCK_CALL_ASSERT(x)
#else
#define YourBlockCallAssert(x) x
#endif
```

# LICENSE

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
