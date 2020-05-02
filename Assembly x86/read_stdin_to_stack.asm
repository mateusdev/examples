; ============================================
; Assembled in x86, Linux, using nasm
; commands:
; $ nasm -f elf32 reader.asm && ld -m elf_i386 reader.o -o reader
; ============================================
;
; Reads a string from stdin from 0 bytes to the maximum space available in the stack
; It took me a day to write this, beyond more and more segmentation faults, and it may be not the best solution, but was a fun challenge! :)
;


section .bss
	buffer: resb 1

section .text
	global _start

l_ret:
	ret

; prints the string buffer pointed by ECX (string needs to be in C-style)
print:
	mov al, [ecx]
	cmp al, 0
	je l_ret

	mov edx, 1
	mov ebx, 1
	mov eax, 4
	int 0x80

	inc ecx
	jmp print


; reads from stdin, places the string in the stack, utilizes an ordernation algorithm to
; ordenate the characters in the correct order and places a pointer to first character in ESI
read_stdin:
	mov ebp, esp	; saves the return address
	push dword 0
	mov esi, esp	; pointer to the end of the string - remember that stacks is LIFO

; loop to read all bytes by read() syscall. If the byte read is 0x10, then it is the end of the string
; at the end we will put the pointer to the first byte of the string in EDI
.loop:
	mov ecx, buffer	; void * ptr
	mov edx, 1		; n bytes
	mov ebx, 0		; fd
	mov eax, 3		; read()
	int 0x80		; syscall

	xor eax, eax
	mov al, [ecx]
	
	sub esp, 1
	mov [esp], al
	
	cmp al, 10
	jne .loop

	mov edi, esp

; because the stack is LIFO, we need to reverse the string, using an ordenation algorithm
; it just swaps two positions of the string, like the example bellow
; \0\nolleH -> H\nolle\0 ->Heoll\n\0 -> Hello\n\0 
.reverse:
	mov eax, esi
	sub esi, edi
	xchg eax, esi	; EAX -> string length, including \0

	mov ebx, 2
	div ebx
	
	xor ecx, ecx
	mov cl, al ; quantity of loops

	sub ecx, 1
	sub esi, 1

; The ordenation algorithm itself ;)
.loop1:
	xor eax, eax
	xor ebx, ebx
	xor edx, edx

	mov al, byte [edi + ecx]

	mov dl, cl
	neg edx

	mov bl, byte [esi + edx]

	mov [esi + edx], al
	mov [edi + ecx], bl
	
	dec ecx
	cmp ecx, 0
	jge .loop1

	; Here, ESI points to the first byte of the ordenated string, and EBP has the return address
	push dword [ebp]
	jmp l_ret

end:
	xor ebx, ebx
	xor eax, eax
	inc al
	int 0x80

_start:
	call read_stdin

	lea ecx, [edi]
	call print
	jmp end
