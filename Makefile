CPP=g++
CPP_FLAGS=-std=c++17 -Wall -Wextra -O3 -Iinclude

SRC=src/main.cpp src/scan.cpp
OBJ=$(SRC:.cpp=.o)
TARGET=gaia-create

all:init $(TARGET)

init: 
	@echo "\n Gaia-Create initialising..."
	@echo "\n--------------------------------------------------------\n"

$(TARGET):$(OBJ)
	$(CPP) $(CPP_FLAGS) -o $(TARGET) $(OBJ)
	@echo "\n--------------------------------------------------------\n"
	@echo "\n Gaia-Create established! Instructions for next steps:"
	@./$(TARGET)
	@echo "--------------------------------------------------------\n"

%.o: %.cpp
	$(CPP) $(CPP_FLAGS) -c $< -o $@

clean:
	rm -f src/*.o $(TARGET)