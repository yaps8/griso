# Description

This projet implements subgraph isomorphism algorithms especially suited to process specific graphs that we call sites.
Sites are directed graphs which are rooted (there is a node from which one can reach every other node), in which the number of children of each node is bounded and the children are numbered.
It is aimed at processing control flow graphs obtained by static (or hybrid) analysis of binary programs but does not make specific asumptions (only that the graphs are sites).

# Compilation
- cmake .
- make

# Binaries
- graphBinAlgo: Ullmann's algorithm implementation
- GTSI: complete site detection algorithm (based on graph traversal)
- SIDT: fast site detection algorithm (with a decision tree)
- tests: unit tests

# Usage
./(binary) --help

## SIDT
Binary: SIDT

Usage:
- ./SIDT --learn file.edg
- ./SIDT --learn-list learn.lst scan.lst

Documentation:
- See doc/SIDT.pdf (in French)
