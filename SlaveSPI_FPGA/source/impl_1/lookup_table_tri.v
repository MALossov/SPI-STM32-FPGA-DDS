module lookup_tables_tri(phase, tri_out);
input  	[7:0] 	phase;
output 	[9:0] 	tri_out;
 
wire     [9:0]   tri_out;
 
reg   	[5:0] 	address;
wire   	[1:0] 	sel;
wire   	[8:0] 	tri_table_out;
 
reg     [9:0]   tri_onecycle_amp;
 
//assign tri_out = {4'b0, tri_onecycle_amp[9:4]} + 9'hff;  // 可以调节输出信号的幅度
assign tri_out = tri_onecycle_amp[9:0];
 
assign sel = phase[7:6];
 
tri_table u_tri_table(address,tri_table_out);
 
always @(sel or tri_table_out)
begin
	case(sel)
	2'b00: 	begin
			tri_onecycle_amp = 9'h1ff + tri_table_out[8:0];
			address = phase[5:0];
	     	end
  	2'b01: 	begin
			tri_onecycle_amp = 9'h1ff + tri_table_out[8:0];
			address = ~phase[5:0];
	     	end
  	2'b10: 	begin
			tri_onecycle_amp = 9'h1ff - tri_table_out[8:0];
			address = phase[5:0];
     		end
  	2'b11: 	begin
			tri_onecycle_amp = 9'h1ff - tri_table_out[8:0];
			address = ~ phase[5:0];
     		end
	endcase
end
 
endmodule