# toy-dt-cpp
A toy dependently typed language implementation in C++.

### Features

* Base on Martin-Löf's intuitionistic type theory and Normalization by Evaluation.
* Support dependent function type, non-cumulative Russell style universes.
* Support universe level polymorphism.
* Pretty Printer.

### Todo:

* [ ] Module System
* [ ] Unit Test
* [ ] Evaluation Optimization

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