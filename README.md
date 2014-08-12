Pillowtalk: CouchDB C API based on libcurl and yajl
===================================================

This library is a basic wrapper around libcurl and yajl that attempts to
provide a more generic interface to couchdb, mainly communicating via url
targets rather than explicit method calls.  More to come...


# Install instructions

## *Xs

1. ```mkdir build; cd build```
2. ```cmake -DCMAKE_BUILD_TYPE=Release ..```
3. ```make```
4. ```make test [optional]```

## Windows
A typical installation will look like (adapated from the YAJL installation
instructions):

1. Click Start > Programs > Microsoft Visual Studio > Visual Studio Tools >
Visual Studio Command Prompt for your version of Visual Studio

  This starts up a Visual studio command prompt. You can check if the compiler
  is in path by typing ```cl /?``` at the  prompt to check.

2. ```cd C:\path\to\pillowtalk\source\```

3. ```mkdir build```

4. ```cd build```

5. ```cmake -G"NMake Makefiles" -DCMAKE_BUILD_TYPE=Release ..```

6. ``` nmake ```


You must have yajl and libcurl installed, and the .dll, .lib, and header files
installed where cmake can find them.  Support for continuous changes feeds
requires pthreads.  This has been tested with the pthread-win32 distribution
(http://sourceware.org/pthreads-win32/).

# Tests/example code

Tests may be run by typing ```make test```.  In addition, the code in the
```test``` directory provides some examples for interacting with this library.

