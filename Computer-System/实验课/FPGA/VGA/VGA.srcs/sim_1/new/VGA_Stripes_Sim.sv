`timescale 1ns / 1ps
module VGA_Stripes_Sim( );
    logic mclk, clr;    // �ӿڣ������ź�
    logic hSync, vSync; // �ӿڣ�����ź�
    logic [3:0] Red, Green, Blue;

    //�ֲ�����
    logic clk25MHz, displayOn;
    logic [10:0] xPixel, yPixel;
      
    always
    begin   // ����ʱ���ź�
        mclk = 0; #5; mclk = 1; #5;
    end
    
    initial
    begin   // �ṩ��ʼ�����ź�
        # 0; clr = 0;  #10; clr = 1;
        #10; clr = 0;  xPixel = 0;  yPixel = 0;      
    end
    
    // ʵ����
    clkDiv C1(.clk(mclk), .clr(clr), .clk25MHz(clk25MHz));
    
    VGA640x480 V1(.clk(clk25MHz), .clr(clr),                // Input
                  .hSync(hSync), .vSync(vSync),             // Output ***
                  .xPixel(xPixel), .yPixel(yPixel),         // Output 
                  .displayOn(displayOn));                   // Output
        
    VGA_Stripes VS(.displayOn(displayOn), .xPixel(xPixel), .yPixel(yPixel), 
                   .red(Red), .green(Green), .blue(Blue) ); // Output ***
endmodule