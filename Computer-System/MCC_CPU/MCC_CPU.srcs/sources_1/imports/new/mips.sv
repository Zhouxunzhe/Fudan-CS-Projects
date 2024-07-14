module mips(
input logic clk,reset,
output logic memwrite,
output logic [31:0]addr,writedata,
input logic [31:0]readdata);


logic iord,irwrite,pcen,zero,alusrcA;
logic regwrite,regdst,memtoreg,immext;
logic [1:0]pcsrc,alusrcB;
logic [2:0]alucontrol;
logic [31:0]instr,Adr;
logic [31:0]pc,aluout;

flopenr #(32)instrreg(clk,reset,irwrite,readdata,instr);
mux2 #(32)addrmux(pc,aluout,iord,addr);

controller c(clk,reset,instr[31:26],instr[5:0],zero,iord,memwrite,irwrite,pcen,
    pcsrc,alucontrol,alusrcB,alusrcA,regwrite,regdst,memtoreg,immext);

datapath dp(clk,reset,iord,irwrite,pcen,pcsrc,alucontrol,alusrcB,alusrcA,
    regwrite,regdst,memtoreg,Adr,instr,writedata,readdata,zero,immext,pc,aluout);

endmodule