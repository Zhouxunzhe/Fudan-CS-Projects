module maindec(
input logic clk,
input logic reset,
input logic [5:0]op,
output logic pcwrite,memwrite,irwrite,regwrite,
output logic alusrcA,branch,iord,memtoreg,regdst,
output logic [1:0] alusrcB,pcsrc,
output logic [2:0] aluop,
output logic immext,branchbne
);

localparam FETCH   = 5'b00000;//state 0
localparam DECODE  = 5'b00001;//state 1
localparam MEMADR  = 5'b00010;//state 2
localparam MEMRD   = 5'b00011;//state 3
localparam MEMWB   = 5'b00100;//state 4
localparam MEMWR   = 5'b00101;//state 5
localparam RTYPEEX = 5'b00110;//state 6
localparam RTYPEWB = 5'b00111;//state 7
localparam BEQEX   = 5'b01000;//state 8
localparam ADDIEX  = 5'b01001;//state 9
localparam ADDIWB  = 5'b01010;//state 10
localparam JEX     = 5'b01011;//state 11
localparam ANDIEX  = 5'b01100;//state 12
localparam ANDIWB  = 5'b01101;//state 13
localparam ORIEX   = 5'b01110;//state 14
localparam ORIWB   = 5'b01111;//state 15
localparam BNEEX   = 5'b10000;//state 16

localparam LW      = 6'b100011;//opcode for lw
localparam SW      = 6'b101011;//opcode for sw
localparam RTYPE   = 6'b000000;//opcode for R-type
localparam BEQ     = 6'b000100;//opcode for beq
localparam ADDI    = 6'b001000;//opcode for addi
localparam J       = 6'b000010;//opcode for j
localparam ANDI    = 6'b001100;//opcode for andi
localparam ORI     = 6'b001101;//opcode for ori
localparam BNE     = 6'b000101;//opcode for bne

logic [4:0] state,nextstate;
logic [17:0]controls;

//state register
always_ff @(posedge clk or posedge reset)
    if(reset) state<=FETCH;
    else state<=nextstate;
    
//next state logic
always_comb
    case(state)
        FETCH: nextstate=DECODE;
        DECODE:case(op)
            LW:     nextstate=MEMADR;
            SW:     nextstate=MEMADR;
            RTYPE:  nextstate=RTYPEEX;
            BEQ:    nextstate=BEQEX;
            ADDI:   nextstate=ADDIEX;
            J:      nextstate=JEX;
            ANDI:   nextstate=ANDIEX;
            ORI:    nextstate=ORIEX;
            BNE:    nextstate=BNEEX;
            default:nextstate=5'bx;
            endcase
        MEMADR:case(op)
            LW:     nextstate=MEMRD;
            SW:     nextstate=MEMWR;
            default:nextstate=5'bx;
            endcase
        MEMRD:   nextstate=MEMWB;
        MEMWB:   nextstate=FETCH;
        MEMWR:   nextstate=FETCH;
        RTYPEEX: nextstate=RTYPEWB;
        RTYPEWB: nextstate=FETCH;
        BEQEX:   nextstate=FETCH;
        ADDIEX:  nextstate=ADDIWB;
        ADDIWB:  nextstate=FETCH;
        JEX:     nextstate=FETCH;
        ANDIEX:  nextstate=ANDIWB;
        ANDIWB:  nextstate=FETCH;
        ORIEX:   nextstate=ORIWB;
        ORIWB:   nextstate=FETCH;
        BNEEX:   nextstate=FETCH;
        default: nextstate=5'bx;
    endcase
    
    //output logic 
    assign {pcwrite,memwrite,irwrite,regwrite,
    alusrcA,branch,iord,memtoreg,regdst,
    alusrcB,pcsrc,aluop,immext,branchbne}=controls;

    always_comb
        case(state)
            FETCH:   controls=18'h28080;
            DECODE:  controls=18'h00180;
            MEMADR:  controls=18'h02100;
            MEMRD:   controls=18'h00800;
            MEMWB:   controls=18'h04400;
            MEMWR:   controls=18'h10800;
            RTYPEEX: controls=18'h02008;
            RTYPEWB: controls=18'h04200;
            BEQEX:   controls=18'h03024;
            ADDIEX:  controls=18'h02100;
            ADDIWB:  controls=18'h04000;
            JEX:     controls=18'h20040;
            ANDIEX:  controls=18'h02112;
            ANDIWB:  controls=18'h04002;
            ORIEX:   controls=18'h0210E;
            ORIWB:   controls=18'h04002;
            BNEEX:   controls=18'h02025;
//            FETCH:  controls = 18'b10_1000_0000_1000_0000;
//            DECODE: controls = 18'b00_0000_0001_1000_0000;
//            MEMADR: controls = 18'b00_0010_0001_0000_0000;
//            MEMRD:  controls = 18'b00_0000_1000_0000_0000;
//            MEMWB:  controls = 18'b00_0100_0100_0000_0000;
//            MEMWR:  controls = 18'b01_0000_1000_0000_0000;
//            RTYPEEX:controls = 18'b00_0010_0000_0000_1000;
//            RTYPEWB:controls = 18'b00_0100_0010_0000_0000;
//            BEQEX:  controls = 18'b00_0011_0000_0010_0100;
//            ADDIEX: controls = 18'b00_0010_0001_0000_0000;
//            ADDIWB: controls = 18'b00_0100_0000_0000_0000;
//            JEX:    controls = 18'b10_0000_0000_0100_0000;
//            ANDIEX: controls = 18'b00_0010_0001_0001_0010;
//            ANDIWB: controls = 18'b00_0100_0000_0000_0010;
//            ORIEX: controls  = 18'b00_0010_0001_0000_1110;
//            ORIWB: controls  = 18'b00_0100_0000_0000_0010;
//            BNEEX:  controls = 18'b00_0010_0000_0010_0101;
            default: controls=18'hxxxxx;
        endcase
 endmodule