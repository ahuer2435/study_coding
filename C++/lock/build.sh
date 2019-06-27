#! /bin/bash
rm lock_guard_1
g++ lock_guard_1.cpp -lpthread -std=c++11 -o lock_guard_1
rm lock_guard_2
g++ lock_guard_2.cpp -lpthread -std=c++11 -o lock_guard_2
rm mutex_test_1
g++ mutex_test_1.cpp -lpthread -std=c++11 -o mutex_test_1