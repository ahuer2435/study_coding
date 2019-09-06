TARGET = AdasBox
#CC= g++
#Multi_Thread = -lpthread
SRC_CPP = $(wildcard ./HMI/src/*.cpp\
			./adas_perception/src/*.cpp\
			./PolyM/src/*.cpp)
SRC_CPP += ./mainThread.cpp
SRC_CPP += ./common/include/CommonParam.cpp
LIB_OPENCV= `pkg-config opencv --cflags --libs`

OBJ1 = $(SRC_CPP:.cpp=.o)
OBJ = $(OBJ1:.cc=.o)

INCPATH= -I"/usr/local/include"\
     	  -I"./HMI/include/"\
     	  -I"./adas_perception/include/"\
		  -I"./common/include"\
		  -I"./PolyM/include/"\
		  -I"./syslayer/include"

LIBPATH = $(LIB_OPENCV)\
          -L"../ThirdParty/PolyM/lib/" \
          -L"/opt/opencv3.4/lib"\
		  -L"/usr/local/lib"


LIB   = -lpthread\
		-lstdc++\
		-lm\
		-lgomp -lboost_log -lboost_thread  -lboost_system -lboost_log_setup
COMMON_FLAGS = -Wall -O3 -std=c++14 -DX86 $(INCPATH)
COMMON_FLAGS2 = -Wall -O3 -std=c++11 -DX86 $(INCPATH)
all:$(TARGET)
	
debug:
	@echo -------SRC_CPP:$(SRC_CPP)---------------
	@echo -------OBJ:$(OBJ)-----------------------
	@echo -------LibPath:$(LIBPATH)-----------------------
%.o:%.cpp
	$(CC) -c -o $@ $< $(COMMON_FLAGS) $(LIBPATH)
%.o:%.cc
	$(CC) -c -o $@ $< $(COMMON_FLAGS2) $(LIBPATH) 
$(TARGET):$(OBJ)
	$(CC) -o $@ $^ $(LIB) $(LIBPATH)


.PHONY:clean
clean:
	rm -f $(OBJ) $(TARGET) log.txt *.log
cleanlog:
	rm -f *.log



