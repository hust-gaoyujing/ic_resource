/******************************************************************************
*
*    File Name:  txblock.v
*      Version:  1.0
*         Date:  October 4, 2008
*        Model:  Uart Chip
*
*      Company:  ecnu
*     Engineer:  Xushuai(Marx)
*       E-mail:  xiaoshuai347@hotmail.com
*   Disclaimer:  this is the sub_module of the Uart Chip design,used to transmit data
*
******************************************************************************/
`timescale 1 ns / 1 ns
module txblock(clk16,
               paral_data,
               serial_data,
               tx_en,
               tx_end,
               rst_n,
               clk_tx);
//io description
input      clk16;
input[7:0] paral_data;
input      tx_en;
input      rst_n;

output     serial_data;
output     tx_end;
output     clk_tx;
//io type


//signal wire
wire      serial_clk;
//signal regs
reg[3:0]  clk_counter;
reg[9:0]  para_data_copy;      
reg[3:0]  tx_counter;
reg       tx_end_flag;
reg        te_flag;
reg        rst_flag;
reg[9:0]   para_data_copy1;

always@(negedge rst_n or posedge clk16)
  begin
   if(!rst_n)
   clk_counter[3:0]<=4'b0000;
   else
   clk_counter[3:0]<=clk_counter[3:0]+4'b0001;
  end 
   
assign serial_clk=clk_counter[3];   

always@(negedge rst_n or posedge tx_en)//para_data latch
  begin
   if(!rst_n)
   para_data_copy1[9:0]<=10'b0_0000_0000_0;
   else
   para_data_copy1[9:0]<={1'b0,paral_data[7:0],1'b1};
  end    

always@(posedge serial_clk)
   begin
    if(tx_en&&tx_counter[3:0]>=4'b0001)
    para_data_copy[9:0]<=para_data_copy1[9:0]<<1;  
    else
    para_data_copy[9:0]<=para_data_copy[9:0];
   end

  always@(posedge tx_en)
      begin
      if(tx_counter[3:0]<4'b1010)
      te_flag<=1'b1;
      else
      te_flag<=1'b0;
      end
  always@(negedge rst_n or posedge tx_en)
    begin
    if(!rst_n)
    rst_flag<=1'b0;
    else if(tx_en)
    rst_flag<=1'b1;
    else
    rst_flag<=rst_flag;
    end
  always@(posedge serial_clk  or negedge rst_flag or posedge te_flag)
   begin
     if(!rst_flag)
      begin
     tx_counter[3:0]<=4'b0000;
     tx_end_flag<=1'b0;
      end
     else if(te_flag)
      begin
     tx_counter[3:0]<=tx_counter[3:0]+1'b1;
     tx_end_flag<=1'b1;
      end
     else
      begin
     tx_counter[3:0]<=4'b1111;
     tx_end_flag<=1'b0;
      end 
   end   
  

assign serial_data=tx_end_flag?para_data_copy[9]:1'b1;
assign tx_end=tx_end_flag;
assign clk_tx=serial_clk;
endmodule  

  





