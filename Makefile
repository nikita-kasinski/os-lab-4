EXECR = bin/receiver.exe
EXECS = bin/sender.exe
objR = obj/receiver.o
objS = obj/sender.o
obj = obj/controller.o
INCLUDE = -Iinclude
CC = g++
CPPFLAGS = -std=c++17 -Wall $(INCLUDE)
TESTLIB = -lgtest -lgtest-main
.PHONY = dirs run build clean test report
run: dirs $(EXECR) $(EXECS)
	./$(EXECR)
build: dirs $(EXECR) $(EXECS)
$(EXECR): $(objR) $(obj)
	$(CC) $(CPPFLAGS) $(objR) $(obj) -o $(EXECR)
$(EXECS): $(objS) $(obj)
	$(CC) $(CPPFLAGS) $(objS) $(obj) -o $(EXECS)
obj/%.o: src/%.cpp
	$(CC) $(CPPFLAGS) -c $< -o $@
dirs:
	-mkdir -p obj bin
clean:
	-rm bin/*
	-rm obj/*