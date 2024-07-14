module music(
input logic clk,
output logic audioData,
output logic audioSD);

assign audioSD = 1;

localparam MEM_SIZE = 228742;
logic [17:0] address;
logic [7:0] value;
logic clk2048kHz;

clk2MHz C2(clk, clk2048kHz);

Jay M1(.clka(clk),
       .addra(address),
       .douta(value));

logic [7:0] counter = 0;
always@(posedge clk2048kHz)
begin
    counter <= counter + 1;
    audioData <= (counter < value);
    if(counter == (256-1))
    begin
        address <= address + 1;
        if(address == MEM_SIZE) address <= 0;
    end
end
endmodule