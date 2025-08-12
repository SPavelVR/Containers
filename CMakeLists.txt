cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

project(containers LANGUAGES C)


set(FILES   src/hash.c 
            src/iterator.c
            src/list.c
            src/root.c
            src/node.c
            src/hashTable.c
            src/linkedList.c
            src/string_t.c
            )

# Создаем библиотеку
add_library(${PROJECT_NAME} STATIC ${FILES})  # STATIC для статической, SHARED для динамической

# Указываем где искать заголовочные файлы
target_include_directories(${PROJECT_NAME} PUBLIC include)

# Устанавливаем стандарт C (опционально)
set_property(TARGET ${PROJECT_NAME} PROPERTY C_STANDARD 11)