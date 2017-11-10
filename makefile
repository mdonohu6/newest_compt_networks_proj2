#
# Specify the target
all:  all

#
# Specify teh object files that the target depends on
# Also specify the object files needed to create the executable
all:
	g++ -std=c++0x -pthread -o Driver Driver.cpp
	g++ -std=c++0x -pthread -o cppnodecodeWithThreads cppnodecodeWithThreads.cpp




