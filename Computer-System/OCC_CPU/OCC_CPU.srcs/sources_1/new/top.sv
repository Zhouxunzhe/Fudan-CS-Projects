`timescale 1ns / 1ps

module top(
input logic CLK100MHZ,BTNC,
input logic BTNL,BTNR,
input logic [15:0]SW,
output logic [7:0]AN,
output logic [6:0]A2G
);

logic [31:0]pc,writeData,dataAdr;
logic Write;
logic [31:0]instr,readData;

mips mips(CLK100MHZ,BTNC,pc,instr,Write,dataAdr,writeData,readData);

imem imem(pc[7:2],instr);

DataMemoryDecoder dmd(CLK100MHZ,Write,dataAdr[7:0],writeData,readData,
BTNC,BTNL,BTNR,SW,AN,A2G);

endmodule