PROJECT(discrete_fourier_transform)
CMAKE_MINIMUM_REQUIRED(VERSION 3.10)
#if(COMMAND cmake_policy)　　　　　　　　　　　　  
#      cmake_policy(SET CMP0003 NEW)  
#endif(COMMAND cmake_policy)  
FIND_PACKAGE( OpenCV REQUIRED )
ADD_EXECUTABLE(discrete_fourier_transform  discrete_fourier_transform.cpp)
TARGET_LINK_LIBRARIES(discrete_fourier_transform ${OpenCV_LIBS})


