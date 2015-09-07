============== Description =============
This projet implements subgraph isomorphism algorithms especially suited to process specific graphs that we call sites.
Sites are directed graphs which are rooted (there is a node from which one can reach every other node), in which the number of children of each node is bounded and the children are numbered.
It is aimed at processing control flow graphs obtained by static (or hybrid) analysis of binary programs but does not make specific asumptions (only that the graphs are sites).

============== Compilation =============
cmake .
make

================= Usage ================
./(binary) --help

------------------ SIDT ------------------
Binary: SIDT

Usage:
./SIDT --learn openssl_byte.edg sality_layer1_v4.exe.edg
./SIDT --learn-list learn.lst scan.lst

where scan.lst and learn.lst look like a list of path to .edg files :
sality_layer1_v3.exe.edg
sality_layer3.exe.edg
sality_layer1_v4.exe.edg

Documentation:
See doc/SIDT.pdf (in French)

------------------ GTSI ------------------
Binary: GTSI


-------------- graphbinalgo --------------
Binary: graphbinalgo
