cmake_minimum_required(VERSION 2.6)
project(DataBase)
ADD_DEFINITIONS(-std=c++11 )
set(CMAKE_BUILD_TYPE "Debug")
#set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_CXX_FALGS_DEBUG "$ENV{CXXFLAGS} -g -std=c++11")
set(CMAKE_CXX_FALGS_RELEASE "$ENV{CXXFLAGS} ")
message("${CMAKE_BUILD_TYPE}")
add_subdirectory(${PROJECT_SOURCE_DIR}/Data) 
aux_source_directory(./src src)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)#可执行文件输出路径
#set(LIBRARY_OUTPUT_PATH {PROJECT_SOURCE_DIR}/lib)#库文件输出路径
#add_subdirectory(math)
set(LINK_DIR ${PROJECT_SOURCE_DIR}/lib)
include_directories(${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/Data)#添加头文件库
link_directories(${PROJECT_SOURCE_DIR}/lib)


add_executable(${PROJECT_NAME} ${src})
target_link_libraries(${PROJECT_NAME}  mysqlcppconn test) 
#target_link_libraries(${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/lib/mysqlcppconn)
