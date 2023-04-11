EXECR = bin/receiver.exe
EXECS = bin/sender.exe
objR = obj/receiver.o
objS = obj/sender.o
CC = g++
CPPFLAGS = -std=c++17 -Wall
TESTLIB = -lgtest -lgtest-main
.PHONY = dirs run build clean test report
run: dirs $(EXECR) $(EXECS)
	./$(EXECR)
build: dirs $(EXECR) $(EXECS)
$(EXECR): $(objR)
	$(CC) $(CPPFLAGS) $(objR) -o $(EXECR)
$(EXECS): $(objS)
	$(CC) $(CPPFLAGS) $(objS) -o $(EXECS)
obj/%.o: src/%.cpp
	$(CC) $(CPPFLAGS) -c $< -o $@
dirs:
	-mkdir -p obj bin