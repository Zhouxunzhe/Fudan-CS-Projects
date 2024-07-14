`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2022/03/07 16:07:14
// Design Name: 
// Module Name: test_regFile
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


module test_regFile();
logic clk;
logic regWriteEn;
logic [4:0]regWriteAddr;
logic [31:0]regWriteData;
logic [4:0]RsAddr;
logic [4:0]RtAddr;
logic [31:0]RsData;
logic [31:0]RtData;

regfile MUT(clk,regWriteEn,regWriteAddr,regWriteData,
           RsAddr, RtAddr,RsData,RtData);

initial begin
clk=0;
regWriteEn=0;
regWriteAddr=0;
regWriteData=0;
RsAddr=0;
RtAddr=0;

#100;
regWriteEn=1;
regWriteData=32'h1234abcd;
end

parameter PERIOD=20;
always begin
clk=1'b0;
#(PERIOD/2) clk=1'b1;
#(PERIOD/2);
end

always begin
regWriteAddr=8;
RsAddr=8;
#PERIOD;
end
endmodule
