default rel

section .text
global get_index_by_hash  ; не используется т.к. накладные расходы на вызов
                          ; функции превышают задержку на div

;   edi = hash
;   esi = size
;   rdx = magic_const

get_index_by_hash:
    mov     eax, edi
    mul     rdx

    mov     ecx, esi
    mul     rcx

    mov     eax, edx
    ret