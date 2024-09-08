cmake_minimum_required(VERSION 3.20.1)
project(chat)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(BOOST_ROOT C:\\Boost)

set(Boost_ON_SYSTEM_PATHS ON)


#find_package(Boost REQUIRED COMPONENTS filesystem regex)

find_package(Boost REQUIRED )



if(Boost_FOUND)
message("EXIT")
aux_source_directory(${PROJECT_SOURCE_DIR}/src SRCLIST)


link_directories(C:\\Windows\\SysWOW64\\mswsock.dll)
link_directories(${PROJECT_SOURCE_DIR}/lib)
include_directories(C:\\Users\\86199\\Desktop\\jscpp\\jsoncpp\\include)
link_directories(${Boost_LIBRARU_DIRS})

include_directories(${PROJECT_SOURCE_DIR}/include ${Boost_INCLUDE_DIRS})


add_executable(${PROJECT_NAME} main.cpp ${SRCLIST})

target_link_libraries(${PROJECT_NAME} PUBLIC ${Boost_LIBRARIES} jsoncpp mswsock ws2_32)





else()
    message("UNEXIT")
endif()




