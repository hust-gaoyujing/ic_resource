/******************************************************************************
*
*    File Name:  rxblock.v
*      Version:  1.0
*         Date:  October 6, 2008
*        Model:  Uart Chip
*
*      Company:  ecnu
*     Engineer:  Xushuai(Marx)
*       E-mail:  xiaoshuai347@hotmail.com
*   Disclaimer:  this is the sub_module of the Uart Chip design,used to receive data
*
******************************************************************************/

`timescale 1 ns / 1 ns
module rxblock(clk16,
               serial_data_in,
               rst_n,
               para_data_out,
               receive_flag);

//io description
input         clk16;
input         serial_data_in;
input         rst_n;


output[7:0]   para_data_out;
output        receive_flag;

//io type


//signal wire



//signal regs

reg[9:0]  output_regs;
reg       re_flag; 
reg[7:0]  clk_sample_counter;
 
always@(negedge rst_n or negedge serial_data_in)//data detect
   begin
   if(!rst_n)
   re_flag<=1'b0;
   else if(clk_sample_counter[7:0]<=152) 
   re_flag<=1'b1;
   else
   re_flag<=1'b0;
   end
   

always@(negedge rst_n or posedge clk16)//sample data_in counter
   if(!rst_n)
   clk_sample_counter[7:0]<=4'b0000;
   else if(re_flag&&clk_sample_counter[7:0]<=152)
   clk_sample_counter[7:0]<=clk_sample_counter[7:0]+4'b0001;
   else
   clk_sample_counter[7:0]<=clk_sample_counter[7:0];
   
 always@(negedge rst_n or posedge clk16)
   begin
     if(!rst_n)
      output_regs[9:0]<=10'b0000_0000_00;
     else
       case(clk_sample_counter)
              8:   output_regs[0]<=serial_data_in;
              24:  output_regs[1]<=serial_data_in;
              40:  output_regs[2]<=serial_data_in;
              56:  output_regs[3]<=serial_data_in;
              72:  output_regs[4]<=serial_data_in;
              88:  output_regs[5]<=serial_data_in;
              104: output_regs[6]<=serial_data_in;
              120: output_regs[7]<=serial_data_in;
              136: output_regs[8]<=serial_data_in;
              152: output_regs[9]<=serial_data_in;
              default: output_regs[9:0]<=output_regs[9:0];
       endcase
    end
   
   assign receive_flag=re_flag;
   assign para_data_out[7:0]=(clk_sample_counter==153)?output_regs[8:1]:8'b0000_0000;

endmodule    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
         