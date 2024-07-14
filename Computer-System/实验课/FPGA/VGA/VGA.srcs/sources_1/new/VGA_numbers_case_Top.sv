module VGA_numbers_case_Top( 
    input  logic CLK100MHZ, BTNC,
    output logic VGA_HS, VGA_VS, 
    output logic [3:0] VGA_R, VGA_G, VGA_B );

    logic clk25MHz, displayOn;
    logic [10:0] hPos, vPos;
    
    clkDiv C1(.clk(CLK100MHZ), .clr(BTNC), .clk25MHz(clk25MHz));
    
    VGA640x480 V1(.clk(clk25MHz),  .clr(BTNC),      // Input
                  .hSync(VGA_HS),  .vSync(VGA_VS),  // Output ***
                  .xPixel(hPos), .yPixel(vPos),     // Output 
                  .displayOn(displayOn));           // Output

    logic [3:0] digit = hPos[7:4];
    logic [2:0] xofs  = hPos[3:1];
    logic [2:0] yofs  = vPos[3:1];
    logic [4:0] bits;
                    
    Digits10_case D1(.digit(digit),  
                     .yofs(yofs),
                     .bits(bits) );   // output (5 bits)                  

    logic r = displayOn && 0;
    logic g = displayOn && bits[xofs ^ 3'b111];
    logic b = displayOn && 0;
    
    assign VGA_R = r;
    assign VGA_G = {4{g}};
    assign VGA_B = b;
endmodule