
#if __x86_64__

.align 4

.global _block_call_assert_wrap_invoke
_block_call_assert_wrap_invoke:

mov  %rdi, %r10

movq $1, 0x28(%r10)         // called

movq 0x20(%r10), %r11       // block
movq %r11, %rdi

movq 0x10(%r11), %r11        // block->block->invoke

jmp *%r11

#endif

#ifdef __arm64__
.align 4

.global _block_call_assert_wrap_invoke
_block_call_assert_wrap_invoke:

mov x9, x0
add x10, x9, #0x20   // &block
add x11, x9, #0x28   // called

mov x12, #1
str x12, [x11]

ldr x12, [x10]        // block
mov x0, x12
add x12, x12, #0x10 // block->invoke
ldr x12, [x12]

br x12
ret
#endif

#if 0
// #ifdef __arm__
// remove support
.align 2

.global _block_call_assert_wrap_invoke
_block_call_assert_wrap_invoke:

mov r5, r0
add r6, r5, #0x14   // &block
add r7, r5, #0x18   // called

mov r8, #1
str r8, [r7]

ldr r8, [r6]        // block
add r8, r8, #0xb // block->invoke
ldr r8, [r8]
mov r0, r7

br r8
ret
#endif
