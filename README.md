# `mapgen2` - A Map Generator

`mapgen2` is a prototyping tool for 2D and 3D procedural terrain maps.

# Prerequisites & Dependencies

`mapgen2` is written in C++ and uses `cmake` to automate the
configuration and build process. To compile `mapgen2` you must make
the following libraries available to `cmake`.

 - [magnum](http://magnum.graphics/), `mapgen2` depends on
   `Magnum::Sdl2Application`, `Magnum::GL`
 - [boost](https://www.boost.org/)

The following libraries are included as `git` submodules.

 - [libnoise](http://libnoise.sourceforge.net/), both `noise-static`
   and `noiseutils-static` are required.
 - [imgui](https://github.com/ocornut/imgui) (included as a submodule)
 - [magnum-imgui](https://github.com/lecopivo/magnum-imgui)

# Building From Source

To compile mapgen2 from source use `cmake`. 

`mapgen2` uses `git` submodules, to pull all of the required external
sources, change to the `mapgen2` source repository and initialize the
submodules.

```
$ git submodule update --init --recursive
```

After that create a build directory and call cmake to create
makefiles.

```
$ mkdir build
$ cd build
$ cmake ..
```

After `cmake` completes you can compile `mapgen2`.

```
$ cmake --build .
```

# Usage

Call the `mapgen2` binary from the command line. `mapgen2` does not
implement any command line options, but you can pass options to
`magnum`. For example to set the
[context](http://doc.magnum.graphics/magnum/classMagnum_1_1GL_1_1Context.html).

# Contribute

PRs welcome.

# License

None ATM.
