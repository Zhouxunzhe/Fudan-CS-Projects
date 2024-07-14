`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2022/03/10 11:30:45
// Design Name: 
// Module Name: sim_top
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


module sim_top(
input logic clk,reset,
output logic [31:0]writedata,
output logic [31:0]dataadr,
output logic memwrite
);

logic [31:0]pc,instr,readdata;

mips mips(clk,reset,pc,instr,memwrite,dataadr,writedata,readdata);

imem imem(pc[7:2],instr);

dmem dmem(clk,memwrite,dataadr[7:0],writedata,readdata);

endmodule
