PROJECT(mat_mask_operationss)
CMAKE_MINIMUM_REQUIRED(VERSION 3.10)
#if(COMMAND cmake_policy)　　　　　　　　　　　　  
#      cmake_policy(SET CMP0003 NEW)  
#endif(COMMAND cmake_policy)  
FIND_PACKAGE( OpenCV REQUIRED )
ADD_EXECUTABLE( mat_mask_operationss  mat_mask_operations.cpp )
TARGET_LINK_LIBRARIES( mat_mask_operationss ${OpenCV_LIBS})


