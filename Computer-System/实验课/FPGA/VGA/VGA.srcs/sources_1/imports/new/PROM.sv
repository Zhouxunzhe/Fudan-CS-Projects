module ROM_Pixels
    #(parameter ColNum=64, RowNum=16, addrNum=4) //2^4=16
    (input  logic [addrNum-1:0] addr,
     output logic [0:ColNum-1] ROM1rowData );
    
    logic [0:ColNum-1] ROM [0:RowNum-1]; // 64x16
    
    // 通过字模提取软件，提取16x16点阵数据
    localparam data = { // 复_旦_大_学
        64'h1008_0010_0100_2208, //0
        64'h1FFC_1FF8_0100_1108, //1
        64'h2000_1010_0100_1110, //2
        64'h2FF0_1010_0100_0020, //3
        64'h4810_1010_0104_7FFE, //4
        64'h8FF0_1010_FFFE_4002, //5
        64'h0810_1FF0_0100_8004, //6
        64'h0FF0_1010_0280_1FE0, //7
        64'h0400_1010_0280_0040, //8
        64'h0FF0_1010_0240_0184, //9
        64'h0C20_1010_0440_FFFE, //10
        64'h1240_1FF0_0420_0100, //11
        64'h2180_1010_0810_0100, //12
        64'h4240_0004_100E_0100, //13
        64'h0430_FFFE_6004_0500, //14
        64'h380E_0000_0000_0200  //15
    };

    integer i;
    // 填充 ROM 中的数据
    initial  begin
        for(i=0; i<RowNum; i=i+1)
            ROM[i] = data[(ColNum*RowNum-1 -ColNum*i)-:ColNum];
    end
    
    // 根据地址addr读取ROM中的一行数据
    assign ROM1rowData = ROM[addr];        
endmodule