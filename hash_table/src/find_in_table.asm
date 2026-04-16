default rel

section .text
global find_in_table
global strcmp_32

; int strcmp_32(const char* word1, const char* word2)
; rdi = word1
; rsi = word2

strcmp_32:
                vmovdqu ymm0, [rdi]
                vpcmpeqb ymm1, ymm0, [rsi]
                vpmovmskb eax, ymm1
                cmp eax, 0FFFFFFFFh
                jne .not_equal

                vzeroupper
                xor eax, eax
                ret

.not_equal:
                vzeroupper
                mov eax, 1
                ret

TABLE_SIZE      equ     0
TABLE_LIST_ARR  equ     8
TABLE_MAGIC     equ     24

LIST_DATA       equ     0
LIST_NEXT       equ     8

; const char* find_in_table(const table_t* table, const char* word)
;
;   rdi = table
;   rsi = word
;   rax = return value

find_in_table:
                push    r15
                push    r14
                push    r13
                push    r12
                push    rbp
                push    rbx
                sub     rsp, 8                  

                mov     rbx, rdi
                mov     r14, rsi                

                movzx   r13d, byte [r14]

                ; inline crc32c_hash(word)
                mov     r8, r14
                test    r13b, r13b
                jz      .hash_zero

                mov     eax, -1

.hash_loop:
                movzx   edx, byte [r8]
                test    dl, dl
                jz      .hash_done
                crc32   eax, dl
                inc     r8
                jmp     .hash_loop

.hash_done:
                not     eax
                jmp     .have_hash

.hash_zero:
                xor     eax, eax

.have_hash:
                ; index = (((__uint128_t)hash * 2^64 * size) >> 64)
                mul     qword [rbx + TABLE_MAGIC]

                mov     ecx, dword [rbx + TABLE_SIZE]
                mul     rcx

                mov     eax, edx

                mov     rdx, [rbx + TABLE_LIST_ARR]
                mov     rdx, [rdx + rax*8]          

                mov     r15, [rdx + LIST_DATA]
                mov     r12, [rdx + LIST_NEXT]

                mov     ebp, [r12]
                test    ebp, ebp
                jz      .not_found

.search_loop:
                movsxd  rax, ebp
                mov     rdi, [r15 + rax*8]

                ; first-char precheck
                cmp     byte [rdi], r13b
                jne     .advance

                ; full compare
                mov     rsi, r14
                call    strcmp_32
                test    eax, eax
                je      .found

.advance:
                movsxd  rax, ebp
                mov     ebp, [r12 + rax*4]
                test    ebp, ebp
                jnz     .search_loop

.not_found:
                xor     eax, eax
                add     rsp, 8
                pop     rbx
                pop     rbp
                pop     r12
                pop     r13
                pop     r14
                pop     r15
                ret

.found:
                movsxd  rax, ebp
                mov     rax, [r15 + rax*8]
            
                add     rsp, 8
                pop     rbx
                pop     rbp
                pop     r12
                pop     r13
                pop     r14
                pop     r15
                ret