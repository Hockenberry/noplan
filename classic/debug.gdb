set width 0
set height 0
set verbose off

target remote localhost:1234

b kernel_main
b itoa

