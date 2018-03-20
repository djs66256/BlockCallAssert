//
//  ViewController.m
//  BlockCallAssert
//
//  Created by hzduanjiashun on 2018/3/20.
//  Copyright © 2018年 Daniel. All rights reserved.
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
