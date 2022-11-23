# Theory of Computation: Programming Assignment
## Files
- `regexMatcher.cpp` is the file that solves the assignment.
- `testGenerator1.cpp` is used to help in generating test files.
- `run.in` and `run.out` are used when `make run` command is used.

## How to run
Any of the following ways may be used.
1. Compile `regexMatcher.cpp` by running 
    ```bash
    make
    ```
    Then its compiled object file `regexMatcher` will be generated. It may be tested in any way, one way being input output redirection:
    ```bash
    ./regexMatcher < path/to/inFile > path/to/outFile
    ```
    where `path/to/inFile` and `path/to/outFile` are appropriately set.

2. The test file is pasted into `run.in`. The following commands will execute the test, and the output be written into `run.out`.
    ```bash
    make run
    ```
The created object files may be cleaned with 
```bash
make clean
```