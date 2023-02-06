module DDS_output( clk, clk_120M, rst_n, pic_dat, fre_dat, amp_dat, dac_dat , SPI_OK );
input clk, clk_120M, rst_n; //120MHz/12MHz
input [ 7: 0 ] pic_dat;
input	[ 15: 0 ] amp_dat;
input [ 23: 0 ] fre_dat;
input SPI_OK;

output reg [ 9: 0 ] dac_dat;
wire [ 9: 0 ] sin_dat, squ_dat, tri_dat;
reg [ 31: 0 ] phase_acc; //32位相位累加器，精度0.028Hz
wire SPI_OK;

always @( posedge clk_120M ) begin
    if ( SPI_OK == 1'b1 ) begin
        phase_acc <= phase_acc + fre_dat * 24'd36;   //在120MHz的主时钟时，输出对应频率的波形
    end
end
//未调幅的波形数据
lookup_tables_sin u_lookup_table_sin( .phase( phase_acc[ 31: 24 ] ), .sin_out( sin_dat ) ); //波形查找表 正弦波
lookup_tables_squ u_lookup_table_squ( .phase( phase_acc[ 31: 24 ] ), .squ_out( squ_dat ) ); //波形查找表 方波
lookup_tables_tri u_lookup_table_tri( .phase( phase_acc[ 31: 24 ] ), .tri_out( tri_dat ) ); //波形查找表 三角波

wire [ 7: 0 ] a_ver; //用于调幅的因数
reg [ 17: 0 ] sin_data; //调幅后的波形数据
reg [ 17: 0 ] squ_data; //调幅后的波形数据
reg [ 17: 0 ] tri_data; //调幅后的波形数据
amp_to_ver u_mp2ver( amp_dat, a_ver );
always @( posedge clk ) begin
    if ( SPI_OK == 1'b1 ) begin
        sin_data = sin_dat * a_ver;  //波形数据乘以调幅因数
        squ_data = squ_dat * a_ver;
        tri_data = tri_dat * a_ver;
    end
end


always @( posedge clk ) begin
    if ( SPI_OK == 1'b1 ) begin
        case ( pic_dat )
            8'd1: begin
                dac_dat = sin_data[ 17: 8 ];
            end
            8'd2: begin
                dac_dat = squ_data[ 17: 8 ];
            end
            8'd3: begin
                dac_dat = tri_data[ 17: 8 ];
            end
        endcase
    end
end
endmodule
