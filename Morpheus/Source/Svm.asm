.code

; UINT16 __stdcall AsmReadEs()
AsmReadEs PROC
    mov ax, es
    ret
AsmReadEs ENDP

; UINT16 __stdcall AsmReadCs()
AsmReadCs PROC
    mov ax, cs
    ret
AsmReadCs ENDP

; UINT16 __stdcall AsmReadSs()
AsmReadSs PROC
    mov ax, ss
    ret
AsmReadSs ENDP

; UINT16 __stdcall AsmReadDs()
AsmReadDs PROC
    mov ax, ds
    ret
AsmReadDs ENDP

; UINT16 __stdcall AsmReadFs()
AsmReadFs PROC
    mov ax, fs
    ret
AsmReadFs ENDP

; UINT16 __stdcall AsmReadGs()
AsmReadGs PROC
    mov ax, gs
    ret
AsmReadGs ENDP

; UINT16 __stdcall AsmReadTr()
AsmReadTr PROC
    str ax
    ret
AsmReadTr ENDP

; UINT16 __stdcall AsmReadLdtr()
AsmReadLdtr PROC
    sldt ax
    ret
AsmReadLdtr ENDP

; void __stdcall AsmSgdt(void* gdtr)
AsmSgdt PROC
    sgdt [rcx]
    ret
AsmSgdt ENDP

; void __stdcall AsmSidt(void* idtr)
AsmSidt PROC
    sidt [rcx]
    ret
AsmSidt ENDP

END