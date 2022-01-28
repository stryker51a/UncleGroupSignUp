cmake_minimum_required(VERSION 3.20)
project(UncleGroupSignUp)

# Set compiler flags
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wextra -pedantic -pedantic-errors -g")


set(SOURCE_FILES
        main.cpp
        Person.h
        Person.cpp
        member.cpp
        member.h
        leader.cpp
        leader.h
        )

add_executable(UncleGroupSignUp ${SOURCE_FILES})

set(SOURCE_FILES2
        main.cpp
        )

add_executable(Testing ${SOURCE_FILES2})

set(SOURCE_FILES3
        main.cpp
        Person.cpp
        Person.h
        )

add_executable(TestingPerson ${SOURCE_FILES3})