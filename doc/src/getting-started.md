Getting Started
===============

## Prerequisites

* a C89 (ANSI C) or a C++ compiler.


> **Note:** The automatic registration of tests requires a widespread
> compiler-specific feature that places data in a given memory section.
> The compilers currently supported are the GNU compilers ([clang][clang],
> [gcc][gcc], [icc][icc]) and [MSVC][msvc].


## Installation


### Just the One File

The simplest option to be good to go is to copy the file [`rexo.h`][rexo.h] into
your source directory.

The folder containing the file `rexo.h` needs to be recognized as an include
directory by your compiler, which will make Rexo available by using
`#include <rexo.h>` in your code.


### The Whole Repository

Instead of copying solely the `rexo.h` file, you could also grab the whole
code repository, which brings in some additional features such as being able
to link against the library using [CMake][cmake].

Additionally, [Git][git] can be used to add Rexo's repository as a submodule of
your project, thus allowing to conveniently pull updates at any time.
This can be done by running the `git submodule` command from within
the directory where Rexo should be added to:

```sh
git submodule add git@github.com:christophercrouzet/rexo.git
```


> **Note:** Linking against the Rexo library using CMake is really simple.
> If your project _Foo_ has a structure on disk similar to this one:
> ```
> foo/
> ├── deps/
> │   └── rexo/
> │       └── ...
> ├── src/
> ├── tests/
> └── CMakeLists.txt
> ```
>
> Then you'd only have to add two lines to your own `CMakeFiles.txt` file:
> ```
> add_subdirectory(deps/rexo)
> target_link_libraries(foo PRIVATE rexo)
> ```


## Writing a First Test

To verify that everything is correctly set-up, create a new file containing
a simple test like the [minimal][minimal-guide] one from the guides section,
compile it, and run it!


[minimal-guide]: ./guides.md#minimal

[cmake]: https://cmake.org
[clang]: https://clang.llvm.org
[gcc]: https://gcc.gnu.org
[git]: https://git-scm.com
[icc]: https://software.intel.com/en-us/c-compilers
[msvc]: https://visualstudio.microsoft.com/vs/features/cplusplus
[rexo.h]: https://github.com/christophercrouzet/rexo/blob/master/include/rexo.h
