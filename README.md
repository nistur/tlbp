TLBP
====

Tiny Little Bit Packer. Small lib for reading and writing buffers, including an interface for trying to pack data as small as possible.

Right now will write 8, 16, 32 and 64 bit signed and unsigned values to the buffer, byte aligned, and has the beginnings of packing 8 bit unsigned to as small as possible. 16, 32 and 64 bit, as well as signed types will follow.

Idea came from reading the SWF file spec.