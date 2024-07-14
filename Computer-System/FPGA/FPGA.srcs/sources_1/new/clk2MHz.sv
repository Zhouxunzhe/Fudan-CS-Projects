`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2022/05/05 18:18:41
// Design Name: 
// Module Name: clk60Hz
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


module clk2MHz(
    input logic clk,
    output logic clk2048kHz
    );
    parameter N = 50;//100M/2M
    logic [31:0]cnt;
    
    always@(posedge clk)
    begin 
        clk2048kHz <= 0;
        if(cnt < N) cnt <= cnt+1;
        else begin cnt <= 0; clk2048kHz <= 1; end
    end
    
endmodule
