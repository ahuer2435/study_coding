PROJECT(blending_op)
CMAKE_MINIMUM_REQUIRED(VERSION 3.10)
#if(COMMAND cmake_policy)　　　　　　　　　　　　  
#      cmake_policy(SET CMP0003 NEW)  
#endif(COMMAND cmake_policy)  
FIND_PACKAGE( OpenCV REQUIRED )
ADD_EXECUTABLE( blending_op  blending_op.cpp )
TARGET_LINK_LIBRARIES( blending_op ${OpenCV_LIBS})


