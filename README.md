# ELEC2204-MIPs
Second year Computer Engineering coursework (ELEC2204). Building a 5-stage pipelined MIPs processor.


## Usage 

Compile and run the main program with the following.

```sh
make main
make runmain
```

## Test Usage

You can also test all the individual components in the file. This simply tests each unit, not the interconnects between them.

```sh
make test
make runtest
```

### Other Makefile

Makefile also has a clean feature to remove all compiled files.

```sh
make clean
```