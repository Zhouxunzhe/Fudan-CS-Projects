module imem(input logic [5:0]a,
output logic [31:0]rd);

logic [31:0]RAM[63:0];

initial 
begin 
    $readmemh("C:/Vivado_works/Second_Term/PCC_CPU/memfile3.dat",RAM);
end
assign rd=RAM[a];
endmodule