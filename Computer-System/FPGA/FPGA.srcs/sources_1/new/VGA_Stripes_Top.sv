module VGA_Stripes_Top(
    input logic CLK100MHZ,BTNC,
    output logic VGA_HS,VGA_VS,
    output logic [3:0]VGA_R,VGA_G,VGA_B);
    
    logic clk25MHz,displayOn;
    logic [10:0]xPixel,yPixel;
    
    clkDiv C1(.clk(CLK100MHZ), .clr(BTNC), .clk25MHz(clk25MHz));
    
    VGA640x480 V1(.clk(clk25MHz), .clr(BTNC),
                  .hSync(VGA_HS), .vSync(VGA_VS),
                  .xPixel(xPixel), .yPixel(yPixel),
                  .displayOn(displayOn));
                  
    VGA_Stripes VS(.displayOn(displayOn),
                   .xPixel(xPixel), .yPixel(yPixel),
                   .red(VGA_R), .green(VGA_G), .blue(VGA_B));
endmodule