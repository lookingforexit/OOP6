cmake_minimum_required(VERSION 3.10)
project(myProgram)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Добавление опций компиляции
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror=maybe-uninitialized -Wall -Wextra")

enable_testing()

# Установка Google Test
include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.15.0
        TLS_VERIFY false
)

FetchContent_MakeAvailable(googletest)

# Создаем список исходных файлов для библиотеки
set(LIB_SOURCES
        src/Arena.cpp
        src/Factory.cpp
        src/NPC.cpp
        src/Observer.cpp
        src/Visitor.cpp
)

# Создаем библиотеку из исходных файлов
add_library(${PROJECT_NAME}_lib STATIC ${LIB_SOURCES})

# Указываем include-директории для библиотеки
target_include_directories(${PROJECT_NAME}_lib PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Создаем основное приложение
add_executable(${PROJECT_NAME} src/main.cpp)

# Связываем приложение с нашей библиотекой
target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}_lib)


# Создаем исполняемый файл для тестов
add_executable(${PROJECT_NAME}_tests tests/tests.cpp)

# Связываем тесты с нашей библиотекой и Google Test
target_link_libraries(${PROJECT_NAME}_tests PRIVATE
        ${PROJECT_NAME}_lib
        gtest_main
)

# Указываем include-директории для тестов
target_include_directories(${PROJECT_NAME}_tests PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Добавление тестов в тестовый набор
add_test(NAME ${PROJECT_NAME}_Tests COMMAND ${PROJECT_NAME}_tests)