`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2021/10/11 12:47:43
// Design Name: 
// Module Name: x7seg
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


module x7seg(
    input logic [39:0]x,
    input logic clk,
    input logic reset,
    output logic [7:0]an,
    output logic [6:0]a2g
    );
    
    logic [2:0] s;
    logic [4:0]digit;
    logic [19:0]clkdiv;
    
    assign s=clkdiv[19:17];
    
    always_comb
        case(s)
            0:digit=x[4:0];
            1:digit=x[9:5];
            2:digit=x[14:10];
            3:digit=x[19:15];//=
            4:digit=x[24:20];
            5:digit=x[29:25];
            6:digit=x[34:30];
            7:digit=x[39:35];
        endcase
        
    always_comb
            case(s)
            0:an=8'b1111_1110;
            1:an=8'b1111_1101;
            2:an=8'b1111_1011;
            3:an=8'b1111_0111;//=
            4:an=8'b1110_1111;
            5:an=8'b1101_1111;
            6:an=8'b1011_1111;
            7:an=8'b0111_1111;
            default:an=8'b1111_1111;
            endcase
        
    always @(posedge clk or posedge reset)
    begin
        if(reset==1)
            clkdiv<=0;
        else    
            clkdiv <= clkdiv+1;
    end
    
    Hex7Seg s7(.x(digit),.a2g(a2g));
endmodule
