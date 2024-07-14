module VGA640x480(input logic clk,clr,
    output logic hSync, vSync,
    output logic [10:0]xPixel,yPixel,
    output logic displayOn
);
    //horizontal
    localparam H_DISPLAY = 640;
    localparam H_SYNC    =  96;
    localparam H_BACK    =  48;
    localparam H_FRONT   =  16;
    
    //vertical
    localparam V_DISPLAY = 480;
    localparam V_SYNC    =   2;
    localparam V_TOP     =  29;
    localparam V_BOTTOM  =  10;
    
    localparam 
        H_SYNC_START = H_SYNC + H_BACK,
        H_SYNC_END   = H_SYNC + H_BACK + H_DISPLAY,
        H_PIXELS     = H_SYNC + H_BACK + H_DISPLAY + H_FRONT,
        V_SYNC_START = V_SYNC + V_TOP,
        V_SYNC_END   = V_SYNC + V_TOP + V_DISPLAY,
        V_LINES      = V_SYNC + V_TOP + V_DISPLAY + V_BOTTOM;
        
    logic hMaxed,vMaxed;
    assign hMaxed = (xPixel == H_PIXELS-1) || clr;
    assign vMaxed = (yPixel == V_LINES -1) || clr;
    
    always @ (posedge clk)
    begin
        hSync <= (xPixel>=H_SYNC && xPixel<H_SYNC_END);
        if(hMaxed) xPixel <= 0;
        else       xPixel <= xPixel + 1;
    end
    
    always @ (posedge clk)
    begin
        vSync <= (yPixel>=V_SYNC && yPixel<V_SYNC_END);
        if(hMaxed) 
            if(vMaxed) yPixel <= 0;
            else       yPixel <= yPixel + 1;
    end
    
    assign displayOn = ((xPixel>=H_SYNC_START) &&
                        (xPixel< H_SYNC_END) &&
                        (yPixel>=V_SYNC_START) &&
                        (yPixel< V_SYNC_END));
endmodule