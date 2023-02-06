module amp_to_ver( amp_dat, a_ver );
input [ 15: 0 ] amp_dat;
output reg [ 7: 0 ] a_ver;
always@( amp_dat ) begin

    a_ver <= amp_dat * 25 / 100;

    case ( amp_dat[ 15: 8 ] )
        8'd0: begin
            a_ver <= a_ver + 8'd0;
        end
        8'd1: begin
            a_ver <= a_ver + 8'd1;
        end
        8'd2: begin
            a_ver <= a_ver + 8'd2;
        end
        8'd3: begin
            a_ver <= a_ver + 8'd3;
        end
        8'd4: begin
            a_ver <= a_ver + 8'd4;
        end
        8'd5: begin
            a_ver <= a_ver + 8'd5;
        end
        8'd6: begin
            a_ver <= a_ver + 8'd6;
        end
        8'd7: begin
            a_ver <= a_ver + 8'd7;
        end
    endcase

end

endmodule
