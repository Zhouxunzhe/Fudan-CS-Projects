module maindec(
input logic [5:0] op,
output logic regwrite,regdst,
output logic alusrc,branch,
output logic memwrite,memtoreg,
output logic jump,immext,
output logic [2:0] aluop
);

logic [10:0] controls;

assign {regwrite,regdst,alusrc,branch,
        memwrite,memtoreg,jump,aluop,immext}=controls;
        
always_comb
    case(op)
        6'b000000:controls=11'b11000000100;//Rtype
        6'b100011:controls=11'b10100100000;//LW
        6'b101011:controls=11'b00101000000;//SW
        6'b000100:controls=11'b00010000010;//BEQ
        6'b001000:controls=11'b10100000000;//ADDI
        6'b000010:controls=11'b00000010000;//J
        6'b001100:controls=11'b10100001001;//ANDI
        default:  controls=11'bxxxxxxxxxxx;//???
    endcase
 endmodule