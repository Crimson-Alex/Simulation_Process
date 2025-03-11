# Master Thesis Simulation Process

This repository contains the simulation process `main.c` and the pollution software (as I like to call it) `pollute.c` as well as a simple launch script `main.sh`.
Use `make` to build both, the simulation and the pollution executables.
The `main.sh` scrip (as well as `main`) takes an argument from -1 to 3 which defines the mode to be executed in. These are as follows:

- -1 : Matrix mode
- 0 : Repeated mode
- 1 : Contiguous mode
- 2 : Strided mode
- 3 : Random mode

The matrix mode is a remnant of early development ideas. In short, it allocates a number of 4KB (i.e. one memory page) arrays and their pointers are stored in another array.
Since it allocates multiple arrays, it can not be compared to single array access behaviors.
I kept it in here because I used it to better understand memory allocation behavior and for fun while tinkering with the kernelmodule.
The modes 0-4 refer to the access behaviors mentioned in my thesis.

Call `main.sh [mode]` to execute the simulation process with the respective mode while also gathering cache misses or `./main [mode]` to skip the cache measurements.
You can furthermore call `./pollute` to start a process that just performs some work in the background for 5 minutes.
