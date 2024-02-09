CXX = g++
CXXFLAGS = -g -std=c++17
CALCULATOR_TESTS_SRCS = tests/calc_test.cc
CALCULATOR_HEADERS = SmartCalcV2/CalcModel.h
CALCULATOR_SRC = SmartCalcV2/CalcModel.cc \
				SmartCalcV2/Token.cc
APP_ARCHIVE = Archive_SmartCalc_v2.0
OS := $(shell uname)

ifeq ($(OS),Linux)
TEST_FLAGS = -lgtest -lm -lpthread -lrt -lsubunit -fprofile-arcs -ftest-coverage
else
TEST_FLAGS = -lgtest -lm -lpthread -fprofile-arcs -ftest-coverage
endif

all: install

install: uninstall
	mkdir build
	@cd build && qmake -r ../SmartCalcV2/view/view.pro && make
	@rm build/*.cpp
	@rm build/*.o
	@rm build/*.h
	@rm build/*Makefile

uninstall:
	@rm -rf build

dist:
	@rm -rf $(APP_ARCHIVE)/
	@mkdir $(APP_ARCHIVE)
	@mkdir $(APP_ARCHIVE)/src/
	@cp -r build/view $(APP_ARCHIVE)/src/view
	@tar -cvzf $(APP_ARCHIVE).tgz $(APP_ARCHIVE)/
	@rm -rf $(APP_ARCHIVE)

tests: $(CALCULATOR_TESTS_SRCS) $(CALCULATOR_HEADERS) $(CALCULATOR_SRC)
	@$(CXX) $(CXXFLAGS) -c $(CALCULATOR_TESTS_SRCS) $(CALCULATOR_SRC)
	@$(CXX) $(CXXFLAGS) *.o $(TEST_FLAGS) -o ./unit_tests
	@./unit_tests

leaks: tests
	CK_FORK=no leaks --atExit -- ./unit_tests

dvi:
	@open documentation.md

clean:
	@rm -rf build
	@rm -rf $(APP_ARCHIVE).tgz
	@rm ./unit_tests
	@rm -rf unit_tests.dSYM
	# @rm SmartCalcV2/*.o
	# @rm SmartCalcV2/.gch
	# @rm SmartCalcV2/CalcModel.h.gch
	rm -rf *.o
	@rm *.gcno
	@rm *.gcda

check:
	@clang-format -style=Google -n SmartCalcV2/*.h SmartCalcV2/*.cc SmartCalcV2/view/*.h SmartCalcV2/view/*.cc 
