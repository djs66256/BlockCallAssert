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

#import "ViewController.h"
#import <BlockCallAssert.h>

@interface ViewController ()

@end

void exception_log(const char *str) {
    NSLog(@"%s", str);
}

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
    block_call_assert_set_exception_handler(exception_log);
    
    [self test1];
    [self test2];
    [self test3];
    [self test4];
}

- (void)test1 {
    [self testBlock1:BLOCK_CALL_ASSERT(^{
        NSLog(@"%@ was called!", NSStringFromSelector(_cmd));
    })];
}
- (void)test2 {
    [self test2:BLOCK_CALL_ASSERT(^{
        NSLog(@"%@ was called!", NSStringFromSelector(_cmd));
    })];
}
- (void)test3 {
    [self test3:BLOCK_CALL_ASSERT(^int(int a, int b, int c, NSString *str, CGPoint point){
        NSLog(@"%@ was called! %d, %d, %d, %@, %@",
              NSStringFromSelector(_cmd),
              a, b, c, str, NSStringFromCGPoint(point));
        return 8;
    })];
}
- (void)test4 {
    [self test4:BLOCK_CALL_ASSERT(^int(int a, int b, int c, NSString *str, CGPoint point){
        NSLog(@"%@ was called! %d, %d, %d, %@, %@",
              NSStringFromSelector(_cmd),
              a, b, c, str, NSStringFromCGPoint(point));
        return 8;
    })];
}

- (void)testBlock1:(dispatch_block_t)block {
    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(1 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
        block();
    });
}

- (void)test2:(dispatch_block_t)block {
    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(1 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
        __unused dispatch_block_t tmp = block;
    });
}

- (void)test3:(int(^)(int a, int b, int c, NSString *str, CGPoint point))block {
    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(1 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
        int res = block(1, 2, 3, @"my string", CGPointMake(4, 5));
        NSLog(@"return %d", res);
    });
}

- (void)test4:(int(^)(int a, int b, int c, NSString *str, CGPoint point))block {
    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(1 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
        __unused id tmp = block;
    });
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}


@end
