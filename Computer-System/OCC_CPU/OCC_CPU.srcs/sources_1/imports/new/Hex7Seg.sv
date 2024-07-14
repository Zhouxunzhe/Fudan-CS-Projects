`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2021/10/11 12:50:01
// Design Name: 
// Module Name: Hex7Seg
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


module Hex7Seg(
    input logic [4:0]x,
    output logic[6:0]a2g
    );
    
    always_comb
    case(x)
        'h0:a2g=7'b1000000;
        'h1:a2g=7'b1111001;
        'h2:a2g=7'b0100100;
        'h3:a2g=7'b0110000;
        'h4:a2g=7'b0011001;
        'h5:a2g=7'b0010010;
        'h6:a2g=7'b0000010;
        'h7:a2g=7'b1111000;
        'h8:a2g=7'b0000000;
        'h9:a2g=7'b0010000;
        'hA:a2g=7'b0001000;
        'hB:a2g=7'b0000011;
        'hC:a2g=7'b1000110;
        'hD:a2g=7'b0100001;
        'hE:a2g=7'b0000110;
        'hF:a2g=7'b0001110;
        'h10:a2g=7'b0111001;//+
        'h11:a2g=7'b1110110;//=10001
        'h1F:a2g=7'b0110111;//=11111
        default:a2g=7'b1111111;
    endcase
endmodule
