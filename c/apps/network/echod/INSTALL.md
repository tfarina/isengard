## Getting Started

***Important***: The following instructions assume that you have created a
`build` subdirectory and that subsequent commands are performed from this
directory, like the following:

```sh
mkdir build && cd build
```

## CMake Basics

CMake isn't actually a built system. It's a meta-build system. In other words,
CMake is a build system *generator*.

On Unix systems, CMake generates Makefiles by default, just like Autotools.
On Windows, it generates Visual Studio projects by default.

The process for using CMake is very similar to Autotools:
1. *Configure*: Generate the build system.
2. *Build*: Compile the project.
3. *Install*: Install to the "prefix" directory.

### CMake Generators

You can choose to use a different generator using the `-G` option.
For example, on macOS you can generate Xcode projects.

Ninja is a popular build system, available on both Unix and Windows.
If you want to use Ninja, you could configure the project with:

```sh
cmake -G Ninja ..
```

For more information about generators, refer to the
[CMake documentation](https://cmake.org/cmake/help/latest/manual/cmake-generators.7.html)
