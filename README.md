CubeDemo
========

Demo written in C++ and OpenGL 3.3. This demonstrates some fairly basic rendering techniques, such as:

* Per-pixel ADS lighting
* Instanced geometry rendering using Texture Buffer Objects
* Gamma correction in the fragment shader

How to build
------------

I use [CMake](http://www.cmake.org/) as the build system. A typical build process on a Unix system looks like this:

Clone the repository (recursively to include submodules glm and glfw):

    $ git clone --recursive https://github.com/rthome/CubeDemo.git cubedemo

Create a build directory, like so for example:

    $ mkdir build
    $ cd build

Generate Makefiles or whatever is appropriate for your system with CMake:

    $ cmake ../cubedemo

Build!

    $ make

Run!

    $ ./CubeDemo

On Windows, download CMake, generate a Visual Studio solution, and use Visual Studio to build the program.
