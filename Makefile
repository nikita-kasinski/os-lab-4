EXECR = bin/receiver.exe
EXECS = bin/sender.exe
TEXEC = bin/test.exe
objR = obj/receiver.o
objS = obj/sender.o
obj = obj/controller.o
objT = obj/test.o obj/tcontroller.o
CC = g++
CPPFLAGS = -std=c++17 -Wall -Iinclude
TESTLIB = -lgtest -lgtest_main -lgcov
COVERAGE = -fprofile-arcs -ftest-coverage
.PHONY = dirs run build clean test report clean_coverage
# run and build recipes
run: dirs $(EXECR) $(EXECS)
	./$(EXECR)
build: dirs $(EXECR) $(EXECS)
$(EXECR): $(objR) $(obj)
	$(CC) $(CPPFLAGS) $(objR) $(obj) -o $(EXECR)
$(EXECS): $(objS) $(obj)
	$(CC) $(CPPFLAGS) $(objS) $(obj) -o $(EXECS)
obj/%.o: src/%.cpp
	$(CC) $(CPPFLAGS) -c $< -o $@
# test recipes
report: clean_coverage test
	-lcov -c --directory obj --output-file coverage.info
	-lcov --extract coverage.info *\src\* -o coverage.info
	-genhtml coverage.info -o cov
	-rm coverage.info
	start cov/index.html
test: dirs $(TEXEC)
	./$(TEXEC)
$(TEXEC): $(objT)
	$(CC) $(CPPFLAGS) $(objT) -o bin/test.exe $(TESTLIB)
obj/%.o: test/%.cc
	$(CC) $(CPPFLAGS) -c $< -o $@ $(TESTLIB)
obj/t%.o: src/%.cpp
	$(CC) $(CPPFLAGS) $(COVERAGE) -c $< -o $@
# additional recipes
dirs:
	-mkdir -p obj bin
clean:
	-rm bin/*
	-rm obj/*
	-rm -fr cov/*
clean_coverage:
	-rm obj/*.gcda