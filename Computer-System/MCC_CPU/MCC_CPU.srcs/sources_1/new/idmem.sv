`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2022/04/12 13:47:46
// Design Name: 
// Module Name: idmem
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


module idmem(input logic clk,we,
input logic [31:0]a,
input logic [31:0]wd,
output logic [31:0]rd);

logic [31:0] RAM[255:0];

initial 
    begin 
        $readmemh("memfile3.dat",RAM);
    end

always_ff @(posedge clk)
    if(we)
        RAM[a[31:2]]<=wd;
        
assign rd=RAM[a[31:2]];

endmodule 
