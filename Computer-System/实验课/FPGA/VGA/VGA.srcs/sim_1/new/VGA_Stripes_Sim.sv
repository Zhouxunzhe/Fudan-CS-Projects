`timescale 1ns / 1ps
module VGA_Stripes_Sim( );
    logic mclk, clr;    // 接口：激励信号
    logic hSync, vSync; // 接口：输出信号
    logic [3:0] Red, Green, Blue;

    //局部变量
    logic clk25MHz, displayOn;
    logic [10:0] xPixel, yPixel;
      
    always
    begin   // 产生时钟信号
        mclk = 0; #5; mclk = 1; #5;
    end
    
    initial
    begin   // 提供初始激励信号
        # 0; clr = 0;  #10; clr = 1;
        #10; clr = 0;  xPixel = 0;  yPixel = 0;      
    end
    
    // 实例化
    clkDiv C1(.clk(mclk), .clr(clr), .clk25MHz(clk25MHz));
    
    VGA640x480 V1(.clk(clk25MHz), .clr(clr),                // Input
                  .hSync(hSync), .vSync(vSync),             // Output ***
                  .xPixel(xPixel), .yPixel(yPixel),         // Output 
                  .displayOn(displayOn));                   // Output
        
    VGA_Stripes VS(.displayOn(displayOn), .xPixel(xPixel), .yPixel(yPixel), 
                   .red(Red), .green(Green), .blue(Blue) ); // Output ***
endmodule