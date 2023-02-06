module squ_table(address,squ);
output [8:0] squ;         //实际波形表为9位分辨率（1/4周期）
input  [5:0] address;     //64个点来生成1/4个周期的波形，完整的一个周期为256个点
 
reg    [8:0] squ;
 
always @(address)
	begin
                  case(address)	
                      6'h0: squ=9'h1FF;
                      6'h1: squ=9'h1FF;
                      6'h2: squ=9'h1FF;
                      6'h3: squ=9'h1FF;
                      6'h4: squ=9'h1FF;
                      6'h5: squ=9'h1FF;
                      6'h6: squ=9'h1FF;
                      6'h7: squ=9'h1FF;
                      6'h8: squ=9'h1FF;
                      6'h9: squ=9'h1FF;
                      6'ha: squ=9'h1FF;
                      6'hb: squ=9'h1FF;
                      6'hc: squ=9'h1FF;
                      6'hd: squ=9'h1FF;
                      6'he: squ=9'h1FF;
                      6'hf: squ=9'h1FF;
                      6'h10: squ=9'h1FF;
                      6'h11: squ=9'h1FF;
                      6'h12: squ=9'h1FF;
                      6'h13: squ=9'h1FF;
                      6'h14: squ=9'h1FF;
                      6'h15: squ=9'h1FF;
                      6'h16: squ=9'h1FF;
                      6'h17: squ=9'h1FF;
                      6'h18: squ=9'h1FF;
                      6'h19: squ=9'h1FF;
                      6'h1a: squ=9'h1FF;
                      6'h1b: squ=9'h1FF;
                      6'h1c: squ=9'h1FF;
                      6'h1d: squ=9'h1FF;
                      6'h1e: squ=9'h1FF;
                      6'h1f: squ=9'h1FF;
                      6'h20: squ=9'h1FF;
                      6'h21: squ=9'h1FF;
                      6'h22: squ=9'h1FF;
                      6'h23: squ=9'h1FF;
                      6'h24: squ=9'h1FF;
                      6'h25: squ=9'h1FF;
                      6'h26: squ=9'h1FF;
                      6'h27: squ=9'h1FF;
                      6'h28: squ=9'h1FF;
                      6'h29: squ=9'h1FF;
                      6'h2a: squ=9'h1FF;
                      6'h2b: squ=9'h1FF;
                      6'h2c: squ=9'h1FF;
                      6'h2d: squ=9'h1FF;
                      6'h2e: squ=9'h1FF;
                      6'h2f: squ=9'h1FF;
                      6'h30: squ=9'h1FF;
                      6'h31: squ=9'h1FF;
                      6'h32: squ=9'h1FF;
                      6'h33: squ=9'h1FF;
                      6'h34: squ=9'h1FF;
                      6'h35: squ=9'h1FF;
                      6'h36: squ=9'h1FF;
                      6'h37: squ=9'h1FF;
                      6'h38: squ=9'h1FF;
                      6'h39: squ=9'h1FF;
                      6'h3a: squ=9'h1FF;
                      6'h3b: squ=9'h1FF;
                      6'h3c: squ=9'h1FF;
                      6'h3d: squ=9'h1FF;
                      6'h3e: squ=9'h1FF;
                      6'h3f: squ=9'h1FF;
                   endcase
              end
endmodule