`timescale 1ns/1ns

module baud_rate_generator_tb  ; 
 
  reg  [15:0]  DIVISOR   ; 
  wire    BAUDOUT_CLK   ; 
  reg    RST_N   ; 
  reg    CLK   ; 
  baud_rate_generator  
   DUT  ( 
       .DIVISOR (DIVISOR ) ,
      .BAUDOUT_CLK (BAUDOUT_CLK ) ,
      .RST_N (RST_N ) ,
      .CLK (CLK ) ); 

initial
  begin
  #0
  CLK=0;
  DIVISOR=16'b0000_0000_0000_0011;
  RST_N=1;
  #10
  RST_N=0;
  #50
  RST_N=1;
  #10000
  $finish;
  end
 
 always
#5 CLK=~CLK; 
  


endmodule

