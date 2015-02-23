	.file	"toSeeAssembly.c"
	.text
	.globl	sum
	.type	sum, @function
sum:
.LFB24:
	.cfi_startproc
	leal	(%rdi,%rsi), %eax
	addl	%eax, accum(%rip)
	ret
	.cfi_endproc
.LFE24:
	.size	sum, .-sum
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"%d"
	.text
	.globl	main
	.type	main, @function
main:
.LFB25:
	.cfi_startproc
	subq	$40, %rsp
	.cfi_def_cfa_offset 48
	movq	%fs:40, %rax
	movq	%rax, 24(%rsp)
	xorl	%eax, %eax
	addl	$3, accum(%rip)
	movl	$3, %r8d
	movl	$.LC0, %ecx
	movl	$15, %edx
	movl	$1, %esi
	movq	%rsp, %rdi
	call	__sprintf_chk
	movq	24(%rsp), %rax
	xorq	%fs:40, %rax
	je	.L3
	call	__stack_chk_fail
.L3:
	addq	$40, %rsp
	.cfi_def_cfa_offset 8
	.p2align 4,,2
	ret
	.cfi_endproc
.LFE25:
	.size	main, .-main
	.globl	accum
	.bss
	.align 4
	.type	accum, @object
	.size	accum, 4
accum:
	.zero	4
	.ident	"GCC: (Ubuntu 4.8.2-19ubuntu1) 4.8.2"
	.section	.note.GNU-stack,"",@progbits
