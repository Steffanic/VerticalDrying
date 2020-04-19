CCC = g++
OPT = 

make all:VertDry

VertDry: VertDry.cpp
	$(CCC) $(OPT) -o VertDry VertDry.cpp
