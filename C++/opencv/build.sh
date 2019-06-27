#! /bin/bash
rm test_mat_while1 &
g++ test_mat_while1.cpp -o test_mat_while1 `pkg-config --cflags --libs opencv`

rm test_mat_assigne &
g++ test_mat_assigne.cpp -o test_mat_assigne `pkg-config --cflags --libs opencv`
