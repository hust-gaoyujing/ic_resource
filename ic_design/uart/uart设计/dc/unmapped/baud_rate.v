`timescale 1 ns / 1 ns
module baud_rate(CLK,
                 DIVISOR,
                 RST_N,
                 BAUDOUT_CLK);



input        CLK;       
input[15:0]  DIVISOR;   
input        RST_N;    
output       BAUDOUT_CLK;




  

reg[15:0]    counter;
reg          out_clk;
reg[15:0]    divisor_copy;


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



always@(negedge RST_N or  posedge CLK)
    begin
    if(!RST_N)
      out_clk<=1'b0;
    else
      out_clk<=(counter[15:0]<=((divisor_copy[15:0]&16'b1111_1111_1111_1110)>>1))? 1'b1 : 1'b0;
    end  

 
  
assign BAUDOUT_CLK=out_clk;

endmodule
    
