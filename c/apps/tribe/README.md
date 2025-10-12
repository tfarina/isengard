# Tribe

Tribe is an address book manager for the GNOME and Xfce desktop environments.

## Dependencies

Tribe depends on the following packages:

* [GLib](https://gitlab.gnome.org/GNOME/glib/) >= 2.19.7
* [GTK](https://www.gtk.org) >= 2.16.1

## Building from Source

This project uses [CMake](https://cmake.org/) as its build system and
[Ninja](https://ninja-build.org/) as the build tool.

### 1. Configure the Build

Create a dedicated directory for the build and generate the build files:

```sh
mkdir out && cd out
cmake -G Ninja ..
```

### 2. Compile the Project

Build the project using Ninja:

```sh
ninja
```

### 3. Install

Install the resulting binaries and other files to the system (root privileges
may be required):

```sh
sudo ninja install
```
