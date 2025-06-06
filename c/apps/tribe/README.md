# Tribe

Tribe is an address book manager for the GNOME and Xfce desktop environments.

## Dependencies

Tribe depends on the following packages:

* [GLib](https://gitlab.gnome.org/GNOME/glib/) >= 2.19.7
* [GTK](https://www.gtk.org) >= 2.16.1

## Configure your build

To configure your build, run the following:

```
$ mkdir out && cd out
$ cmake -GNinja ..
```

## Build and install

After it is properly configured, build it with:

```
$ ninja
```

When it is done, run the following to install it:

```
[Become root if necessary]
$ ninja install
```
