`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2022/05/19 13:15:52
// Design Name: 
// Module Name: music_Top
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


module music_Top(
input logic CLK100MHZ,
output logic AUD_PWM,
output logic AUD_SD
    );
    
    music M1(.clk(CLK100MHZ),
             .audioData(AUD_PWM), .audioSD(AUD_SD));
endmodule
