
all:
	#gcc -o hello -L. -lcontrolcan -lpthread -lusb main.c
	#arm-none-linux-gnueabi-gcc -o hello -L. -L /home/caidunqing/controlcan -lcontrolcan -lpthread main.c
	LIB_OPENCV= `pkg-config opencv --cflags --libs`
	$(CXX) -o system_audio system_audio.cpp $(LIB_OPENCV)

clean:
	rm -f *.o system_audio
