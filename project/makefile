regexMatcher: regexMatcher.cpp
	g++ -std=c++17 regexMatcher.cpp -o regexMatcher

.PHONY: test clean run
all : regexMatcher test prog3
clean:
	@echo cleaning all files except code, makefile, sample tests
	@find . -type f -not \( -name 'regexMatcher.cpp' -or -name 'run*' -or -name 'Test*' -or -name 'makefile' -or -name 'compareop.sh' -or -name 'testGenerator1.cpp' -or -name 'SampleTestFile.txt' -or -name 'README.md' \) -delete

run: regexMatcher
	./regexMatcher < run.in > run.out

gener: testGenerator1.cpp
	g++ -std=c++17 testGenerator1.cpp -o gener

MYDIR = tests
test: $(MYDIR)/*
	@rm -rf testouts
	@mkdir testouts
	@mkdir testouts/tests
	for file in $^ ; do \
		./regexMatcher < $${file} > testouts/$${file};\
		echo $${file};\
	done