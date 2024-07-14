module PWM  #( parameter N=8 )     // ռ�ձ�λ��
    //��N=8���������Ŀ��PWMƵ�� = pwmClk��Ƶ��/256   
    ( input  logic pwmClk,       
      input  logic [N-1:0] duty, //duty cycleռ�ձ�
      output logic pwmOut   );

    logic [N-1:0] cnt = 0;
    
    always_ff @(posedge pwmClk) 
    begin
        cnt <= cnt + 1;
        // ��cnt<dutyʱ��pwmOut=1;
        // ����        pwmOut=0.
        pwmOut <= (cnt < duty);
    end
endmodule

//�����ȵ��� PWM (Pulse-Width Modulation)
/*
�ǽ�ģ���źű任Ϊ�����һ�ּ�����
һ��任����������ڹ̶���������Ĺ�������
����ģ���źŵĴ�С���ı䡣

ռ�ձ�:�����ʱ��������崰��ʱ��
*/