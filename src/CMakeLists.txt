find_package(SDL2 REQUIRED)
find_package(SDL2TTF REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${SDL2_TTF_INCLUDE_DIRS})

add_library(structures vector.c level.c)
target_link_libraries(structures PUBLIC m)

add_executable(${PROJECT_NAME} main.c draw.c)
target_link_libraries(${PROJECT_NAME} PRIVATE ${SDL2_LIBRARIES} ${SDL2TTF_LIBRARY} structures)
