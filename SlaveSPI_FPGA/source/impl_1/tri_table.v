module tri_table(address,tria);
output [8:0] tria;         //实际波形表为9位分辨率（1/4周期）
input  [5:0] address;     //64个点来生成1/4个周期的波形，完整的一个周期为256个点
 
reg    [8:0] tria;
 
always @(address)
	begin
                  case(address)	
                      6'h0: tria=9'h7;
                      6'h1: tria=9'hF;
                      6'h2: tria=9'h17;
                      6'h3: tria=9'h1F;
                      6'h4: tria=9'h27;
                      6'h5: tria=9'h2F;
                      6'h6: tria=9'h37;
                      6'h7: tria=9'h3F;
                      6'h8: tria=9'h47;
                      6'h9: tria=9'h4F;
                      6'ha: tria=9'h57;
                      6'hb: tria=9'h5F;
                      6'hc: tria=9'h67;
                      6'hd: tria=9'h6F;
                      6'he: tria=9'h77;
                      6'hf: tria=9'h7F;
                      6'h10: tria=9'h87;
                      6'h11: tria=9'h8F;
                      6'h12: tria=9'h97;
                      6'h13: tria=9'h9F;
                      6'h14: tria=9'hA7;
                      6'h15: tria=9'hAF;
                      6'h16: tria=9'hB7;
                      6'h17: tria=9'hBF;
                      6'h18: tria=9'hC7;
                      6'h19: tria=9'hCF;
                      6'h1a: tria=9'hD7;
                      6'h1b: tria=9'hDF;
                      6'h1c: tria=9'hE7;
                      6'h1d: tria=9'hEF;
                      6'h1e: tria=9'hF7;
                      6'h1f: tria=9'hFF;
                      6'h20: tria=9'h107;
                      6'h21: tria=9'h10F;
                      6'h22: tria=9'h117;
                      6'h23: tria=9'h11F;
                      6'h24: tria=9'h127;
                      6'h25: tria=9'h12F;
                      6'h26: tria=9'h137;
                      6'h27: tria=9'h13F;
                      6'h28: tria=9'h147;
                      6'h29: tria=9'h14F;
                      6'h2a: tria=9'h157;
                      6'h2b: tria=9'h15F;
                      6'h2c: tria=9'h167;
                      6'h2d: tria=9'h16F;
                      6'h2e: tria=9'h177;
                      6'h2f: tria=9'h17F;
                      6'h30: tria=9'h187;
                      6'h31: tria=9'h18F;
                      6'h32: tria=9'h197;
                      6'h33: tria=9'h19F;
                      6'h34: tria=9'h1A7;
                      6'h35: tria=9'h1AF;
                      6'h36: tria=9'h1B7;
                      6'h37: tria=9'h1BF;
                      6'h38: tria=9'h1C7;
                      6'h39: tria=9'h1CF;
                      6'h3a: tria=9'h1D7;
                      6'h3b: tria=9'h1DF;
                      6'h3c: tria=9'h1E7;
                      6'h3d: tria=9'h1EF;
                      6'h3e: tria=9'h1F7;
                      6'h3f: tria=9'h1FF;
                   endcase
              end
endmodule