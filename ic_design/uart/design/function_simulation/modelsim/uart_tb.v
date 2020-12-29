`timescale  1ns/1ns

module uart_tb  ; 
 
  wire  [7:0]  para_data_out   ; 
  reg[15:0]    divisor   ; 
  wire    tx_flag   ; 
  reg    serial_in   ; 
  reg    tx_en   ; 
  reg    rst_n   ; 
  reg    clk   ; 
  wire    rx_flag   ; 
  wire    serial_out   ; 
  wire    clk_tx;
  reg  [7:0]  para_data_in   ; 
  uart  
   DUT  ( 
       .para_data_out (para_data_out ) ,
      .divisor (divisor ) ,
      .tx_flag (tx_flag ) ,
      .serial_in (serial_in ) ,
      .tx_en (tx_en ) ,
      .rst_n (rst_n ) ,
      .clk (clk ) ,
      .rx_flag (rx_flag ) ,
      .serial_out (serial_out ) ,
      .clk_tx(clk_tx),
      .para_data_in (para_data_in ) ); 
initial
  begin
  clk=0;
  rst_n=1;
  tx_en=1'b0;
  divisor=16'b0000_0000_0000_0011;
  serial_in=1;
  para_data_in[7:0]=8'b10001110;
  #200
  rst_n=0;
  #2000
  rst_n=1;
  #2000
  tx_en=1'b1;
  
  
  
  end
always
#5 clk=~clk;

endmodule

