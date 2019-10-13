message(" includ data")
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
include_directories(${PROJECT_SOURCE_DIR}/include)
aux_source_directory(. sour_list)
add_library(test SHARED ${sour_list})
SET_TARGET_PROPERTIES(test PROPERTIES VERSION 1.2 SOVERSION 1)

