module VGA_words(
    input  logic displayOn,
    input  logic [10:0] hCounter, vCounter,
    input  logic [0:63] ROM1rowData, //ROM一行的点阵数据
    output logic [3:0]  ROM_addr4,   //到ROM取数据的地址
    output logic [3:0]  red, green, blue   );
    
    // horizontal constants
    localparam H_SYNC    =  96; // horizontal sync width
    localparam H_BACK    =  48; // left border (back porch) 
    // vertical constants
    localparam V_SYNC    =   2; // vertical sync lines
    localparam V_TOP     =  29; // vertical top border 
    // derived constants
    localparam H_SYNC_START = H_SYNC + H_BACK;   //行显示后沿 = 144(96+48) 10'b00_1001_0000
    localparam V_SYNC_START = V_SYNC + V_TOP;    //场显示后沿 =  31(2+29)  10'b00_0001_1111  

    localparam W = 64;
    localparam H = 16;
    
    logic [10:0] R1, C1;  //sprite左上角的行、列
    // sprite起点暂定(100,100)
    assign R1 = 100;    //{2'b00, sw[7:4], 5'b00001};
    assign C1 = 100;    //{2'b00, sw[3:0], 5'b00001};
       
    logic [10:0] ROM_addr, ROM_pix;
    logic spriteOn, r, g, b;
    
    // <1> 判断sprite显示范围
    always @(*)
        if ((hCounter>=H_SYNC_START+C1) && (hCounter<H_SYNC_START+C1+W) &&
            (vCounter>=V_SYNC_START+R1) && (vCounter<V_SYNC_START+R1+H))
            spriteOn = 1;
        else
            spriteOn = 0;
            
    // <2> 到sprite的ROM中取一行点阵的数据：[0:63] ROM1rowData
    assign ROM_addr  = vCounter - V_SYNC_START - R1;
    assign ROM_addr4 = ROM_addr[3:0];            
            
     // <3> 输出视频色彩信号到VGA
     always @(*)
     begin
        red   = 0;
        green = 0;
        blue  = 0;
        if((spriteOn==1) && (displayOn==1))
        begin
            ROM_pix  = hCounter - H_SYNC_START - C1;
            // 根据文字点阵填充色彩
            r = ROM1rowData[ROM_pix]; //************
            g = 0; 
            b = 0; 
            // 输出色彩到VGA
            red   = {r, r, r, r};
            green = {g, g, g, g};
            blue  = {b, b, b, b};
        end
     end        
endmodule