# Compiler
CC = g++

# Target executable
TARGET = Block_Properties_Editor.exe

# Compiler flags
CFLAGS = -I./include -c

# Object files
objects = main.o PathGetter.o IdGetter.o WordRegistrar.o PatchExecutor.o BlockIdObj.o OneBlockId.o Tester.o

# Bundles of header files(.h) and implementation files(.cpp)
PathGetter = include/ControlFlow/PathGetter.h lib/ControlFlow/PathGetter.cpp
IdGetter = include/ControlFlow/IdGetter.h lib/ControlFlow/IdGetter.cpp
WordRegistrar = include/ControlFlow/WordRegistrar.h lib/ControlFlow/WordRegistrar.cpp
PatchExecutor = include/ControlFlow/PatchExecutor.h lib/ControlFlow/PatchExecutor.cpp

BlockIdObj = include/Objects/BlockIdObj.h lib/Objects/BlockIdObj.cpp
OneBlockId = include/Objects/OneBlockId.h lib/Objects/OneBlockId.cpp

Config = include/Config.h
Tester = include/Tester.h lib/Tester.cpp

all : $(TARGET)

$(TARGET) : $(objects)
	$(CC) -o $(TARGET) $(objects)

main.o : main.cpp $(PathGetter) $(IdGetter) $(WordRegistrar) $(PatchExecutor) $(BlockIdObj) $(OneBlockId) $(Config) $(Tester)
	$(CC) $(CFLAGS) main.cpp

PathGetter.o : $(PathGetter)
	$(CC) $(CFLAGS) lib/ControlFlow/PathGetter.cpp
IdGetter.o : $(IdGetter) $(BlockIdObj) $(Config)
	$(CC) $(CFLAGS) lib/ControlFlow/IdGetter.cpp
WordRegistrar.o : $(WordRegistrar) $(BlockIdObj) $(OneBlockId) $(Config)
	$(CC) $(CFLAGS) lib/ControlFlow/WordRegistrar.cpp
PatchExecutor.o : $(PatchExecutor) $(OneBlockId) $(BlockIdObj) $(Config)
	$(CC) $(CFLAGS) lib/ControlFlow/PatchExecutor.cpp

BlockIdObj.o : $(BlockIdObj)
	$(CC) $(CFLAGS) lib/Objects/BlockIdObj.cpp
OneBlockId.o : $(OneBlockId) $(BlockIdObj) $(Config)
	$(CC) $(CFLAGS) lib/Objects/OneBlockId.cpp

Tester.o : $(Tester) $(BlockIdObj)
	$(CC) $(CFLAGS) lib/Tester.cpp

run :
	./$(TARGET)

clean :
	del /Q $(TARGET) *.o