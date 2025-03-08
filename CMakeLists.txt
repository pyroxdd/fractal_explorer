cmake_minimum_required(VERSION 3.14)
project(project)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB shaders CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/src/*.glsl"
)
set(fths "C:\\file_to_header_string\\out\\fths")

foreach(shader ${shaders})
    get_filename_component(shader_name ${shader} NAME_WE)
    set(header_name ${CMAKE_BINARY_DIR}/${shader_name}.h)
    add_custom_target(prebuild_${shader_name} ALL
        COMMAND ${fths} ${shader}
    )

    list(APPEND sources ${header_name})
endforeach()

set(OUT "${CMAKE_SOURCE_DIR}/out")
add_compile_options(-fdeclspec)

add_library(includes INTERFACE)

target_include_directories(includes INTERFACE
${CMAKE_SOURCE_DIR}/src/

${CMAKE_BINARY_DIR}#?
)

file(GLOB_RECURSE sources
"${CMAKE_SOURCE_DIR}/src/*.cpp"
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUT})

add_executable(main ${sources})

set_target_properties(main PROPERTIES SUFFIX ".html")

target_link_libraries(main PRIVATE
    includes
)

file(COPY "${CMAKE_SOURCE_DIR}/src/shell.html" DESTINATION "${CMAKE_BINARY_DIR}")

target_link_options(main PRIVATE
    --shell-file shell.html

    -Wno-deprecated-declarations
    -Wno-unused-variable
    -Wno-unused-function

    -sWASM=1
    -sEXPORT_ALL=1

    -sERROR_ON_UNDEFINED_SYMBOLS=0

    -sUSE_SDL=2

    -sUSE_SDL_TTF=2

    -sUSE_WEBGL2=1

    -sMIN_WEBGL_VERSION=2
    -sMAX_WEBGL_VERSION=2

    -sFULL_ES3

    -fPIC
)