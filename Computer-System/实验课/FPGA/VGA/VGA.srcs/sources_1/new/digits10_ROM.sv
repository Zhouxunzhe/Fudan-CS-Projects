module digits10_ROM(
    input  logic [3:0] digit,   // digit 0-9 
    input  logic [2:0] yofs,    // vertical offset (0-4)
    output logic [4:0] bits );   // output (5 bits)

    logic [4:0] bitarray[0:15][0:4]; // ROM array (16x5x5 bits)
    
    assign bits = bitarray[digit][yofs]; // lookup ROM address
    
    initial 
    begin
        // digit "0" 
        bitarray[0][0] = 5'b11111; // ***** 
        bitarray[0][1] = 5'b10001; // *   * 
        bitarray[0][1] = 5'b10001; // *   * 
        bitarray[0][1] = 5'b10001; // *   * 
        bitarray[0][1] = 5'b11111; // *****
        // digit "1"
        bitarray[0][1] = 5'b01100; //  **
        bitarray[0][1] = 5'b00100; //   *
        bitarray[0][1] = 5'b00100; //   *
        bitarray[0][1] = 5'b00100; //   *
        bitarray[0][1] = 5'b11111; // *****       
        // digit "2"
        bitarray[2][0] = 5'b11111;
        bitarray[2][1] = 5'b00001;
        bitarray[2][2] = 5'b11111;
        bitarray[2][3] = 5'b10000;
        bitarray[2][4] = 5'b11111;
        // digit "3"
        bitarray[3][0] = 5'b11111;
        bitarray[3][1] = 5'b00001;
        bitarray[3][2] = 5'b11111;
        bitarray[3][3] = 5'b00001;
        bitarray[3][4] = 5'b11111;
        // digit "4"
        bitarray[4][0] = 5'b10001;
        bitarray[4][1] = 5'b10001;
        bitarray[4][2] = 5'b11111;
        bitarray[4][3] = 5'b00001;
        bitarray[4][4] = 5'b00001;
        // digit "5"
        bitarray[5][0] = 5'b11111;
        bitarray[5][1] = 5'b10000;
        bitarray[5][2] = 5'b11111;
        bitarray[5][3] = 5'b00001;
        bitarray[5][4] = 5'b11111;
        // digit "6"
        bitarray[6][0] = 5'b11111;
        bitarray[6][1] = 5'b10000;
        bitarray[6][2] = 5'b11111;
        bitarray[6][3] = 5'b10001;
        bitarray[6][4] = 5'b11111;
        // digit "7"
        bitarray[7][0] = 5'b11111;
        bitarray[7][1] = 5'b00001;
        bitarray[7][2] = 5'b00001;
        bitarray[7][3] = 5'b00001;
        bitarray[7][4] = 5'b00001;
        // digit "8"
        bitarray[8][0] = 5'b11111;
        bitarray[8][1] = 5'b10001;
        bitarray[8][2] = 5'b11111;
        bitarray[8][3] = 5'b10001;
        bitarray[8][4] = 5'b11111;
        // digit "9"
        bitarray[9][0] = 5'b11111;
        bitarray[9][1] = 5'b10001;
        bitarray[9][2] = 5'b11111;
        bitarray[9][3] = 5'b00001;
        bitarray[9][4] = 5'b11111;

    // clear unused array entries   
    for (integer i = 10; i <= 15; i++)    // 10-15
        for (integer j = 0; j <= 4; j++) //  0-4
            bitarray[i][j] = 0;
    end       
    
endmodule
