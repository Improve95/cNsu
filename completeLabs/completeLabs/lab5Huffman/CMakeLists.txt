cmake_minimum_required(VERSION 3.5.1)
project(lab5 C)

set(SRC src/main.c)
set(HUFFTREE src/executables/encodeDecode.c src/executables/huffTree.c)
set(ENCODE src/executables/encodeFunction.c)
set(DECODE src/executables/decodeFunctions.c)

add_executable(${PROJECT_NAME} ${SRC} ${HUFFTREE} ${ENCODE} ${DECODE})

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/../modules")
include(common_lab)
