PROJECT(file_input_output)
CMAKE_MINIMUM_REQUIRED(VERSION 3.10)
#if(COMMAND cmake_policy)　　　　　　　　　　　　  
#      cmake_policy(SET CMP0003 NEW)  
#endif(COMMAND cmake_policy)  
FIND_PACKAGE( OpenCV REQUIRED )
ADD_EXECUTABLE(file_input_output  file_input_output.cpp)
TARGET_LINK_LIBRARIES(file_input_output ${OpenCV_LIBS})


