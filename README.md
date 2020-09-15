# System Monitor

A system monitor built using ncurses that mimics the htop monitor.

![System Monitor](images/system_monitor.png)

## ncurses
[ncurses](https://www.gnu.org/software/ncurses/) is a library that facilitates text-based graphical output in the terminal. This project relies on ncurses for display output.

## Make
This project uses [Make](https://www.gnu.org/software/make/). The Makefile has four targets:
* `build` compiles the source code and generates an executable
* `format` applies [ClangFormat](https://clang.llvm.org/docs/ClangFormat.html) to style the source code
* `debug` compiles the source code and generates an executable, including debugging symbols
* `clean` deletes the `build/` directory, including all of the build artifacts

## Usage Instructions

1. Clone the project repository: `git clone https://github.com/naveen13m/System-Monitor.git`

2. Build the project: `make build`

3. Run the resulting executable: `./build/monitor`