module PWM_LED_Top(
    input  logic CLK100MHZ,
    output logic [3:0] LED  );

    // 100MHz/(2^8) = 390kHz (目标PWM频率: LED的闪烁频率)
    PWM #(8)L0(.pwmClk(CLK100MHZ), .duty(10),  .pwmOut(LED[0])); //  11/256= 4%
    PWM #(8)L1(.pwmClk(CLK100MHZ), .duty(50),  .pwmOut(LED[1])); //  51/256=20%
    PWM #(8)L2(.pwmClk(CLK100MHZ), .duty(127), .pwmOut(LED[2])); // 128/256=50%
    PWM #(8)L3(.pwmClk(CLK100MHZ), .duty(255), .pwmOut(LED[3])); // 256/256=100%
endmodule