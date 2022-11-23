regexMatcher: regexMatcher.cpp
	g++ -std=c++17 regexMatcher.cpp -o regexMatcher
test:
	./regexMatcher < $(i) > $(o)
.PHONY: test clean
all : regexMatcher test prog3
clean:
	@echo cleaning all files except code, makefile, sample tests
	@find . -type f -not \( -name 'regexMatcher.cpp' -or -name 'makefile' -or -name 'Test1' -or -name 'SampleTestFile.txt' \) -delete