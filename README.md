# Branch-Predictor-Simulator

Implements a branch predictor simulator that takes as an input the configuartions of a pattern history table and a global history buffer to simulate gshare branch prediction.

## Usage

Clone this repository using `git clone git@github.com:CrChromium/Branch-Predictor-Simulator.git`

Navigate to the folder

Run `./SIM gshare <GPB> <RB> <trace_file>` in your terminal

### Arguments

GPB: The bit width of the pattern history table

RB: The number of register bits in the global history buffer

trace_file: The path to the file with your addresses

### Example File Data

0x00 t  
0x04 n  
0x00 t  
0x04 t  