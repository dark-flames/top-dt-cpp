# toy-dt-cpp
A toy dependently typed language implementation in C++.

### Features

* Base on Martin-Löf's intuitionistic type theory.
* Support dependent function type, non-cumulative Russell style universes.
* Support universe level polymorphism.

### Build

Dependencies:

* clang, cmake, and make
* flex and bison

```
mkdir build
cd build
cmake ..
make
bin/bin /path/to/input
```