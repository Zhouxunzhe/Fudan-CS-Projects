`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2022/03/08 23:06:09
// Design Name: 
// Module Name: DataMemoryDecoder
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


module DataMemoryDecoder(
input logic clk,writeEN,
input logic [7:0]addr,
input logic [31:0]writeData, 
output logic [31:0]readData,
input logic reset,
input logic btnL,btnR,
input logic [15:0]switch,
output logic [7:0]an,
output logic [6:0]a2g
    );
   logic [31:0]ReadData1,ReadData2;
   logic pWrite,pRead,mWrite;
   logic [11:0]led;
   
   assign pRead=(addr[7]==1'b1)?1:0;
   assign pWrite=(addr[7]==1'b1)?writeEN:0;
   assign mWrite=writeEN&(addr[7]==1'b0);
   
   dmem dmem(clk,mWrite,addr,writeData,ReadData1); 
   IO io(clk,reset,pRead,pWrite,addr[3:2],writeData[11:0],ReadData2,
        btnL,btnR,switch,led); 
   x7seg x7seg({1'b0,switch[15:12],
                1'b0,switch[11:8],
                1'b0,switch[7:4],
                1'b0,switch[3:0],
                6'b100010,led[11:8],
                1'b0,led[7:4],
                1'b0,led[3:0]},
                clk,reset,an,a2g); 
   mux2 #(32) rdmux(ReadData1,ReadData2,addr[7],readData);
endmodule
