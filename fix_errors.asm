.extern transpose_data_chunk
.global isCorrect, find_index_of_errored_row, fix_data_block

.text
isCorrect:
	pushq %rbp
	movq %rsp, %rbp
	mov $0, %eax
	mov $8, %rcx
shift:	
	cmp $0, %rcx
	je end
	shr $1, %rdi
	jnc zero
	inc %eax
zero:
	dec %rcx
	jmp shift
end:
	shr %eax
	jc odd
	mov $1, %eax
	jmp out
odd:
	mov $0, %eax
out:
	leave
	ret

find_index_of_errored_row:
	pushq %rbp
	movq %rsp, %rbp
	pushq %rbx
	pushq %r12
	pushq %r13
	movl $-1, (%rsi)
	mov $0, %r13d
	movq %rdi, %r12
	mov $0, %ebx
loop:
	cmp $8, %ebx
	je end_loop
	movb (%r12,%rbx,1), %dil
	pushq %rdi
	pushq %rsi
	callq isCorrect
	popq %rsi
	popq %rdi
	cmp $1, %eax
	je correct
	inc %r13d
	movl %ebx, (%rsi)
correct:
	inc %ebx
	jmp loop
end_loop:
	mov %r13d, %eax

	popq %r13
	popq %r12		
	popq %rbx
	leave
	ret


.section .data
transposed: .space 8
error_row: .space 4
error_col: .space 4


.section .text
fix_data_block:
  	pushq %rbp
	movq %rsp, %rbp

	leaq (error_row), %rsi			
	pushq %rdi
	callq find_index_of_errored_row
	popq %rdi

	cmpl $1, %eax
	jg unfixable
	
	pushq %rdi
	movq $8, %rsi
	mov $transposed, %rdx
	callq transpose_data_chunk
	popq %rdi 

	mov $error_col, %rsi
	pushq %rdi
	mov $transposed, %rdi
	callq find_index_of_errored_row
	popq %rdi
	cmpl $1, %eax
	jg unfixable
	cmpl $0, %eax
	je no_error
	
	mov $128, %al
	mov (error_col), %cl
	shr %cl, %al
	
	mov (error_row), %edx
	xorb %al, (%rdi, %rdx, 1)
	mov $2, %eax
	jmp end_fix

unfixable:
	movl $0, %eax
	jmp end_fix
no_error:
	movl $1, %eax
end_fix:

	leave
	ret

