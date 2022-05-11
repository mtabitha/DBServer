cmake_minimum_required(VERSION 3.20)
project(server_test)

set(CMAKE_CXX_STANDARD 14)

set (CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS} -lpq)

FIND_PACKAGE(PostgreSQL REQUIRED )
IF(PostgreSQL_FOUND)
    INCLUDE_DIRECTORIES(${PostgreSQL_INCLUDE_DIRS})
    LINK_DIRECTORIES(${PostgreSQL_LIBRARY_DIRS})
ENDIF(PostgreSQL_FOUND)

add_executable(DBServer   TestTCPServerMain.cpp
                        src/Session.cpp
                        src/DBServer.cpp
                        src/DataAccessObject.cpp
                        src/AbstractServer.cpp)

#-DPostgreSQL_INCLUDE_DIR=[...]/postgresql/[version]/include