module datapath(
input logic clk,reset,
input logic iord,irwrite,
input logic pcen,
input logic [1:0]pcsrc,
input logic [2:0]alucontrol,
input logic [1:0]alusrcB,
input logic alusrcA,
input logic regwrite,regdst,
input logic memtoreg,
output logic [31:0]Adr,
input logic [31:0]instr,
output logic [31:0]writedata,
input logic [31:0]readdata,
output logic zero,
input logic immext,
output logic [31:0] pc,aluout
);

logic [4:0]writereg;
logic [31:0]pcnext;
logic [31:0]data;
logic [31:0]wd3,rd1,rd2;
logic [31:0]A,B;
logic [31:0]signimm,signimmsh,zeroimm,imm;
logic [31:0]srcA,srcB;
logic [31:0]aluresult;
logic [31:0]pcjump;

flopenr #(32) pcreg(clk,reset,pcen,pcnext,pc);
mux2 #(32) adrmux(pc,aluout,iord,Adr);

flopr #(32) datareg(clk,reset,readdata,data);

mux2 #(5) wrmux(instr[20:16],instr[15:11],regdst,writereg);
mux2 #(32) regmux(aluout,data,memtoreg,wd3);

regfile rf(clk,regwrite,instr[25:21],instr[20:16],
            writereg,wd3,rd1,rd2);

flopr #(32) Areg(clk,reset,rd1,A);
flopr #(32) Breg(clk,reset,rd2,B);
assign writedata=B;

mux2 #(32) srcAmux(pc,A,alusrcA,srcA);
mux4 #(32) srcBmux(B,{{29{1'b0}},3'b100},imm,signimmsh,alusrcB,srcB);

signext se(instr[15:0],signimm);
zeroext ze(instr[15:0],zeroimm);
sl2 immsh(signimm,signimmsh);

mux2 #(32) extmux(signimm,zeroimm,immext,imm);

alu alu(srcA,srcB,alucontrol,aluresult,zero);
flopr #(32) aluoutreg(clk,reset,aluresult,aluout);
assign pcjump={pc[31:28],instr[25:0],2'b00};
mux4 #(32) pcnextmux(aluresult,aluout,pcjump,{32{1'b0}},pcsrc,pcnext);

endmodule