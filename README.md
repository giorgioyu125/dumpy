# dumpy

A very simple and easy to read hexdump clone made with only one flag option (for now!) -C.

## Building the project

### Prerequisites

- A C compiler (gcc, clang, etc.)
- [Meson](https://mesonbuild.com/) build system
- [Ninja](https://ninja-build.org/) build backend

### Build

While in the root directory of the project:

```sh
mkdir build
meson setup build
cd build
ninja
