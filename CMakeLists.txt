cmake_minimum_required(VERSION 3.12)
project(online_store LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB SRC_FILES
    src/*.cpp
)

add_executable(online_store ${SRC_FILES})

# Вариант A: libpqxx уже установлена в системе
find_package(libpqxx REQUIRED)
target_link_libraries(online_store PRIVATE pqxx)

# Если find_package не работает, можно заменить на:
# target_link_libraries(online_store PRIVATE pqxx pq)
