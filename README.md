# Hardware Simulator for UoY CS LPC17xx dev boards used in the Embeded Systems Project (EMPR) module

This repo contains;
 - A hardware simulator written in python
 - A selection of (incomplete) stub LPC17xx libraries to use to build a simulator-compatible test version of your code
 - an example hello world program that prints to the LCD.


## Overview of architecture

The simulator is a python program that runs on the host machine and listens on a unix socket for a test program to connect.

Any time a connection is oppened, a GUI pops up with a simulation of the keyboard and LCD

(This GUI was also going to do the serial and some other things, but never did)

The code you write for the LPC board can be re-compiled with the stub libraries to target the host machine, and will run on the host and connect to the socket of the simulator, to test the program. This allows you to use gdb on the host machine to debug things, which is very useful. You can also valgrind it, use a memory sanitizer, or whatever to find bugs without having to use the limited debugging available on the LPC board.

## Quick start

 - open two terminals
 - in one terminal, do the following:
   - `cd sim`
   - `./init_venv.sh`
   - `source venv/bin/activate` (or activate.fish or activate.csh, depending on your shell)
   - `./run`
 - if everything went well the sim program is listening for connections.
 - in the other terminal:
   - `cd hello_world`
   - `make test-hello_world`
   - `./run_in_simulator.sh`
 - you should see hello world run in a new simulator window

## How does this work?

`hello_world` can be compiled for the LPC or for the host machine with simulator, (use `make hello_world` to build a binary for the real hardware)

See `commonMakefile` for details of how these two version are built.

Any program that you want to be able to run in the simulator must be built with the simulator's stub implementations of the `lpcxx_foo.[hc]` (see the makefile for details, it does this when you `make test-hello_world`) and also needs to call `init_test_env()` which does the magic to connect to the simulator. See `hello_world.c` for an example.

## Libraries

This works with the I2C library
