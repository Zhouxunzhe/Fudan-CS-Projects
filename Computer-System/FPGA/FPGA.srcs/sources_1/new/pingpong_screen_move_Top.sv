module pingpong_screen_move_Top(
    input logic CLK100MHZ,BTND,
    input logic BTNU,BTNC,
    output logic VGA_HS,VGA_VS,
    output logic [3:0]VGA_R,VGA_G,VGA_B
);
    logic clk25MHz,clk60Hz,displayOn;
    logic [10:0]xPixel,yPixel;
    
    clkDiv C1(.clk(CLK100MHZ), .clr(BTND), .clk25MHz(clk25MHz));
    
    VGA640x480 V1(.clk(clk25MHz), .clr(BTND),
                  .hSync(VGA_HS), .vSync(VGA_VS),
                  .xPixel(xPixel), .yPixel(yPixel),
                  .displayOn(displayOn));
    clk60Hz C2(.clk(CLK100MHZ),.clr(BTND),.clk60Hz(clk60Hz));
    
    pingpong_screen_move P1(.clk(clk60Hz),
                            .reset(BTND),
                            .btnUp(BTNU),
                            .btnDown(BTNC),
                            .videoOn(displayOn),
                            .pix_x(xPixel),
                            .pix_y(yPixel),
                            .red(VGA_R),
                            .green(VGA_G),
                            .blue(VGA_B));
endmodule