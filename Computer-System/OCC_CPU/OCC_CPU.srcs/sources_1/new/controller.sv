module controller(
input logic [5:0]op,funct,
input logic zero,
output logic memtoreg,memwrite,
output logic pcsrc,alusrc,
output logic regdst,regwrite,
output logic jump,
output logic [2:0]alucontrol,
output logic immext
);

logic[2:0]aluop;
logic branch,branchbne;

maindec md(op,memtoreg,memwrite,branch,
    alusrc,regdst,regwrite,jump,aluop,immext,branchbne);

aludec ad(funct,aluop,alucontrol);

assign pcsrc=(branch & zero)|(branchbne & ~zero);
endmodule