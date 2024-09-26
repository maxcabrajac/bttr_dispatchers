# bttr_dispatchers

A collection of useful dispatchers!

Most, if not all, of Hyprland dispatchers are made with direct user interaction in mind, that is, they are thought to be used as keybindings.
Although Hyprland's Plugin API is very powerful, this way of designing dispatchers makes a very poor scripting interface.
This plugin aims to add some scripting-friendly dispatchers!

# Installing

## `hyprpm`

```sh
hyprpm add github.com/maxcabrajac/bttr_dispatchers
```

This will do everything that needs to be done!

## Manually

```sh
make setup
make compile
make load

```

I guess these `make` targets are self-explanatory.

# Available dispatchers

- `bttr_workspacemonitor <workspace id> <monitor id>`

    Set a (possibly new) workspace as active on monitor without changing focus

# Inspired By

- [bspwm](https://github.com/baskerville/bspwm): `bspc` is the greatest window manager interface for scripters.
