module PWM  #( parameter N=8 )     // 占空比位数
    //若N=8，则表明：目标PWM频率 = pwmClk的频率/256   
    ( input  logic pwmClk,       
      input  logic [N-1:0] duty, //duty cycle占空比
      output logic pwmOut   );

    logic [N-1:0] cnt = 0;
    
    always_ff @(posedge pwmClk) 
    begin
        cnt <= cnt + 1;
        // 当cnt<duty时，pwmOut=1;
        // 否则，        pwmOut=0.
        pwmOut <= (cnt < duty);
    end
endmodule

//脉冲宽度调制 PWM (Pulse-Width Modulation)
/*
是将模拟信号变换为脉冲的一种技术，
一般变换后脉冲的周期固定，但脉冲的工作周期
会依模拟信号的大小而改变。

占空比:脉冲高时间除以脉冲窗口时间
*/