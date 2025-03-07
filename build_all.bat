call C:\emsdk\emsdk_env.bat
cmake -B build-wasm -G "Unix Makefiles" -DCMAKE_TOOLCHAIN_FILE="C:/emsdk/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake" -DCMAKE_BUILD_TYPE=Release

cmake --build build-wasm