module mips(
input logic clk,reset,
input logic [31:0]instrF,readdataM,
output logic [31:0]pcF,
output logic memwriteM,
output logic [31:0]aluoutM,
output logic [31:0]writedataM);


logic [5:0] opD,functD;
logic       pcsrcD,equalD,branchD,jumpD,ImmExt;
logic [2:0] alucontrolE;
logic       memtoregE,regwriteE,regdstE,alusrcE,flushE;
logic       memtoregM,regwriteM;
logic       memtoregW,regwriteW;

controller c(clk,reset,
             opD,functD,equalD,
             flushE,
             pcsrcD,branchD,jumpD,ImmExt,
             alucontrolE,
             regwriteE,memtoregE,alusrcE,regdstE,
             regwriteM,memtoregM,memwriteM,
             regwriteW,memtoregW);

datapath dp(clk,reset,
            instrF,
            pcsrcD,branchD,jumpD,ImmExt,
            alucontrolE,
            regwriteE,memtoregE,alusrcE,regdstE,
            readdataM,
            regwriteM,memtoregM,
            regwriteW,memtoregW,
            pcF,
            opD,functD,equalD,
            flushE,
            aluoutM,writedataM);

endmodule