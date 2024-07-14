`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2022/04/12 13:50:03
// Design Name: 
// Module Name: testbench
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


module testbench( );
    logic clk,reset,memwrite;
    logic[31:0]writedata,dataadr;
    
    topext dut(clk,reset,writedata,dataadr,memwrite);
    
    initial begin
        reset<=1;#22;reset<=0; end
    always begin
        clk<=1;#5;clk<=0;#5; end
        
    always @(negedge clk)
    begin
        if(memwrite) begin
            if(dataadr === 84 & writedata === 7) begin
                $display("Simulation succeeded");
                $stop;
            end else if(dataadr !== 80) begin
                $display("Simulation failed");
                $stop;
            end
        end
    end
endmodule
