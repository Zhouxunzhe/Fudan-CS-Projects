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


module clk60Hz(
    input logic clk,
    input logic clr,
    output logic clk60Hz
    );
    parameter N = 1666666;//1M/60
    logic [31:0]cnt;
    
    always@(posedge clk,posedge clr)
        if(clr==1) cnt<=0;
        else 
        begin 
            clk60Hz <= 0;
            if(cnt < N) cnt <= cnt+1;
            else begin cnt <= 0; clk60Hz <= 1; end
        end
    
endmodule
