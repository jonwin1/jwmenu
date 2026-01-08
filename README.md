<!--
SPDX-FileCopyrightText: 2025-2026 Jonatan Wincent <jonatan.wincent@proton.me>
SPDX-License-Identifier: MIT
-->

# JWMenu

`JWMenu` is a small, configurable menu runner built on top of
[rofi](https://github.com/davatorium/rofi).

Menus are defined declaratively using TOML configuration files and may
contain arbitrary levels of nested submenus. Each menu entry can either
execute a command or open another submenu.

It can be used to build system menus, power menus, application launchers,
or any other structured rofi-based workflows.

[Read more about why I created JWMenu.](https://jonwin.se/posts/jwmenu/)

## Features

- Hierarchical menus with arbitrary nesting
- Declarative configuration using TOML
- Supports multiple independent menus via separate configuration files
- Minimal runtime dependencies

## Nix

### Running

```sh
nix run github:jonwin1/jwmenu -- path/to/menu.toml
```

or

```sh
git clone https://github.com/jonwin1/jwmenu.git
cd jwmenu
nix run . -- path/to/menu.toml
```

### Building

```sh
git clone https://github.com/jonwin1/jwmenu.git
cd jwmenu
nix build
```

Builds the executable `result/bin/jwmenu`.

### Installing

Add it as an input in your `flake.nix`:

```nix
inputs = {
  # ...
  jwmenu = {
    url = "github:jonwin1/jwmenu";
    inputs.nixpkgs.follows = "nixpkgs";
  };
};
```

And add the package to `environment.systemPackages` or `home.packages`:

```nix
inputs.jwmenu.packages.${system}.default
```

Rebuild your configuration and jump to [Usage](#usage).

## Non-Nix systems

### Dependencies

- [`rofi`](https://github.com/davatorium/rofi)
- A C17-compatible compiler

### Make

```sh
git clone https://github.com/jonwin1/jwmenu.git
cd jwmenu
make
```

This will produce the `jwmenu` binary in the project root.

## Usage

```sh
jwmenu path/to/menu.toml
```

## Configuration

The menu is defined using a TOML file with nested tables.

### Example

```toml
[config]
label = "Example Menu"

[[config.menu]]
label = "󰎤    Simple command"
command = "notify-send Simple"

[[config.menu]]
label = "󰎧    Multiline command"
command = """
echo Hello
notify-send Hello
"""

[[config.menu]]
label = "󰎪    Submenu →"

[[config.menu.submenu]]
label = "nested item"
command = "notify-send 'Hello from submenu'"
```

See more examples in [`config/`](config/).

### Semantics

- Each menu entry has a `label`
- An entry with a `command` executes that command
- An entry without a `command` is treated as a submenu
- Selecting a command closes the entire menu stack
- Pressing `Esc` closes the current menu level

## Project Structure

```text
.
├── config/           # Example menus
│   └── ...
├── lib/
│   ├── tomlc17.c     # TOML parsing library
│   └── tomlc17.h
├── LICENSES/
│   └── ...
├── src/
│   ├── config.c      # TOML parsing
│   ├── config.h
│   ├── main.c        # Entry point
│   ├── menu.c        # Menu logic and rofi interaction
│   ├── menu.h
│   ├── util.c        # Helper functions
│   └── util.h
├── flake.lock        # Flake dependencies lock
├── flake.nix         # Nix package and devshell definition
├── Makefile
├── README.md
└── REUSE.toml
```

## License

This project is licensed under the MIT License. See LICENSES/MIT.txt.

## Acknowledgements

- [rofi](https://github.com/davatorium/rofi)
- [tomlc17](https://github.com/cktan/tomlc17)
