			.thumb

			.text			; Puts code in ROM

			.global asm_main
			.thumbfunc asm_main

asm_main:	.asmfunc		; main

	bx		lr				; return to C program

	        .endasmfunc
	        .end
