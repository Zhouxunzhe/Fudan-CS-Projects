`timescale 1ns / 1ps
module alu(input logic [31:0]a,b,
input logic [2:0]alucont,
output logic [31:0]result
);

logic [31:0]b2,sum,slt;

assign b2 = alucont[2] ? ~b : b;
assign sum = a + b2 + alucont[2];
assign slt = sum[31];

always_comb
case(alucont[1:0])
    2'b00: result = a & b;
    2'b01: result = a | b;
    2'b10: result = sum;
    2'b11: result = slt;
endcase
endmodule