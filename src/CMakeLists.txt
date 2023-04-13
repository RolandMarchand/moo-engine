find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_library(structures vector.c level.c)
target_link_libraries(structures PUBLIC m)

add_executable(${PROJECT_NAME} main.c)
target_link_libraries(${PROJECT_NAME} PRIVATE ${SDL2_LIBRARIES} structures)
