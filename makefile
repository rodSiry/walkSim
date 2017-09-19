SRC=$(wildcard src/*.cpp)
OBJ=$(SRC:.cpp=.o)
LIBS=-lGL -lGLU -lGLEW -lILU -lIL -lSDL2 -lBulletDynamics -lBulletCollision  -lLinearMath 
CXXFLAGS=-g -std=c++11 -Wall -pedantic 
all:main.o $(OBJ)
	g++ -o Programme -L/usr/local/lib  $(LIBS) $^
%.o:%.cpp
	g++ -c $^ -I include -I /usr/include -I /usr/local/include/bullet -o $@
