module PWM #(parameter N=8)
    (input logic pwmClk,
     input logic [N-1:0] duty,
     output logic pwmOut);
     
     logic [N-1:0]cnt = 0;
     
     always_ff @(posedge pwmClk)
     begin
        cnt <= cnt+1;
        
        pwmOut <= (cnt < duty);
    end
endmodule