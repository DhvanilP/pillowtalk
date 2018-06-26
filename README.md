Pillowtalk: CouchDB C API based on libcurl and yajl
===================================================

This library is a basic wrapper around libcurl and yajl that attempts to
provide a more generic interface to couchdb, mainly communicating via url
targets rather than explicit method calls.


# Install instructions

## *Xs

1. ```mkdir build; cd build```
2. ```cmake -DCMAKE_BUILD_TYPE=Release ..```
3. ```make```
4. ```make test [optional]```
5. ```sudo make install```



# Tests/example code

Tests may be run by typing ```make test```.  In addition, the code in the
```test``` directory provides some examples for interacting with this library.

