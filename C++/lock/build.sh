#! /bin/bash
rm mutex_test_1
g++ mutex_test_1.cpp -lpthread -std=c++11 -o mutex_test_1
rm lock_guard_1
g++ lock_guard_1.cpp -lpthread -std=c++11 -o lock_guard_1
rm lock_guard_2
g++ lock_guard_2.cpp -lpthread -std=c++11 -o lock_guard_2
rm unique_lock_1
g++ unique_lock_1.cpp -lpthread -std=c++11 -o unique_lock_1
rm unique_lock_hunhe
g++ unique_lock_hunhe.cpp -lpthread -std=c++11 -o unique_lock_hunhe

