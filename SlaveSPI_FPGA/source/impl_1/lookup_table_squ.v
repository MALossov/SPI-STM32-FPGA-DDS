module lookup_tables_squ(phase, squ_out);
input  	[7:0] 	phase;
output 	[9:0] 	squ_out;
 
wire     [9:0]   squ_out;
 
reg   	[5:0] 	address;
wire   	[1:0] 	sel;
wire   	[8:0] 	squ_table_out;
 
reg     [9:0]   squ_onecycle_amp;
 
//assign squ_out = {4'b0, squ_onecycle_amp[9:4]} + 9'hff;  // 可以调节输出信号的幅度
assign squ_out = squ_onecycle_amp[9:0];
 
assign sel = phase[7:6];
 
squ_table u_squ_table(address,squ_table_out);
 
always @(sel or squ_table_out)
begin
	case(sel)
	2'b00: 	begin
			squ_onecycle_amp = 9'h1ff + squ_table_out[8:0];
			address = phase[5:0];
	     	end
  	2'b01: 	begin
			squ_onecycle_amp = 9'h1ff + squ_table_out[8:0];
			address = ~phase[5:0];
	     	end
  	2'b10: 	begin
			squ_onecycle_amp = 9'h1ff - squ_table_out[8:0];
			address = phase[5:0];
     		end
  	2'b11: 	begin
			squ_onecycle_amp = 9'h1ff - squ_table_out[8:0];
			address = ~ phase[5:0];
     		end
	endcase
end
 
endmodule