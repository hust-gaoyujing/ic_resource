`timescale 1ns/1ns
module uart(clk,
            divisor,
            rst_n,
            para_data_in,
            serial_out,
            tx_en,
            tx_flag,
            serial_in,
            para_data_out,
            rx_flag,
            clk_tx);
 
 input       clk;
 input[15:0] divisor;
 input[7:0]  para_data_in;
 input       tx_en;
 input       serial_in;
 input       rst_n;
 
 output      serial_out;
 output      tx_flag;
 output[7:0] para_data_out;
 output      rx_flag;
 output      clk_tx;
 wire   baudout_clk;
 
 
 baud_rate  u1(.CLK(clk),.DIVISOR(divisor),.RST_N(rst_n),.BAUDOUT_CLK(baudout_clk));
 txblock u2 (.clk16(baudout_clk),.paral_data(para_data_in),.serial_data(serial_out),.tx_en(tx_en),.tx_end(tx_flag),.rst_n(rst_n),.clk_tx(clk_tx));
 rxblock u3 (.clk16(baudout_clk),.serial_data_in(serial_in),.rst_n(rst_n),.para_data_out(para_data_out),.receive_flag(rx_flag));
 
 endmodule