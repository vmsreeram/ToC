regexMatcher: regexMatcher.cpp
	g++ -std=c++17 regexMatcher.cpp -o regexMatcher
test: regexMatcher
	./regexMatcher < $(i) > $(o)
.PHONY: test clean run
all : regexMatcher test prog3
clean:
	@echo cleaning all files except code, makefile, sample tests
	@find . -type f -not \( -name 'regexMatcher.cpp' -or -name 'run*' -or -name 'Test*' -or -name 'makefile' -or -name 'Test1' -or -name 'testGenerator1.cpp' -or -name 'SampleTestFile.txt' \) -delete

run: regexMatcher
	./regexMatcher < run.in > run.out