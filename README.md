# ARCHIE AES Example

[[_TOC_]]

This is an AES example for ARCHIE.
The original binary of the paper currently can not be published.
Therefore i developed this replica.
Functionally it behaves the same as the original binary.
However the compiled output differs in how the AES was compiled.
But functionally they behave the same.

## Binary
A already compiled binary can be found in the binary folder.
You can also compile the binary for yourself using the CMake build system.
If you do most certannly locations will change.
Furthermore some parts could be assembled differently compared to this binary, therefore please analyse your lst file to find out the key locations.

## ARCHIE

Archie can be found under https://github.com/Fraunhofer-AISEC/archie .
The fault configuration can be found in [fault.json](fault.json)
The QEMU configuration used can be found in [qemuconf.json](qemuconf.json).
If you have archie in another folder, please change the paths inside the qemu configuration or move the fault configuration and the binary to archie.

### Run Experiment

To run the archie simulation, you can use 
```
python3 archie/controller.py --fault fault.json --qemu qemuconf.json -w ${nproc --all} aes-experiment.hdf5
```

