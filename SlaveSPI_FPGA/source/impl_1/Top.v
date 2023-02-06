module Top( input clk,
            output LED_R,
            output LED_G,
            output LED_B,
            input SPI_SCK,
            input SPI_SS,
            input SPI_MOSI,
            output SPI_MISO,
            output wire [3:0]LED_Groups ,
			output wire [9:0]DAC_d,
			output DAC_CLK);

wire [23:0] Freq_reg;
wire [7:0] WaveSet_reg;
wire [15:0] Amp_reg;
wire [9:0] dac_dat;


u_pll_120mhz u_u_pll_120mhz(
                 .ref_clk_i ( clk ),
                 .rst_n_i ( rst_n_i ),
                 .outcore_o ( outcore_o),
                 .outglobal_o ( clk_pll )
             );
spi_top u_spi_top(
            .clk ( clk ),
            .LED_R ( LED_R ),
            .LED_G ( LED_G ),
            .LED_B ( LED_B ),
            .SPI_SCK ( SPI_SCK ),
            .SPI_SS ( SPI_SS ),
            .SPI_MOSI ( SPI_MOSI ),
            .SPI_MISO ( SPI_MISO ),
            .LED_Groups ( LED_Groups ),
            .Freq_reg ( Freq_reg ),
            .WaveSet_reg ( WaveSet_reg ),
            .Amp_reg ( Amp_reg ),
            .SPI_OK ( SPI_OK )
        );

DDS_output u_dds_output(
               .clk( clk ),
               .clk_120M( clk_pll ),
               .rst_n( rst_n ),
               .pic_dat( WaveSet_reg ),
               .fre_dat( Freq_reg ),
               .amp_dat( Amp_reg ),
               .dac_dat( dac_dat ),
               .SPI_OK ( SPI_OK ) );
DAC u_dac(
        .clk( clk_pll ),
        .rst_n( rst_n ),
        .dac_dat( dac_dat ),
        .DAC_clk( DAC_clk ),
        .DAC_d( DAC_d )
    );





endmodule
