PROJECT(image_convert_test)
CMAKE_MINIMUM_REQUIRED(VERSION 3.10)
#if(COMMAND cmake_policy)　　　　　　　　　　　　  
#      cmake_policy(SET CMP0003 NEW)  
#endif(COMMAND cmake_policy)  
FIND_PACKAGE( OpenCV REQUIRED )
ADD_EXECUTABLE(image_convert_test  image_convert_test.cpp)
TARGET_LINK_LIBRARIES(image_convert_test ${OpenCV_LIBS})


