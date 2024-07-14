`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2022/05/10 21:52:02
// Design Name: 
// Module Name: floprc
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


module floprc #(parameter WIDTH=8)
(input logic clk,
 input logic reset,
 input logic clear,
 input logic [WIDTH-1:0]d,
 output logic [WIDTH-1:0]q
    );
    
    always_ff @(posedge clk,posedge reset)
        if(reset)      q<=0;
        else if(clear) q<=0;
            else       q<=d;
endmodule
