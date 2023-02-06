module DAC(clk,rst_n,dac_dat,DAC_clk,DAC_d);
input clk,rst_n;
input [9:0] dac_dat;
output DAC_clk;
output [9:0]	DAC_d;

assign DAC_d[9:0] = dac_dat;
assign DAC_clk = clk;
endmodule