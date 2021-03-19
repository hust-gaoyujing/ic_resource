`timescale 1ns/1ns
module rxblock_tb  ; 
 
  wire    receive_flag   ; 
  wire  [7:0]  para_data_out   ; 
  reg    clk16   ; 
  reg    rst_n   ; 
  reg    serial_data_in   ; 
  rxblock  
   DUT  ( 
       .receive_flag (receive_flag ) ,
      .para_data_out (para_data_out ) ,
      .clk16 (clk16 ) ,
      .rst_n (rst_n ) ,
      .serial_data_in (serial_data_in ) ); 

initial
  begin
      clk16=0;
      rst_n=1;
      serial_data_in=1'b1;
      #20
      rst_n=0;
      #20
      rst_n=1;
      #10
      serial_data_in=1'b0;
      #160
      serial_data_in=1'b1;
      #160
      serial_data_in=1'b0;
      #160
      serial_data_in=1'b0;
      #160
      serial_data_in=1'b1;
      #160
      serial_data_in=1'b1;
      #160
      serial_data_in=1'b0;
      #160
      serial_data_in=1'b1;
      #160
      serial_data_in=1'b0;
      #160
      serial_data_in=1'b1;
      #160
      serial_data_in=1'b0;
   end

always
#5  clk16=~clk16;
endmodule

