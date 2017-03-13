SRC=$(wildcard src/*.cpp)
OBJ=$(SRC:.cpp=.o)
CXXFLAGS=-g -std=c++11 -Wall -pedantic
all:main.o $(OBJ)
	g++ -o Physics -lBulletDynamics -lBulletCollision -lGL -lX11 -lGLU -lLinearMath $^



%.o:%.cpp
	g++ -c $^ -I include -I /usr/local/include/bullet -I /usr/include -o $@
