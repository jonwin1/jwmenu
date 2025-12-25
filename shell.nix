{
  pkgs ? import <nixpkgs> { },
}:
pkgs.mkShell {
  buildInputs = with pkgs; [
    clang-tools
    gdb
    gnumake
    valgrind

    man-pages
    man-pages-posix
  ];
}
