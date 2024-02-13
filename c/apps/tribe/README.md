# Tribe

Tribe is an address book manager utility for the GNOME and XFCE desktop environments.

## Dependencies

Tribe depends on the following packages:

* [GLib](https://wiki.gnome.org/Projects/GLib) >= 2.48.2
* [GTK](https://www.gtk.org) >= 2.24.30

## Manual installation

1. Install all the needed dependencies.
2. Clone and install with the following commands:

```
git clone https://github.com/tfarina/maple3.git
cd maple3/c/apps/tribe
mkdir build && cd build
cmake -GNinja ..
ninja
sudo ninja install
```
