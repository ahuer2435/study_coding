PROJECT(how_to_scan_images)
CMAKE_MINIMUM_REQUIRED(VERSION 3.10)
#if(COMMAND cmake_policy)　　　　　　　　　　　　  
#      cmake_policy(SET CMP0003 NEW)  
#endif(COMMAND cmake_policy)  
FIND_PACKAGE( OpenCV REQUIRED )
ADD_EXECUTABLE(how_to_scan_images  how_to_scan_images.cpp)
TARGET_LINK_LIBRARIES(how_to_scan_images ${OpenCV_LIBS})


