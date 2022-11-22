#  Theory of Computation - Programming Assignment
## Introduction
NFA constructed, converted to DFA, tested

TODO : DFA Minimization

Files
------
- `regexMatcher.cpp` is the file having the code for the project
- `makefile` helps in compilation and testing

How to run
-----------
Run this command to compile and create the object file `regexMatcher`.
```bash
make
```

Then you can test the program by either
* using command line redirection
    ```bash
    ./regexMatcher < path/to/InputFile > path/to/outputfile
    ```
    where `path/to/InputFile` and `path/to/outputfile` may appropriately be modified.


* by using makefile
    ```bash
    make test i=inputfilename o=outputfilename
    ```
    where `inputfilename` and `outputfilename` may appropriately be modified.

* by typing or copy-pasting the testfile contents, but redirecting output (optional)
    ```bash
    ./regexMatcher > path/to/outputfile
    ```

