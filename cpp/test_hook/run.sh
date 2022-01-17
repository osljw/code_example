set -e
g++ -std=c++11 main.cpp vfunc_hook.cpp  -o test_hook
./test_hook
