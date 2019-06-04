# [cadexchanger](https://cadexchanger.com/) [interview](https://cadexchanger.com/download/documents/hiring/CADEX%20C++%20assignment%20v4%20for%20probationers.pdf)


Design a small program in C++ that would implement the following:
1. Support a few types of 3D geometric curves – circles, ellipses and 3D helixes. (Simplified
   definitions are below). Each curve should be able to return a 3D point and a first derivative (3D
   vector) per parameter t along the curve.
2. Populate a container (e.g. vector or list) of objects of these types created in random manner with
   random parameters. Ensure that the container will contain objects of all supported types.
3. Print coordinates of points and derivatives of all curves in the container at t=PI/4.
4. Populate a second container that would contain only circles from the first container. Make sure the
   second container shares (i.e. not clones) circles of the first one, e.g. via pointers.
5. Sort the second container in the ascending order of circles’ radii. That is, the first element has the
   smallest radius, the last - the greatest.
6. Compute the total sum of radii of all curves in the second container
7. (Additional) Split implementation into a library of curves and executable which uses API of this library.

Expectations to the implementation:
1. The implementation must use virtual methods.
2. Has neither explicit memory deallocation nor memory leaks.
3. Curves must be physically correct (e.g. radii must be positive).
4. Containers and sorting must be implemented using STL (C++ Standard Template Library).
5. The implementation may use constructs of C++11 or higher.
6. The code must compile with any compiler of your choice (gcc, Visual C++, etc).
7. The result should be sent via email or posted on github. The delivery must contain source code only
   (no compiled binaries): *.h and *.cpp files and project files, so that we can compile the result by
   ourselves

Curve definitions:
- All curves are parametrically defined, i.e. a point is calculated using some C(t) formula.
- Circle is planar in the plane XoY (i.e. all Z-coordinates are 0) and is defined by its radius.
- Ellipse is planar in the plane XoY and is defined by its two radii, along X and Y axes.
- Helix is spatial and is defined by its radius and step (see the figure below). It takes 2 * PI in
  parametric space to make a round, i.e. any point on helix satisfies the condition C(t + 2*PI) = C(t) +
  {0, 0, step}.
 
#### Supported compiler: gcc, clang
 
#### Supported OS: Linux x64

#### Depends on: C++ compiler, CMake

#### Build & Run
```bash
mkdir cmake-build-gcc && cd cmake-build-gcc \
 && /sbin/cmake -DCMAKE_CXX_COMPILER=/sbin/g++ -G "CodeBlocks - Unix Makefiles" .. \
 && /sbin/cmake --build . --target mcl_test \
 && ./mcl_test
```

#### Example output
[mcl_test.out](./mcl_test.out)

#### Author: Ivan Stepanov 

#### License: [The Unlicense](./LICENSE.md)

# Code review notes

1. Base class that have derived classes should have virtual destructor (`-Werror=non-virtual-dtor`). Use virtual destructor everywhere.
1. Prefer dynamic_cast instead of typeid. Dynamic cast will not work properly w/o virtual destructor.
1. Use [dynamic_pointer_cast](https://en.cppreference.com/w/cpp/memory/shared_ptr/pointer_cast) instead of `get`.
1. Use of `str` method inside `operator <<` is overhead. Creating new instances of stringstream causes performance drop.
1. Use `make_shared` whenever when possible.
1. `uniform_real_distribution` can return zero (Use some kind of epsilon, e.g. [subnormal value](https://en.wikipedia.org/wiki/Denormal_number)).
1. Compiler flags should be platform-independent
