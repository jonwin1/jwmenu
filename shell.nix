{
  pkgs ? import <nixpkgs> { },
}:
pkgs.mkShell {
  buildInputs = with pkgs; [
    clang-tools
    gnumake
    valgrind

    man-pages
    man-pages-posix
  ];
}
