
module dll_syn ( clk_out, reset, limit, clk, clk_in, duty );
  input [3:0] limit;
  input [3:0] duty;
  input reset, clk, clk_in;
  output clk_out;
  wire   \counter[3] , \counter[2] , \counter[1] , \coua[3] , \coua[2] ,
         \coua[1] , reg_in, N24, N39, N40, N41, N42, N43, N51, N52, N53, N54,
         N55, N56, N57, N58, N60, N61, n11, n12, n13, n14, n15, n16, n17, n18,
         n19, n20, n21, n22, n23, n24, n25, n26, n28, n30, n31, n32, n33, n34,
         n35, n36, n37, n38, n39, n40, n41, n42, n43, n44, n45, n46, n47, n48,
         n49, n50, n51, n52, n53, n54, n55, n56, n57, n58, n59, n60, n61, n62,
         n63, n64, n65, n66, n67, n68, n69, n70, n71, n72, n73, n74, n75, n76,
         n77, n78, n79, n80, n81, n82, n83;
  wire   [3:0] coub;

  DFCN1 \coub_reg[3]  ( .CDN(N43), .CP(clk), .D(1'b0), .Q(coub[3]) );
  IOA21D0A U6 ( .A1(limit[3]), .A2(n53), .B(n11), .ZN(N42) );
  IAO21D0B U10 ( .A1(N53), .A2(n12), .B(n13), .ZN(n11) );
  IAO33D1A U11 ( .A1(n16), .A2(N61), .A3(n15), .B1(n14), .B2(N57), .B3(n50), 
        .ZN(n13) );
  IOA21D0A U12 ( .A1(limit[2]), .A2(n53), .B(n17), .ZN(N41) );
  IAO21D0B U13 ( .A1(N52), .A2(n12), .B(n18), .ZN(n17) );
  IAO33D1A U14 ( .A1(n16), .A2(N60), .A3(n15), .B1(n14), .B2(N56), .B3(n50), 
        .ZN(n18) );
  IOA21D0A U15 ( .A1(limit[1]), .A2(n53), .B(n19), .ZN(N40) );
  IAO21D0B U16 ( .A1(N51), .A2(n12), .B(n20), .ZN(n19) );
  IAO33D1A U17 ( .A1(n16), .A2(n48), .A3(n15), .B1(n44), .B2(N55), .B3(n50), 
        .ZN(n20) );
  IOA21D0A U18 ( .A1(limit[0]), .A2(n53), .B(n21), .ZN(N39) );
  IAO21D0B U19 ( .A1(n28), .A2(n12), .B(n22), .ZN(n21) );
  IAO33D1A U20 ( .A1(n16), .A2(N58), .A3(n15), .B1(n14), .B2(N54), .B3(n50), 
        .ZN(n22) );
  MOAI22D0 U23 ( .A1(N24), .A2(n26), .B1(n26), .B2(n24), .ZN(n12) );
  IOA31D0A U31 ( .A1(n33), .A2(reg_in), .A3(n34), .B(n35), .ZN(n32) );
  AN3D1 U32 ( .A1(reg_in), .A2(n34), .A3(n36), .Z(n35) );
  XOR2D1 U36 ( .A1(n28), .A2(n43), .Z(n40) );
  XOR2D1 U38 ( .A1(n60), .A2(limit[1]), .Z(n39) );
  XOR2D1 U40 ( .A1(n30), .A2(limit[2]), .Z(n38) );
  XOR2D1 U42 ( .A1(n31), .A2(limit[3]), .Z(n37) );
  INV0 U44 ( .I(reset), .ZN(n51) );
  INV0 U45 ( .I(reset), .ZN(n52) );
  INV0 U46 ( .I(reset), .ZN(N43) );
  BUF1 U47 ( .I(limit[1]), .Z(n41) );
  BUF1 U48 ( .I(limit[2]), .Z(n42) );
  NR2D0 U49 ( .A1(n42), .A2(n56), .ZN(n57) );
  INV0 U50 ( .I(N54), .ZN(n43) );
  INV0 U51 ( .I(n24), .ZN(n44) );
  INV0 U52 ( .I(n59), .ZN(n45) );
  INV0 U53 ( .I(n30), .ZN(n46) );
  INV0 U54 ( .I(n31), .ZN(n47) );
  INV0 U55 ( .I(\counter[1] ), .ZN(n48) );
  INV0 U56 ( .I(n48), .ZN(n49) );
  NR2D0 U57 ( .A1(n49), .A2(n46), .ZN(n54) );
  INV0 U58 ( .I(n25), .ZN(n50) );
  INV0 U59 ( .I(n25), .ZN(n15) );
  INV0 U60 ( .I(n24), .ZN(n14) );
  INV3 U61 ( .I(n58), .ZN(clk_out) );
  INV0 U62 ( .I(n69), .ZN(n58) );
  OAI21D0 U63 ( .A1(n68), .A2(n67), .B(n66), .ZN(n69) );
  AOI21D0 U64 ( .A1(n64), .A2(n63), .B(n62), .ZN(n67) );
  ND2D0 U65 ( .A1(N24), .A2(n32), .ZN(n25) );
  MAOI22D0 U66 ( .A1(n26), .A2(n14), .B1(n25), .B2(n23), .ZN(n53) );
  INV0 U67 ( .I(n32), .ZN(n26) );
  ND4D0 U68 ( .A1(n28), .A2(n48), .A3(n30), .A4(n31), .ZN(n24) );
  ND2D0 U69 ( .A1(n23), .A2(n24), .ZN(n16) );
  INV0 U70 ( .I(n72), .ZN(n71) );
  DFCN1Q \coua_reg[3]  ( .CDN(n51), .CP(clk), .D(limit[3]), .Q(\coua[3] ) );
  DFCN1Q \coua_reg[2]  ( .CDN(n52), .CP(clk), .D(limit[2]), .Q(\coua[2] ) );
  DFCN1Q \coua_reg[1]  ( .CDN(N43), .CP(clk), .D(limit[1]), .Q(\coua[1] ) );
  NR2D0 U71 ( .A1(\counter[3] ), .A2(n65), .ZN(n68) );
  NR2D0 U72 ( .A1(duty[2]), .A2(n61), .ZN(n62) );
  ND2D0 U73 ( .A1(\counter[3] ), .A2(n65), .ZN(n66) );
  OAI22D0 U74 ( .A1(duty[1]), .A2(n60), .B1(duty[0]), .B2(n59), .ZN(n63) );
  INV0 U75 ( .I(N58), .ZN(n59) );
  AOI22D0 U76 ( .A1(duty[2]), .A2(n61), .B1(duty[1]), .B2(n60), .ZN(n64) );
  INV0 U77 ( .I(duty[3]), .ZN(n65) );
  INV0 U78 ( .I(n36), .ZN(n33) );
  ND4D0 U79 ( .A1(n37), .A2(n38), .A3(n39), .A4(n40), .ZN(n36) );
  ND2D0 U80 ( .A1(n72), .A2(n70), .ZN(N51) );
  INV0 U81 ( .I(n49), .ZN(n60) );
  INV0 U82 ( .I(\counter[2] ), .ZN(n61) );
  XOR2D1 U83 ( .A1(n57), .A2(limit[3]), .Z(N57) );
  ND2D0 U84 ( .A1(n56), .A2(n55), .ZN(N55) );
  ND2D0 U85 ( .A1(n41), .A2(limit[0]), .ZN(n55) );
  XNR2D1 U86 ( .A1(n56), .A2(n42), .ZN(N56) );
  INV0 U87 ( .I(clk_in), .ZN(n34) );
  INV0 U88 ( .I(limit[0]), .ZN(N54) );
  INV0 U89 ( .I(\counter[2] ), .ZN(n30) );
  INV0 U90 ( .I(\counter[3] ), .ZN(n31) );
  INV0 U91 ( .I(n45), .ZN(n28) );
  ND4D0 U92 ( .A1(N58), .A2(n60), .A3(n30), .A4(n31), .ZN(n23) );
  NR2D0 U93 ( .A1(\counter[2] ), .A2(n76), .ZN(n77) );
  OAI22D0 U94 ( .A1(\counter[1] ), .A2(n75), .B1(n45), .B2(n74), .ZN(n78) );
  INV0 U95 ( .I(coub[0]), .ZN(n74) );
  AOI22D0 U96 ( .A1(n46), .A2(n76), .B1(\counter[1] ), .B2(n75), .ZN(n79) );
  OAI21D0 U97 ( .A1(n83), .A2(n82), .B(n81), .ZN(N24) );
  AOI21D0 U98 ( .A1(n79), .A2(n78), .B(n77), .ZN(n82) );
  ND2D0 U99 ( .A1(coub[3]), .A2(n80), .ZN(n81) );
  NR2D0 U100 ( .A1(coub[3]), .A2(n80), .ZN(n83) );
  XOR2D1 U101 ( .A1(n54), .A2(n47), .Z(N61) );
  NR2D0 U102 ( .A1(n46), .A2(n72), .ZN(n73) );
  ND2D0 U103 ( .A1(\counter[1] ), .A2(n45), .ZN(n70) );
  INV0 U104 ( .I(coub[1]), .ZN(n75) );
  INV0 U105 ( .I(\counter[3] ), .ZN(n80) );
  INV0 U106 ( .I(coub[2]), .ZN(n76) );
  DFCN1Q \counter_reg[2]  ( .CDN(n51), .CP(clk), .D(N41), .Q(\counter[2] ) );
  DFCN1Q \counter_reg[0]  ( .CDN(n51), .CP(clk), .D(N39), .Q(N58) );
  DFCN1Q \counter_reg[1]  ( .CDN(n51), .CP(clk), .D(N40), .Q(\counter[1] ) );
  DFCN1Q \counter_reg[3]  ( .CDN(N43), .CP(clk), .D(N42), .Q(\counter[3] ) );
  DFCN1Q \coub_reg[2]  ( .CDN(n52), .CP(clk), .D(\coua[3] ), .Q(coub[2]) );
  DFCNS1Q reg_in_reg ( .CDN(1'b1), .CP(clk), .D(clk_in), .Q(reg_in), .S(n52)
         );
  DFCN1Q \coub_reg[1]  ( .CDN(N43), .CP(clk), .D(\coua[2] ), .Q(coub[1]) );
  DFCN1Q \coub_reg[0]  ( .CDN(n52), .CP(clk), .D(\coua[1] ), .Q(coub[0]) );
  XNR2D1 U107 ( .A1(n46), .A2(n49), .ZN(N60) );
  OR2D1 U108 ( .A1(limit[0]), .A2(n41), .Z(n56) );
  OR2D1 U109 ( .A1(n45), .A2(n49), .Z(n72) );
  XNR2D1 U110 ( .A1(n71), .A2(n61), .ZN(N52) );
  XNR2D1 U111 ( .A1(n73), .A2(n80), .ZN(N53) );
endmodule

