 module VGA640x480(  input  logic clk, clr,
    output logic hSync, vSync,    //行同步信号, 场同步信号
    output logic [10:0] xPixel, yPixel, //行(800)、列(521)计数器
    output logic displayOn );     //是否处于可显示的范围？
     
    // horizontal constants
    localparam H_DISPLAY = 640; // horizontal display width 
    localparam H_SYNC    =  96; // horizontal sync width
    localparam H_BACK    =  48; // left border (back porch) 
    localparam H_FRONT   =  16; // right border (front porch) 
    // vertical constants
    localparam V_DISPLAY = 480; // vertical display height 
    localparam V_SYNC    =   2; // vertical sync lines
    localparam V_TOP     =  29; // vertical top border  
    localparam V_BOTTOM  =  10; // vertical bottom border  
    // derived constants
    localparam 
      H_SYNC_START = H_SYNC + H_BACK,                       //行显示后沿 = 144(96+48)
      H_SYNC_END   = H_SYNC + H_BACK + H_DISPLAY,           //行显示前沿 = 784(96+48+640)
      H_PIXELS     = H_SYNC + H_BACK + H_DISPLAY + H_FRONT, //行像素点数 = 800(96+48+640+16)
      V_SYNC_START = V_SYNC + V_TOP,                        //场显示后沿 =  31(2+29)
      V_SYNC_END   = V_SYNC + V_TOP + V_DISPLAY,            //场显示前沿 = 511(2+29+480)
      V_LINES      = V_SYNC + V_TOP + V_BOTTOM + V_DISPLAY; //    总行数 = 521(2+29+480+10)

    logic hMaxed, vMaxed;
    assign hMaxed = (xPixel == H_PIXELS-1) || clr;	// set when xPixel is maximum
    assign vMaxed = (yPixel == V_LINES -1) || clr;	// set when yPixel is maximum
     
    // horizontal position counter  
    always @(posedge clk)
    begin
        //行同步信号  [96~784)
        hSync <= (xPixel>=H_SYNC && xPixel<H_SYNC_END); 
        if(hMaxed)  xPixel <= 0;
        else        xPixel <= xPixel + 1;
    end  
    
    // vertical position counter  
    always @(posedge clk)
    begin
        //场同步信号 [2~511)
        vSync <= (yPixel>=V_SYNC && yPixel<V_SYNC_END); 
        if(hMaxed)
            if (vMaxed)  yPixel <= 0;
            else         yPixel <= yPixel + 1;
    end  
    
    //Enable displayOn when beam is in "safe" visible frame
    assign displayOn = ((xPixel>=H_SYNC_START) && // [144~
                        (xPixel< H_SYNC_END) &&   //  784)
                        (yPixel>=V_SYNC_START) && // [31~
                        (yPixel< V_SYNC_END));    //  511)
endmodule