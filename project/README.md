# Theory of Computation: Programming Assignment
**Note 1** : The program expects input from STDIN and prints output to STDOUT. Command line input output redirection may be used for file input-output.

**Note 2** : This is the concise version of README for convenience. The detailed version is [here](.Detailed_readme.md).
## Files
- `regexMatcher.cpp` is the program that solves the assignment.
- `run.in` and `run.out` are used when `make run` command is used. 

## How to run
Any of the following ways may be used:
- Compile `regexMatcher.cpp` by running 
    ```bash
    make
    ```
    Then the corresponding executable `regexMatcher` will be generated. It may be tested in any way, one way being input output redirection:
    ```bash
    ./regexMatcher < path/to/testFile > path/to/outFile
    ```
    where `path/to/testFile` and `path/to/outFile` are appropriately set.

- The test file may be pasted into `run.in`. The following commands will execute the test, and the output be written into `run.out`.

    ```bash
    make run
    ```

##
The created executable may be cleaned with this command. Note that any new files added will also be removed.
```bash
make clean
```