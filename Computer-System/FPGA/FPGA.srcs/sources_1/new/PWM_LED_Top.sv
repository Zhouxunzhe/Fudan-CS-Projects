module PWM_LED_Top(
input logic CLK100MHZ,
output logic [3:0] LED);

PWM #(8) L0(.pwmClk(CLK100MHZ), .duty(10),  .pwmOut(LED[0]));
PWM #(8) L1(.pwmClk(CLK100MHZ), .duty(50),  .pwmOut(LED[1]));
PWM #(8) L2(.pwmClk(CLK100MHZ), .duty(127), .pwmOut(LED[2]));
PWM #(8) L3(.pwmClk(CLK100MHZ), .duty(255), .pwmOut(LED[3]));

endmodule