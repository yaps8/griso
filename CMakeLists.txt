cmake_minimum_required(VERSION 2.6)
project(sigraph)
CMAKE_POLICY(SET CMP0015 NEW)
set(CMAKE_CXX_FLAGS " -std=c++11")

include_directories(
	./
	src/
	libsSIDT/BFSGraph/
	libsSIDT/decisionTree/
	libsSIDT/Prog/
	libsSIDT/funcInfo/
	libs/rb_tree/
	libs/include/
	libs/libgraph/
	libsGTSI/
	libsGTSI/Traversal/
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
)
add_library(rb libs/rb_tree/rb.c)

#SIDT
add_library(decisionTree libsSIDT/decisionTree/decisionTree.c
			 libsSIDT/decisionTree/decisionNode.c  
			 libsSIDT/decisionTree/hashTable.c
)
add_library(BFSGraph libsSIDT/BFSGraph/BFSGraph.c)
add_library(funcInfo libsSIDT/funcInfo/funcInfo.c)
add_library(libprog libsSIDT/Prog/prog.c)

add_executable(SIDT SIDT.cpp)
SET_TARGET_PROPERTIES(SIDT PROPERTIES ENABLE_EXPORTS ON)
target_link_libraries(SIDT graph rb decisionTree BFSGraph libprog funcInfo)

# GTSI
add_executable(GTSI GTSI.cpp
      libsGTSI/utils-gtsi.cpp
      libsGTSI/Traversal/Traversal.cpp
)
SET_TARGET_PROPERTIES(GTSI PROPERTIES ENABLE_EXPORTS ON)
target_link_libraries(GTSI graph rb)

# graphbinalgo
#add_library(libgraphBinAlgo graphBinAlgo.h)
add_executable(graphBinAlgo graphBinAlgo.c)
SET_TARGET_PROPERTIES(graphBinAlgo PROPERTIES ENABLE_EXPORTS ON)
target_link_libraries(graphBinAlgo graph pthread rb)

# tests
add_executable(tests tests.cpp)
link_directories( . )
target_link_libraries(tests graphBinAlgo graph decisionTree BFSGraph libprog funcInfo pthread rb)
