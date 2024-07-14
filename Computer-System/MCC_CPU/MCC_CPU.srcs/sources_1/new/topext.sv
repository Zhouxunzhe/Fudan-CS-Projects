`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2022/04/22 14:57:43
// Design Name: 
// Module Name: topext
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module topext(
input logic clk,reset,
output logic [31:0]writeData,addr,
output logic MemWrite
);

logic [31:0]readData;

mips mips(clk,reset,MemWrite,addr,writeData,readData);

idmem idmem (clk,Memwrite,addr,writeData,readData);
endmodule
