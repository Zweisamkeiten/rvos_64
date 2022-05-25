display/z $a0
display/z $sp
set disassemble-next-line on
watch _tick
b _start
target remote :1234
c
