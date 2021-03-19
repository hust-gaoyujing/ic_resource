
module txblock ( clk16, paral_data, serial_data, tx_en, tx_end, rst_n, clk_tx
 );
  input [7:0] paral_data;
  input clk16, tx_en, rst_n;
  output serial_data, tx_end, clk_tx;
  wire   \clk_counter[2] , \clk_counter[1] , \clk_counter[0] ,
         \para_data_copy1[8] , N4, \para_data_copy[9] , te_flag, N5, rst_flag,
         N10, N11, N12, N13, N16, N17, N18, n4, n7, n9, n15, n16, n17, n18,
         n20, n21, n22, n23, n24, n25, n26, n28, n29, n30, n31, n32, n33, n34,
         n35, n36;
  wire   [3:0] tx_counter;

  INV2 U18 ( .I(n25), .ZN(tx_end) );
  AN4D1 U23 ( .A1(n21), .A2(n22), .A3(n23), .A4(n24), .Z(n9) );
  DFCN1 rst_flag_reg ( .CDN(n28), .CP(tx_en), .D(n26), .Q(rst_flag), .QN(n31)
         );
  DFCN2Q \clk_counter_reg[3]  ( .CDN(n28), .CP(clk16), .D(N13), .Q(clk_tx) );
  ND2D0 U30 ( .A1(N17), .A2(n4), .ZN(n17) );
  NR2D0 U31 ( .A1(n35), .A2(n34), .ZN(n36) );
  BUF1 U32 ( .I(rst_n), .Z(n28) );
  BUF1 U33 ( .I(clk_tx), .Z(n29) );
  IND2D2 U34 ( .A1(\para_data_copy[9] ), .B1(tx_end), .ZN(serial_data) );
  DFCN1Q \para_data_copy1_reg[8]  ( .CDN(n28), .CP(tx_en), .D(paral_data[7]), 
        .Q(\para_data_copy1[8] ) );
  XOR2D1 U35 ( .A1(n32), .A2(n23), .Z(N17) );
  XOR2D1 U36 ( .A1(n35), .A2(n34), .Z(N12) );
  ND2D0 U37 ( .A1(tx_counter[1]), .A2(tx_counter[0]), .ZN(n32) );
  IND2D1 U38 ( .A1(tx_counter[0]), .B1(n4), .ZN(n15) );
  ND2D0 U39 ( .A1(N16), .A2(n4), .ZN(n16) );
  ND2D0 U40 ( .A1(N18), .A2(n4), .ZN(n18) );
  NR2D0 U41 ( .A1(n32), .A2(n23), .ZN(n33) );
  INV0 U42 ( .I(n20), .ZN(n4) );
  OAI21D0 U43 ( .A1(tx_counter[2]), .A2(tx_counter[1]), .B(tx_counter[3]), 
        .ZN(N5) );
  ND2D0 U44 ( .A1(\clk_counter[1] ), .A2(\clk_counter[0] ), .ZN(n35) );
  XOR2D1 U45 ( .A1(n36), .A2(clk_tx), .Z(N13) );
  INV0 U46 ( .I(\clk_counter[2] ), .ZN(n34) );
  INV0 U47 ( .I(\clk_counter[0] ), .ZN(N10) );
  ND2D0 U48 ( .A1(te_flag), .A2(rst_flag), .ZN(n20) );
  NR2D0 U49 ( .A1(n31), .A2(te_flag), .ZN(n30) );
  NR2D0 U50 ( .A1(n9), .A2(n7), .ZN(N4) );
  INV0 U51 ( .I(n22), .ZN(tx_counter[1]) );
  INV0 U52 ( .I(n23), .ZN(tx_counter[2]) );
  INV0 U53 ( .I(n24), .ZN(tx_counter[3]) );
  INV0 U54 ( .I(n21), .ZN(tx_counter[0]) );
  ND2D0 U55 ( .A1(n7), .A2(n31), .ZN(n26) );
  INV0 U56 ( .I(tx_en), .ZN(n7) );
  DFCN1Q \clk_counter_reg[0]  ( .CDN(rst_n), .CP(clk16), .D(N10), .Q(
        \clk_counter[0] ) );
  DFCN1Q \clk_counter_reg[1]  ( .CDN(rst_n), .CP(clk16), .D(N11), .Q(
        \clk_counter[1] ) );
  DFCN1Q \clk_counter_reg[2]  ( .CDN(rst_n), .CP(clk16), .D(N12), .Q(
        \clk_counter[2] ) );
  DFF1Q te_flag_reg ( .CP(tx_en), .D(N5), .Q(te_flag) );
  DFCSN1N \tx_counter_reg[3]  ( .CDN(n30), .CP(clk_tx), .D(1'b1), .QN(n24), 
        .SDN(n18) );
  DFCSN1N \tx_counter_reg[0]  ( .CDN(n30), .CP(n29), .D(1'b1), .QN(n21), .SDN(
        n15) );
  DFCSN1N \tx_counter_reg[2]  ( .CDN(n30), .CP(clk_tx), .D(1'b1), .QN(n23), 
        .SDN(n17) );
  DFCSN1N \tx_counter_reg[1]  ( .CDN(n30), .CP(n29), .D(1'b1), .QN(n22), .SDN(
        n16) );
  DFCSN1N tx_end_flag_reg ( .CDN(rst_flag), .CP(n29), .D(1'b0), .QN(n25), 
        .SDN(n20) );
  DFCNS1Q \para_data_copy_reg[9]  ( .CDN(1'b1), .CP(n29), .D(
        \para_data_copy1[8] ), .Q(\para_data_copy[9] ), .S(N4) );
  XOR2D1 U57 ( .A1(tx_counter[1]), .A2(tx_counter[0]), .Z(N16) );
  XNR2D1 U58 ( .A1(n33), .A2(n24), .ZN(N18) );
  XOR2D1 U59 ( .A1(\clk_counter[1] ), .A2(\clk_counter[0] ), .Z(N11) );
endmodule

