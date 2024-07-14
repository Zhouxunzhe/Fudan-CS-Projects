module dmem(input logic clk,we,
input logic [7:0]a,
input logic [31:0]wd,
output logic [31:0]rd);

logic [31:0]RAM[255:0];
assign rd=RAM[a[7:2]];

always_ff @(posedge clk)
if(we)
RAM[a[7:2]]<=wd;

endmodule 