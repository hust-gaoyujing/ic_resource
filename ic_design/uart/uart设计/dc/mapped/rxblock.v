
module rxblock_DW01_inc_8_0 ( A, SUM );
  input [7:0] A;
  output [7:0] SUM;
  wire   \carry[7] , \carry[6] , \carry[5] , \carry[4] , \carry[3] ,
         \carry[2] ;

  HA1D1 U1_1_2 ( .A(A[2]), .B(\carry[2] ), .CO(\carry[3] ), .S(SUM[2]) );
  HA1D1 U1_1_3 ( .A(A[3]), .B(\carry[3] ), .CO(\carry[4] ), .S(SUM[3]) );
  HA1D1 U1_1_4 ( .A(A[4]), .B(\carry[4] ), .CO(\carry[5] ), .S(SUM[4]) );
  HA1D1 U1_1_5 ( .A(A[5]), .B(\carry[5] ), .CO(\carry[6] ), .S(SUM[5]) );
  HA1D1 U1_1_1 ( .A(A[1]), .B(A[0]), .CO(\carry[2] ), .S(SUM[1]) );
  HA1D1 U1_1_6 ( .A(A[6]), .B(\carry[6] ), .CO(\carry[7] ), .S(SUM[6]) );
  INV0 U5 ( .I(A[0]), .ZN(SUM[0]) );
  XOR2D1 U6 ( .A1(\carry[7] ), .A2(A[7]), .Z(SUM[7]) );
endmodule


module rxblock ( clk16, serial_data_in, rst_n, para_data_out, receive_flag );
  output [7:0] para_data_out;
  input clk16, serial_data_in, rst_n;
  output receive_flag;
  wire   N10, \output_regs[8] , \output_regs[7] , \output_regs[6] ,
         \output_regs[5] , \output_regs[4] , \output_regs[3] ,
         \output_regs[2] , \output_regs[1] , N51, N55, N59, N63, N67, N71, N75,
         N79, N88, N89, N90, N91, N92, N93, N94, N95, n9, n10, n13, n14, n15,
         n16, n17, n18, n20, n22, n24, n25, n26, n27, n28, n29, n30, n31, n32,
         n33, n34, n35, n36, n37, n38, n39, n40, n41, n42, n43, n44, n45, n46,
         n47, n48, n49, n50;
  wire   [7:0] clk_sample_counter;

  AN3D1 U15 ( .A1(n29), .A2(n43), .A3(n44), .Z(N79) );
  IND3D1 U18 ( .A1(clk_sample_counter[6]), .B1(n28), .B2(n44), .ZN(n13) );
  AN3D1 U19 ( .A1(n14), .A2(n15), .A3(n16), .Z(N67) );
  AN3D1 U20 ( .A1(clk_sample_counter[4]), .A2(n15), .A3(n16), .Z(N63) );
  AN3D1 U21 ( .A1(clk_sample_counter[5]), .A2(n14), .A3(n16), .Z(N59) );
  AN3D1 U22 ( .A1(clk_sample_counter[5]), .A2(n29), .A3(n16), .Z(N55) );
  IND2D1 U24 ( .A1(n18), .B1(n44), .ZN(n17) );
  AN4D1 U28 ( .A1(n22), .A2(n26), .A3(n43), .A4(n14), .Z(N51) );
  INV0 U40 ( .I(n24), .ZN(n25) );
  BUF1 U41 ( .I(clk_sample_counter[7]), .Z(n26) );
  BUF1 U42 ( .I(n38), .Z(n27) );
  INV0 U43 ( .I(n15), .ZN(n28) );
  INV0 U44 ( .I(n46), .ZN(n29) );
  AN7D1 U45 ( .A1(n9), .A2(n10), .A3(n25), .A4(n43), .A5(clk_sample_counter[7]), .A6(clk_sample_counter[3]), .A7(clk_sample_counter[4]), .Z(n30) );
  INV0 U46 ( .I(n30), .ZN(n31) );
  INV0 U47 ( .I(n30), .ZN(n32) );
  INV0 U48 ( .I(serial_data_in), .ZN(n33) );
  INV0 U49 ( .I(n33), .ZN(n34) );
  INV0 U50 ( .I(n33), .ZN(n35) );
  INV0 U51 ( .I(rst_n), .ZN(n36) );
  INV0 U52 ( .I(n36), .ZN(n37) );
  INV0 U53 ( .I(n36), .ZN(n38) );
  INV0 U54 ( .I(n36), .ZN(n39) );
  INR2D2 U55 ( .A1(\output_regs[8] ), .B1(n31), .ZN(para_data_out[7]) );
  INR2D2 U56 ( .A1(\output_regs[7] ), .B1(n32), .ZN(para_data_out[6]) );
  INR2D2 U57 ( .A1(\output_regs[6] ), .B1(n31), .ZN(para_data_out[5]) );
  INR2D2 U58 ( .A1(\output_regs[5] ), .B1(n32), .ZN(para_data_out[4]) );
  INR2D2 U59 ( .A1(\output_regs[4] ), .B1(n31), .ZN(para_data_out[3]) );
  INR2D2 U60 ( .A1(\output_regs[3] ), .B1(n31), .ZN(para_data_out[2]) );
  ND2D0 U61 ( .A1(receive_flag), .A2(N10), .ZN(n40) );
  INV0 U62 ( .I(n40), .ZN(n41) );
  INV0 U63 ( .I(n40), .ZN(n42) );
  INR2D2 U64 ( .A1(\output_regs[2] ), .B1(n32), .ZN(para_data_out[1]) );
  DFCN2 re_flag_reg ( .CDN(n37), .CP(n33), .D(N10), .Q(receive_flag) );
  INR2D2 U65 ( .A1(\output_regs[1] ), .B1(n32), .ZN(para_data_out[0]) );
  AN2D1 U66 ( .A1(n18), .A2(n15), .Z(n43) );
  INV0 U67 ( .I(n17), .ZN(n16) );
  NR2D0 U68 ( .A1(n14), .A2(n13), .ZN(N71) );
  INV0 U69 ( .I(clk_sample_counter[5]), .ZN(n15) );
  INV0 U70 ( .I(clk_sample_counter[1]), .ZN(n9) );
  INV0 U71 ( .I(clk_sample_counter[2]), .ZN(n10) );
  INV0 U72 ( .I(clk_sample_counter[6]), .ZN(n18) );
  ND4D0 U73 ( .A1(clk_sample_counter[3]), .A2(n24), .A3(n9), .A4(n10), .ZN(n20) );
  INV0 U74 ( .I(clk_sample_counter[0]), .ZN(n24) );
  NR3D0 U75 ( .A1(clk_sample_counter[0]), .A2(clk_sample_counter[2]), .A3(
        clk_sample_counter[1]), .ZN(n47) );
  NR2D0 U76 ( .A1(n20), .A2(n26), .ZN(n44) );
  OAI21D0 U77 ( .A1(n50), .A2(n49), .B(clk_sample_counter[7]), .ZN(N10) );
  ND2D0 U78 ( .A1(n48), .A2(n18), .ZN(n49) );
  NR3D0 U79 ( .A1(n47), .A2(n46), .A3(n45), .ZN(n50) );
  NR2D0 U80 ( .A1(n29), .A2(n13), .ZN(N75) );
  INV0 U81 ( .I(n29), .ZN(n14) );
  INV0 U82 ( .I(n20), .ZN(n22) );
  INV0 U83 ( .I(clk_sample_counter[4]), .ZN(n46) );
  INV0 U84 ( .I(clk_sample_counter[3]), .ZN(n45) );
  INV0 U85 ( .I(n28), .ZN(n48) );
  DFCNS1Q \clk_sample_counter_reg[5]  ( .CDN(rst_n), .CP(clk16), .D(N93), .Q(
        clk_sample_counter[5]), .S(n42) );
  DFCNS1Q \clk_sample_counter_reg[4]  ( .CDN(rst_n), .CP(clk16), .D(N92), .Q(
        clk_sample_counter[4]), .S(n41) );
  DFCNS1Q \clk_sample_counter_reg[0]  ( .CDN(n37), .CP(clk16), .D(N88), .Q(
        clk_sample_counter[0]), .S(n42) );
  DFCNS1Q \clk_sample_counter_reg[7]  ( .CDN(n39), .CP(clk16), .D(N95), .Q(
        clk_sample_counter[7]), .S(n41) );
  DFCNS1Q \clk_sample_counter_reg[6]  ( .CDN(n39), .CP(clk16), .D(N94), .Q(
        clk_sample_counter[6]), .S(n41) );
  DFCNS1Q \clk_sample_counter_reg[3]  ( .CDN(n37), .CP(clk16), .D(N91), .Q(
        clk_sample_counter[3]), .S(n42) );
  DFCNS1Q \clk_sample_counter_reg[1]  ( .CDN(n27), .CP(clk16), .D(N89), .Q(
        clk_sample_counter[1]), .S(n42) );
  DFCNS1Q \clk_sample_counter_reg[2]  ( .CDN(n39), .CP(clk16), .D(N90), .Q(
        clk_sample_counter[2]), .S(n41) );
  DFCNS1Q \output_regs_reg[1]  ( .CDN(n37), .CP(clk16), .D(n34), .Q(
        \output_regs[1] ), .S(N79) );
  DFCNS1Q \output_regs_reg[2]  ( .CDN(n38), .CP(clk16), .D(n35), .Q(
        \output_regs[2] ), .S(N75) );
  DFCNS1Q \output_regs_reg[3]  ( .CDN(n27), .CP(clk16), .D(serial_data_in), 
        .Q(\output_regs[3] ), .S(N71) );
  DFCNS1Q \output_regs_reg[4]  ( .CDN(n38), .CP(clk16), .D(n35), .Q(
        \output_regs[4] ), .S(N67) );
  DFCNS1Q \output_regs_reg[5]  ( .CDN(n39), .CP(clk16), .D(n34), .Q(
        \output_regs[5] ), .S(N63) );
  DFCNS1Q \output_regs_reg[6]  ( .CDN(n38), .CP(clk16), .D(n35), .Q(
        \output_regs[6] ), .S(N59) );
  DFCNS1Q \output_regs_reg[7]  ( .CDN(rst_n), .CP(clk16), .D(serial_data_in), 
        .Q(\output_regs[7] ), .S(N55) );
  DFCNS1Q \output_regs_reg[8]  ( .CDN(n27), .CP(clk16), .D(n34), .Q(
        \output_regs[8] ), .S(N51) );
  rxblock_DW01_inc_8_0 add_59 ( .A(clk_sample_counter), .SUM({N95, N94, N93, 
        N92, N91, N90, N89, N88}) );
endmodule

