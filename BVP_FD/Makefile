CXX = g++ -fconcepts -std=c++17
CXXFLAGS = -c
LAPACKFLAGS = -llapacke -llapack -lcblas  -lgfortran -no-pie
SRC_DIR = ./src
OBJ_DIR = ./obj
SRC = $(wildcard $(SRC_DIR)/*.cpp)
OBJ = $(patsubst $(SRC_DIR)/%.cpp, %.o, $(SRC))
VPATH = $(SRC_DIR)
vpath %.o $(OBJ_DIR)

%.o: %.cpp
	@$(CXX) $(CXXFLAGS)  $<  -o $(OBJ_DIR)/$@
run:$(OBJ)
	@$(CXX) $(CXXFLAGS) ./main/main.cpp -o ./main/main.o
	@$(CXX) ./main/main.o $(addprefix $(OBJ_DIR)/, $(OBJ)) -o ./main/main $(LAPACKFLAGS)
	@./main/main

.PHONY: clean test all
clean:
	@rm -f $(OBJ_DIR)/*.o
test: $(OBJ)
	@$(CXX) $(CXXFLAGS) ./test/test.cpp -o ./test/test.o 
	@$(CXX) ./test/test.o $(addprefix $(OBJ_DIR)/, $(OBJ)) -o ./test/test $(LAPACKFLAGS)
	@./test/test

