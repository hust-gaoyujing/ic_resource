`timescale 1ns/1ns
module txblock_tb  ; 
 
  wire    serial_data   ; 
  reg  [7:0]  paral_data   ; 
  reg    clk16   ; 
  reg    rst_n   ; 
  wire    tx_end   ; 
  reg    tx_en   ; 
  wire   clk_tx;
  txblock  
   DUT  ( 
       .serial_data (serial_data ) ,
      .paral_data (paral_data ) ,
      .clk16 (clk16 ) ,
      .rst_n (rst_n ) ,
      .tx_end (tx_end ) ,
      .clk_tx(clk_tx),
      .tx_en (tx_en ) ); 

initial
  begin
    clk16=0;
    paral_data[7:0]=8'b1000_1110;
    rst_n=1;
    tx_en=0;
    #40
    rst_n=0;
    #40
    rst_n=1;
    #20
    tx_en=1;
      
  end

always
#20 clk16=~clk16;


endmodule

