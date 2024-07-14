module VGA_Stripes(
    input logic displayOn,
    input logic [10:0]xPixel,yPixel,
    output logic [3:0]red,green,blue
);
    //∫·≤ Ãı
    assign red   =  {4{yPixel[4]}};
    assign green = ~{4{yPixel[4]}};
    assign blue  = 0;
    // ˙≤ Ãı
    assign red   =  {4{xPixel[4]}};
    assign green = ~{4{xPixel[4]}};
    assign blue  = 0;
endmodule