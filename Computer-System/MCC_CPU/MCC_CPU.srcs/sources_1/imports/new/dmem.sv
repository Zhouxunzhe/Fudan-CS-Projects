module dmem(input logic clk,we,
input logic [31:0]a,
input logic [31:0]wd,
output logic [31:0]rd);

logic [31:0]RAM[255:0];
initial 
begin 
    $readmemh("memfile4.dat",RAM);
    //$readmemh("memfile.dat",RAM);
end
assign rd=RAM[a[31:2]];

always_ff @(posedge clk)
if(we)
RAM[a[31:2]]<=wd;

endmodule 