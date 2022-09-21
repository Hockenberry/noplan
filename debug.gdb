set width 0
set height 0
set verbose off

target remote localhost:1234

# at entry point - cmd1
b kmain
b itoa

