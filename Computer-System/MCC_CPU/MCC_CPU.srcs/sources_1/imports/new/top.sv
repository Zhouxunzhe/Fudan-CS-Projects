`timescale 1ns / 1ps

module top(
input logic CLK100MHZ,
input logic BTNC,
input logic BTNL,
input logic BTNR,
input logic [15:0]SW,
output logic [7:0]AN,
output logic [6:0]A2G
);

logic [31:0]writeData,addr;
logic MemWrite;
logic [31:0]readData;

mips mips(CLK100MHZ,BTNC,MemWrite,addr,writeData,readData);

DataMemoryDecoder dmd(CLK100MHZ,MemWrite,addr[7:0],writeData,readData,
              BTNC,BTNL,BTNR,SW,AN,A2G);
endmodule