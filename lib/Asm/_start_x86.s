.global _start
.text
_start:
	xor %rbp, %rbp
    andq $-16, %rsp

    movq %rsp, %rdi

    call __rgl_start