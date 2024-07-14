module maindec(input logic [5:0]op,
output logic memtoreg,memwrite,
output logic branch,alusrc,
output logic regdst,regwrite,
output logic jump,
output logic [2:0]aluop,
output logic immext,
output logic branchbne
);

 logic [11:0]controls;
 
 assign
 {regwrite,regdst,alusrc,branch,memwrite,
 memtoreg,jump,aluop,immext,branchbne}=controls;
 
 always_comb
 case(op)
 6'b000000:controls<=12'b110000001000;//RTYPE 
 6'b100011:controls<=12'b101001000000; //LW
 6'b101011:controls<=12'b001010000000; //SW
 6'b000100:controls<=12'b000100000100; //BEQ
 6'b001000:controls<=12'b101000000000; //ADDI
 6'b000010:controls<=12'b000000100000; //J
 6'b000101:controls<=12'b000000000101; //BNE
 6'b001101:controls<=12'b101000001110; //ORI
 6'b001100:controls<=12'b101000010010; //ANDI
 default:  controls<=12'bxxxxxxxxxxxx;//illegal op
 endcase
 endmodule