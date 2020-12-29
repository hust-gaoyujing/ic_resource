/******************************************************************************
*
*    File Name:  Baud_rate_generator.v
*      Version:  1.0
*         Date:  October 1, 2008
*        Model:  Uart Chip
*
*      Company:  ecnu
*     Engineer:  Xushuai(Marx)
*       E-mail:  xiaoshuai347@hotmail.com
*   Disclaimer:  this is the sub_module of the Uart Chip design,
*                 used to generate clock
*
******************************************************************************/

`timescale 1 ns / 1 ns
module baud_rate_generator(CLK,
                           DIVISOR,
                           RST_N,
                           BAUDOUT_CLK);


//IO description
input        CLK;       //global clk
input[15:0]  DIVISOR;   //fenpinxishu 2-16~1
input        RST_N;     //reset,active low
output       BAUDOUT_CLK;//output clk

//IO type

//signal wire
  
//signal reg
reg[15:0]    counter;
reg          out_clk;
reg[15:0]    divisor_copy;
//reg[3:0]     counter_16;
//reg          clk_out;
reg          clk_baud;



always@(negedge RST_N or posedge CLK) 
   begin
        if(!RST_N)   
        divisor_copy[15:0]<=16'b0000_0000_0000_0000;        
        else
        divisor_copy[15:0]<=DIVISOR[15:0];
               
    end

always@(negedge RST_N or posedge CLK)
     begin 
      if(!RST_N)
        counter[15:0]<=16'b0000_0000_0000_0000;
      else if(counter[15:0]<DIVISOR[15:0])
        counter[15:0]<=counter[15:0]+16'b0000_0000_0000_0001;
      else 
        counter[15:0]<=16'b0000_0000_0000_0000;
     end



always@(negedge RST_N or counter[15:0])
    if(!RST_N)
      out_clk<=1'b0;
    else
      out_clk<=(counter[15:0]<=((divisor_copy[15:0]&16'b1111_1111_1111_1110)>>1))? 1'b1 : 1'b0;
      
//always@(negedge RST_N or posedge out_clk)
//      if(!RST_N)
//     counter_16[3:0]<=4'b0;
//     else
//     counter_16[3:0]<=counter_16[3:0]+4'b0001; 

//always@(negedge RST_N or counter_16[3:0])
//       begin
//       if(!RST_N)
//      clk_out<=1'b0;
//      else
//     clk_out<=(counter_16[3:0]<4'b1000)? 1'b1 : 1'b0;
//    end
 
  
assign BAUDOUT_CLK=out_clk;
endmodule
    
