PROJECT(Mat_test)
CMAKE_MINIMUM_REQUIRED(VERSION 3.10)
#if(COMMAND cmake_policy)　　　　　　　　　　　　  
#      cmake_policy(SET CMP0003 NEW)  
#endif(COMMAND cmake_policy)  
FIND_PACKAGE( OpenCV REQUIRED )
ADD_EXECUTABLE(Mat_test  Mat_test.cpp)
TARGET_LINK_LIBRARIES(Mat_test ${OpenCV_LIBS})


