global isr_stub_table
extern exception_handler

%macro isr_exception_stub 1
isr_stub_%+%1:
    call exception_handler
    iretq
%endmacro

isr_exception_stub 0
isr_exception_stub 1
isr_exception_stub 2
isr_exception_stub 3
isr_exception_stub 4
isr_exception_stub 5
isr_exception_stub 6
isr_exception_stub 7
isr_exception_stub 8
isr_exception_stub 9
isr_exception_stub 10
isr_exception_stub 11
isr_exception_stub 12
isr_exception_stub 13
isr_exception_stub 14
isr_exception_stub 15
isr_exception_stub 16
isr_exception_stub 17
isr_exception_stub 18
isr_exception_stub 19
isr_exception_stub 20
isr_exception_stub 21
isr_exception_stub 22
isr_exception_stub 23
isr_exception_stub 24
isr_exception_stub 25
isr_exception_stub 26
isr_exception_stub 27
isr_exception_stub 28
isr_exception_stub 29
isr_exception_stub 30
isr_exception_stub 31

isr_stub_table:
%assign i 0
%rep    32
    dq isr_stub_%+i ; use DQ instead if targeting 64-bit
%assign i i+1
%endrep
