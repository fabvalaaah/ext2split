# ext2split
Ext2 file-system splitter for Linux.

This is a C NetBeans project, compiled with GCC 5.4.0 and tested with Ubuntu
Linux 16.04.5 LTS x64. No leak detected using Valgrind.

This piece of software is splitting an ext2 file-system dump into blocks and
checks if some blocks are empty (i.e. full of '\0'). The directory containing
extracted blocks and the empty blocks list (report) are created next to the
binary. It provides an easy access to blocks: After their extraction, they can
be accessed and manipulated as usual raw data files. This program should handle
ext3 and ext4 file-system as well (not tested).

Usage example: "./ext2split 4096 ~/dsk.img", will split the ext2 file-system
image "dsk.img" into blocks of 4096 bytes (excepted the first one, number 0,
which is the boot block of a fixed size of 1024 bytes as an ext2 file-system).

DONATION:
As I share these sources for commercial use too, maybe you could consider
sending me a reward (even a tiny one) to my Ethereum wallet at the address
0x1fEaa1E88203cc13ffE9BAe434385350bBf10868
If so, I would be forever grateful to you and motivated to keep up the good work
for sure :oD Thanks in advance !

FEEDBACK:
You like my work? It helps you? You plan to use/reuse/transform it? You have
suggestions or questions about it? Just want to say "hi"? Let me know your
feedbacks by mail to the address fabvalaaah@laposte.net

DISCLAIMER:
I am not responsible in any way of any consequence of the usage of this piece of
software. You are warned, use it at your own risks.