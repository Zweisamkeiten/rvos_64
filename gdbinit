display/z $a0
display/z $sp
set disassemble-next-line on
b _start
target remote :1234
c
