// 显示红色“复旦大学”4个字
module VGA_words_TOP(
    input  logic CLK100MHZ, BTNC,
    output logic VGA_HS, VGA_VS,
    output logic [3:0] VGA_R, VGA_G, VGA_B );
    
    logic clk25MHz, videoOn;
    logic [10:0] hCnt, vCnt;
    logic [0:63] ROM_data;
    logic [3:0]  ROM_addr4;
      
    clkDiv C1(.clk(CLK100MHZ), .clr(BTNC), .clk25MHz(clk25MHz));
    
    VGA640x480 V1(.clk(clk25MHz), .clr(BTNC), // Input
        .hSync(VGA_HS), .vSync(VGA_VS),       // Output ***
        .xPixel(hCnt), .yPixel(vCnt),         // Output
        .displayOn(videoOn));                 // Output
    
    // 到ROM中取点阵数据
    ROM_Pixels P1(.addr(ROM_addr4), 
                  .ROM1rowData(ROM_data));    // Output
    
    // 显示文字                      
    VGA_words W1(.displayOn(videoOn),
                 .hCounter(hCnt), .vCounter(vCnt),
                 .ROM1rowData(ROM_data),
                 .ROM_addr4(ROM_addr4),     // Output
                 .red(VGA_R),               // Output ***
                 .green(VGA_G),             // Output ***
                 .blue(VGA_B) );            // Output ***                   
endmodule