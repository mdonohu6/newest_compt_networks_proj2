#
# Specify the target
all:  driver cppnodecodeWithThreads

#
# Specify teh object files that the target depends on
# Also specify the object files needed to create the executable
driver:
	g++ -std=c++0x -pthread -o Driver Driver.cpp


# Specify how the object files should be created from source files
cppnodecodeWithThreads:	
	g++ -std=c++0x -pthread -o cppnodecodeWithThreads cppnodecodeWithThreads.cpp

