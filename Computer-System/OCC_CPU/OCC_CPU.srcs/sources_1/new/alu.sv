module alu(input logic [31:0]a,b,
input logic [2:0]alucont,
output logic [31:0]result,
output logic zero
);
always@(*)
case(alucont)
    3'b000:begin                    //and
                result<=a&b;
            end
    3'b001:begin                    //or
                result<=a|b;
            end
    3'b010:begin                    //add
                result<=a+b;
            end
    3'b110:begin                    //sub
                result<=a-b;
            end
    3'b111:begin                    //slt
                if(a<b) result<={{31{1'b0}},1'b1};
                else result<={32{1'b0}};
            end
    default:begin
                result <= {32{1'bx}};
            end
endcase
assign zero = (result == 0)? 1 : 0;
endmodule