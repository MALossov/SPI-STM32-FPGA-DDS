//~ `New testbench
`timescale  1ns / 1ps

module spi_top_tb;

// top Parameters
parameter PERIOD = 8;
parameter NOP = 0;

// top Inputs
reg clk = 0 ;
reg SPI_SCK = 0 ;
reg SPI_SS = 0 ;
reg SPI_MOSI = 0 ;

// top Outputs
wire LED_R ;
wire LED_G ;
wire LED_B ;
wire SPI_MISO ;
wire [ 3: 0 ] LED_Groups ;


initial begin
    forever
        #( PERIOD / 8 ) clk = ~clk;
end

top #(
        .NOP ( NOP ) )
    u_top (
        .clk ( clk ),
        .SPI_SCK ( SPI_SCK ),
        .SPI_SS ( SPI_SS ),
        .SPI_MOSI ( SPI_MOSI ),

        .LED_R ( LED_R ),
        .LED_G ( LED_G ),
        .LED_B ( LED_B ),
        .SPI_MISO ( SPI_MISO ),
        .LED_Groups ( LED_Groups [ 3: 0 ] )
    );

initial begin
	SPI_SS = 1;
	#(PERIOD*15) SPI_SS = 0;
	#(PERIOD * 40) forever begin
        # PERIOD SPI_SCK = ~SPI_SCK;
    end
	#(PERIOD*100) SPI_SS = 1;
	#(PERIOD*300) SPI_SS = 0;
    //SPI_MOSI = 0;
    //# (PERIOD*13) SPI_MOSI = 1;
    //# (16*PERIOD) SPI_MOSI = 0;
end

endmodule
