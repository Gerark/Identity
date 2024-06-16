emcmake cmake -B build -DCMAKE_CXX_FLAGS="--shell-file ../src/emscripten_shell.html --preload-file ../data"
cd build
emmake make