TARGET = TestMsgQueue
CC= g++
SRC = testSaveData.cpp 
OBJ = $(SRC:.cpp=.o)
INCPATH = -I../../include\
          -I../../../common/include/\
          -I../../../ThirdParty/PolyM/include\
		  -I/opt/opencv3.4/include

#LIB_OPENCV = 'pkg-config opencv --cflags --libs'
LIB_OPENCV  = `pkg-config opencv --cflags --libs`
LIBPATH = $(LIB_OPENCV)\
          -L"../../../ThirdParty/PolyM/lib/"\
		  -L"/usr/local/lib"

LIB = -l"polym" -lpthread -lboost_log  -lboost_thread  -lboost_system -lboost_log_setup

COMMON_FLAGS = -Wall -std=c++14
$(TARGET):$(OBJ)
	$(CC) -o  $@ $^ $(LIB) $(LIBPATH) $(COMMON_FLAGS)
$(OBJ):$(SRC)
	$(CC) -c -o $@ $< $(INCPATH) $(COMMON_FLAGS)
clean:
	rm -f $(TARGET) $(OBJ) *.dat *.log