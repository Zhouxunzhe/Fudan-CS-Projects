module controller(
input logic clk,reset,
input logic [5:0]op,funct,
input logic zero,
output logic iord,memwrite,
output logic irwrite,pcen,
output logic [1:0]pcsrc,
output logic [2:0]alucontrol,
output logic [1:0]alusrcB,
output logic alusrcA,
output logic regwrite,regdst,
output logic memtoreg,
output logic immext
);

logic[2:0]aluop;
logic pcwrite,branch,branchbne;

maindec md(clk,reset,op,pcwrite,memwrite,irwrite,regwrite,
    alusrcA,branch,iord,memtoreg,regdst,alusrcB,pcsrc,aluop,immext,branchbne);

aludec ad(funct,aluop,alucontrol);

assign pcen=((branch & zero)|(branchbne & ~zero))|pcwrite;
endmodule