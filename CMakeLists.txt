cmake_minimum_required(VERSION 2.6)
project(griso)
CMAKE_POLICY(SET CMP0015 NEW)
set(CMAKE_CXX_FLAGS " -std=c++11")

include_directories(
	./
	src/
	libsSIDT/BFSGraph/
	libsSIDT/decisionTree/
	libsSIDT/Prog/
	libsSIDT/funcInfo/
	libs/include/
	libs/libgraph/
	libsGTSI/
	libsGTSI/Traversal/
	libs/dict/
	libs/dotparser/
)

#shared libs
add_library(graph
      libs/libgraph/graph.h
      libs/libgraph/graph.c
      libs/libgraph/graphIO.h
      libs/libgraph/graphIO.c
      libs/libgraph/node.h
      libs/libgraph/node.c
      libs/libgraph/node_list.h
      libs/libgraph/node_list.c
      libs/libgraph/nodeIO.h
      libs/libgraph/nodeIO.c
      libs/dict/dict.c
      libs/dict/dict.h
)

#add_library(dict 
#      libs/dict/dict.c
#      libs/dict/dict.h
#)

# Dot parser
# Create target for the parser
ADD_CUSTOM_TARGET(simpledotparser echo "Creating dotparser")

# Create custom command for flex/lex (note the outputs)
ADD_CUSTOM_COMMAND(
   OUTPUT Lexer.c
   COMMAND flex
   ARGS    libs/dotparser/Lexer.l
   DEPENDS  libs/dotparser/Lexer.l
   )

# Create custom command for bison/yacc (note the DEPENDS)
ADD_CUSTOM_COMMAND(
   OUTPUT Parser.c
   COMMAND bison
   ARGS -y libs/dotparser/Parser.y
   DEPENDS Lexer.c
   DEPENDS libs/dotparser/Parser.y
   )

# Add parser.c to the list of sources
# SET(Foo_SRCS ${Foo_SRCS} Parser.c)

# Since parser.c does not exists yet when cmake is run, mark
# it as generated
SET_SOURCE_FILES_PROPERTIES(libs/dotparser/Parser.c GENERATED)

#add_library(parser Parser.c Lexer.c)

add_executable(sdp libs/dotparser/main.c Lexer.c Parser.c libs/dotparser/Expression.c)
target_link_libraries(sdp graph)

#SIDT
add_library(decisionTree libsSIDT/decisionTree/decisionTree.c
			 libsSIDT/decisionTree/decisionNode.c  
			 libsSIDT/decisionTree/hashTable.c
)
add_library(BFSGraph libsSIDT/BFSGraph/BFSGraph.c)
add_library(funcInfo libsSIDT/funcInfo/funcInfo.c)
add_library(libprog libsSIDT/Prog/prog.c)

add_library(libSIDT libSIDT.cpp)
add_executable(SIDT SIDT.cpp)
target_link_libraries(SIDT libSIDT graph decisionTree BFSGraph libprog funcInfo)

# GTSI
add_library(libGTSI libGTSI.cpp
      libsGTSI/utils-gtsi.cpp
      libsGTSI/Traversal/Traversal.cpp
)

add_executable(GTSI GTSI.cpp)
target_link_libraries(GTSI libGTSI graph)

# graphbinalgo
add_library(libGraphBinAlgo libGraphBinAlgo.c)

add_executable(graphBinAlgo graphBinAlgo.c)
target_link_libraries(graphBinAlgo libGraphBinAlgo graph pthread)

# tests
add_executable(tests tests.cpp)
target_link_libraries(tests libGTSI libSIDT libGraphBinAlgo graph decisionTree BFSGraph libprog funcInfo pthread)

# edg2dot
add_executable(edg2dot edg2dot.cpp)
target_link_libraries(edg2dot graph)
