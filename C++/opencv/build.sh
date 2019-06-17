#! /bin/bash
g++ test_mat.cpp `pkg-config --cflags --libs opencv`
