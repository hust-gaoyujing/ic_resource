module baud_rate_DW01_cmp2_16_1 (
	A,
	B,
	LT_LE );
   input [15:0] A;
   input [15:0] B;
   output LT_LE;

   // Internal wires
   wire n15;
   wire n16;
   wire n17;
   wire n18;
   wire n19;
   wire n20;
   wire n21;
   wire n22;
   wire n23;
   wire n24;
   wire n25;
   wire n26;
   wire n27;
   wire n28;
   wire n29;
   wire n30;
   wire n31;
   wire n32;
   wire n33;
   wire n34;
   wire n35;
   wire n36;
   wire n37;
   wire n38;
   wire n39;
   wire n40;
   wire n41;
   wire n42;
   wire n43;
   wire n44;
   wire n45;
   wire n46;
   wire n47;
   wire n48;
   wire n49;
   wire n50;
   wire n51;
   wire n52;
   wire n53;
   wire n54;
   wire n55;
   wire n56;

   NR3D0 U6 ( .ZN (n18),
	.A3 (n26),
	.A2 (n25),
	.A1 (n24) );
   NR2D0 U7 ( .ZN (n25),
	.A2 (n28),
	.A1 (B[12]) );
   NR2D0 U8 ( .ZN (n26),
	.A2 (n27),
	.A1 (B[11]) );
   AOI222D0 U9 ( .ZN (n24),
	.C2 (n27),
	.C1 (B[11]),
	.B2 (n31),
	.B1 (B[10]),
	.A2 (n30),
	.A1 (n29) );
   OAI211D0 U10 ( .ZN (n48),
	.C (n51),
	.B (A[0]),
	.A2 (n50),
	.A1 (A[1]) );
   INV0 U11 ( .ZN (n51),
	.I (B[0]) );
   OAI21D0 U12 ( .ZN (n39),
	.B (n43),
	.A2 (n42),
	.A1 (n41) );
   OAI22D0 U13 ( .ZN (n42),
	.B2 (n44),
	.B1 (B[4]),
	.A2 (n46),
	.A1 (B[3]) );
   AOI22D0 U14 ( .ZN (n43),
	.B2 (n45),
	.B1 (B[5]),
	.A2 (n44),
	.A1 (B[4]) );
   AOI222D0 U15 ( .ZN (n41),
	.C2 (n46),
	.C1 (B[3]),
	.B2 (n49),
	.B1 (B[2]),
	.A2 (n48),
	.A1 (n47) );
   OAI21D0 U16 ( .ZN (n30),
	.B (n34),
	.A2 (n33),
	.A1 (n32) );
   OAI22D0 U17 ( .ZN (n33),
	.B2 (n35),
	.B1 (B[8]),
	.A2 (n37),
	.A1 (B[7]) );
   AOI22D0 U18 ( .ZN (n34),
	.B2 (n36),
	.B1 (B[9]),
	.A2 (n35),
	.A1 (B[8]) );
   AOI222D0 U19 ( .ZN (n32),
	.C2 (n37),
	.C1 (B[7]),
	.B2 (n40),
	.B1 (B[6]),
	.A2 (n39),
	.A1 (n38) );
   AOI22D0 U20 ( .ZN (n47),
	.B2 (n50),
	.B1 (A[1]),
	.A2 (n52),
	.A1 (A[2]) );
   INV0 U21 ( .ZN (n52),
	.I (B[2]) );
   AOI22D0 U22 ( .ZN (n38),
	.B2 (n54),
	.B1 (A[5]),
	.A2 (n53),
	.A1 (A[6]) );
   INV0 U23 ( .ZN (n53),
	.I (B[6]) );
   INV0 U24 ( .ZN (n54),
	.I (B[5]) );
   AOI22D0 U25 ( .ZN (n29),
	.B2 (n56),
	.B1 (A[10]),
	.A2 (n55),
	.A1 (A[9]) );
   INV0 U26 ( .ZN (n55),
	.I (B[9]) );
   INV0 U27 ( .ZN (n56),
	.I (B[10]) );
   INV0 U28 ( .ZN (n46),
	.I (A[3]) );
   INV0 U29 ( .ZN (n37),
	.I (A[7]) );
   INV0 U30 ( .ZN (n44),
	.I (A[4]) );
   INV0 U31 ( .ZN (n35),
	.I (A[8]) );
   INV0 U32 ( .ZN (n50),
	.I (B[1]) );
   AOI21D0 U33 ( .ZN (LT_LE),
	.B (A[15]),
	.A2 (n16),
	.A1 (n15) );
   ND2D0 U34 ( .ZN (n16),
	.A2 (n17),
	.A1 (B[14]) );
   OAI21D0 U35 ( .ZN (n15),
	.B (n20),
	.A2 (n19),
	.A1 (n18) );
   INV0 U36 ( .ZN (n17),
	.I (A[14]) );
   INV0 U37 ( .ZN (n27),
	.I (A[11]) );
   INV0 U38 ( .ZN (n45),
	.I (A[5]) );
   INV0 U39 ( .ZN (n36),
	.I (A[9]) );
   INV0 U40 ( .ZN (n31),
	.I (A[10]) );
   INV0 U41 ( .ZN (n49),
	.I (A[2]) );
   INV0 U42 ( .ZN (n40),
	.I (A[6]) );
   INV0 U43 ( .ZN (n28),
	.I (A[12]) );
   OAI22D0 U44 ( .ZN (n19),
	.B2 (n23),
	.B1 (A[12]),
	.A2 (n22),
	.A1 (A[13]) );
   INV0 U45 ( .ZN (n23),
	.I (B[12]) );
   AOI22D0 U46 ( .ZN (n20),
	.B2 (n22),
	.B1 (A[13]),
	.A2 (n21),
	.A1 (A[14]) );
   INV0 U47 ( .ZN (n21),
	.I (B[14]) );
   INV0 U48 ( .ZN (n22),
	.I (B[13]) );
endmodule

module baud_rate_DW01_inc_16_0 (
	A,
	SUM );
   input [15:0] A;
   output [15:0] SUM;

   // Internal wires
   wire \carry[15] ;
   wire \carry[14] ;
   wire \carry[13] ;
   wire \carry[12] ;
   wire \carry[11] ;
   wire \carry[10] ;
   wire \carry[9] ;
   wire \carry[8] ;
   wire \carry[7] ;
   wire \carry[6] ;
   wire \carry[5] ;
   wire \carry[4] ;
   wire \carry[3] ;
   wire \carry[2] ;

   INV0 U5 ( .ZN (SUM[0]),
	.I (A[0]) );
   HA1D1 U1_1_2 ( .S (SUM[2]),
	.CO (\carry[3] ),
	.B (\carry[2] ),
	.A (A[2]) );
   HA1D1 U1_1_3 ( .S (SUM[3]),
	.CO (\carry[4] ),
	.B (\carry[3] ),
	.A (A[3]) );
   HA1D1 U1_1_4 ( .S (SUM[4]),
	.CO (\carry[5] ),
	.B (\carry[4] ),
	.A (A[4]) );
   HA1D1 U1_1_5 ( .S (SUM[5]),
	.CO (\carry[6] ),
	.B (\carry[5] ),
	.A (A[5]) );
   HA1D1 U1_1_6 ( .S (SUM[6]),
	.CO (\carry[7] ),
	.B (\carry[6] ),
	.A (A[6]) );
   HA1D1 U1_1_7 ( .S (SUM[7]),
	.CO (\carry[8] ),
	.B (\carry[7] ),
	.A (A[7]) );
   HA1D1 U1_1_8 ( .S (SUM[8]),
	.CO (\carry[9] ),
	.B (\carry[8] ),
	.A (A[8]) );
   HA1D1 U1_1_9 ( .S (SUM[9]),
	.CO (\carry[10] ),
	.B (\carry[9] ),
	.A (A[9]) );
   HA1D1 U1_1_10 ( .S (SUM[10]),
	.CO (\carry[11] ),
	.B (\carry[10] ),
	.A (A[10]) );
   HA1D1 U1_1_11 ( .S (SUM[11]),
	.CO (\carry[12] ),
	.B (\carry[11] ),
	.A (A[11]) );
   HA1D1 U1_1_12 ( .S (SUM[12]),
	.CO (\carry[13] ),
	.B (\carry[12] ),
	.A (A[12]) );
   HA1D1 U1_1_13 ( .S (SUM[13]),
	.CO (\carry[14] ),
	.B (\carry[13] ),
	.A (A[13]) );
   HA1D1 U1_1_1 ( .S (SUM[1]),
	.CO (\carry[2] ),
	.B (A[0]),
	.A (A[1]) );
   HA1D1 U1_1_14 ( .S (SUM[14]),
	.CO (\carry[15] ),
	.B (\carry[14] ),
	.A (A[14]) );
   XOR2D1 U6 ( .Z (SUM[15]),
	.A2 (A[15]),
	.A1 (\carry[15] ) );
endmodule

module baud_rate_DW01_cmp2_16_0 (
	A,
	B,
	LT_LE );
   input [15:0] A;
   input [15:0] B;
   output LT_LE;

   // Internal wires
   wire n16;
   wire n17;
   wire n18;
   wire n19;
   wire n20;
   wire n21;
   wire n22;
   wire n23;
   wire n24;
   wire n25;
   wire n26;
   wire n27;
   wire n28;
   wire n29;
   wire n30;
   wire n31;
   wire n32;
   wire n33;
   wire n34;
   wire n35;
   wire n36;
   wire n37;
   wire n38;
   wire n39;
   wire n40;
   wire n41;
   wire n42;
   wire n43;
   wire n44;
   wire n45;
   wire n46;
   wire n47;
   wire n48;
   wire n49;
   wire n50;
   wire n51;
   wire n52;
   wire n53;
   wire n54;
   wire n55;
   wire n56;
   wire n57;
   wire n58;
   wire n59;

   AOI222D0 U6 ( .ZN (n33),
	.C2 (n38),
	.C1 (B[7]),
	.B2 (n41),
	.B1 (B[6]),
	.A2 (n40),
	.A1 (n39) );
   INV0 U7 ( .ZN (n41),
	.I (A[6]) );
   AOI22D0 U8 ( .ZN (n39),
	.B2 (n56),
	.B1 (A[5]),
	.A2 (n55),
	.A1 (A[6]) );
   OAI211D0 U9 ( .ZN (n40),
	.C (n45),
	.B (n44),
	.A2 (n43),
	.A1 (n42) );
   AOI222D0 U10 ( .ZN (n24),
	.C2 (n29),
	.C1 (B[11]),
	.B2 (n32),
	.B1 (B[10]),
	.A2 (n31),
	.A1 (n30) );
   INV0 U11 ( .ZN (n32),
	.I (A[10]) );
   AOI22D0 U12 ( .ZN (n30),
	.B2 (n58),
	.B1 (A[10]),
	.A2 (n57),
	.A1 (A[9]) );
   OAI21D0 U13 ( .ZN (n31),
	.B (n35),
	.A2 (n34),
	.A1 (n33) );
   AOI222D0 U14 ( .ZN (n42),
	.C2 (n48),
	.C1 (B[3]),
	.B2 (n51),
	.B1 (B[2]),
	.A2 (n50),
	.A1 (n49) );
   INV0 U15 ( .ZN (n51),
	.I (A[2]) );
   OAI22D0 U16 ( .ZN (n50),
	.B2 (n53),
	.B1 (A[0]),
	.A2 (n52),
	.A1 (A[1]) );
   AOI22D0 U17 ( .ZN (n49),
	.B2 (n52),
	.B1 (A[1]),
	.A2 (n54),
	.A1 (A[2]) );
   ND2D0 U18 ( .ZN (n44),
	.A2 (n47),
	.A1 (B[5]) );
   INV0 U19 ( .ZN (n47),
	.I (A[5]) );
   ND2D0 U20 ( .ZN (n45),
	.A2 (n46),
	.A1 (B[4]) );
   OAI22D0 U21 ( .ZN (n34),
	.B2 (n36),
	.B1 (B[8]),
	.A2 (n38),
	.A1 (B[7]) );
   OAI22D0 U22 ( .ZN (n43),
	.B2 (n46),
	.B1 (B[4]),
	.A2 (n48),
	.A1 (B[3]) );
   AOI21D0 U23 ( .ZN (n17),
	.B (n22),
	.A2 (n21),
	.A1 (n20) );
   AOI22D0 U24 ( .ZN (n20),
	.B2 (n59),
	.B1 (A[13]),
	.A2 (n23),
	.A1 (A[14]) );
   NR2D0 U25 ( .ZN (n22),
	.A2 (n23),
	.A1 (A[14]) );
   OAI21D0 U26 ( .ZN (n21),
	.B (n26),
	.A2 (n25),
	.A1 (n24) );
   OAI21D0 U27 ( .ZN (LT_LE),
	.B (n18),
	.A2 (n17),
	.A1 (n16) );
   ND2D0 U28 ( .ZN (n18),
	.A2 (n19),
	.A1 (B[15]) );
   INV0 U29 ( .ZN (n52),
	.I (B[1]) );
   INV0 U30 ( .ZN (n54),
	.I (B[2]) );
   INV0 U31 ( .ZN (n55),
	.I (B[6]) );
   INV0 U32 ( .ZN (n56),
	.I (B[5]) );
   INV0 U33 ( .ZN (n53),
	.I (B[0]) );
   NR2D0 U34 ( .ZN (n16),
	.A2 (n19),
	.A1 (B[15]) );
   OAI22D0 U35 ( .ZN (n25),
	.B2 (n27),
	.B1 (B[12]),
	.A2 (n29),
	.A1 (B[11]) );
   AOI22D0 U36 ( .ZN (n26),
	.B2 (n28),
	.B1 (B[13]),
	.A2 (n27),
	.A1 (B[12]) );
   INV0 U37 ( .ZN (n28),
	.I (A[13]) );
   AOI22D0 U38 ( .ZN (n35),
	.B2 (n37),
	.B1 (B[9]),
	.A2 (n36),
	.A1 (B[8]) );
   INV0 U39 ( .ZN (n37),
	.I (A[9]) );
   INV0 U40 ( .ZN (n23),
	.I (B[14]) );
   INV0 U41 ( .ZN (n57),
	.I (B[9]) );
   INV0 U42 ( .ZN (n58),
	.I (B[10]) );
   INV0 U43 ( .ZN (n59),
	.I (B[13]) );
   INV0 U44 ( .ZN (n27),
	.I (A[12]) );
   INV0 U45 ( .ZN (n48),
	.I (A[3]) );
   INV0 U46 ( .ZN (n38),
	.I (A[7]) );
   INV0 U47 ( .ZN (n29),
	.I (A[11]) );
   INV0 U48 ( .ZN (n36),
	.I (A[8]) );
   INV0 U49 ( .ZN (n46),
	.I (A[4]) );
   INV0 U50 ( .ZN (n19),
	.I (A[15]) );
endmodule

module baud_rate (
	CLK,
	DIVISOR,
	RST_N,
	BAUDOUT_CLK );
   input CLK;
   input [15:0] DIVISOR;
   input RST_N;
   output BAUDOUT_CLK;

   // Internal wires
   wire \divisor_copy[15] ;
   wire \divisor_copy[14] ;
   wire \divisor_copy[13] ;
   wire \divisor_copy[12] ;
   wire \divisor_copy[11] ;
   wire \divisor_copy[10] ;
   wire \divisor_copy[9] ;
   wire \divisor_copy[8] ;
   wire \divisor_copy[7] ;
   wire \divisor_copy[6] ;
   wire \divisor_copy[5] ;
   wire \divisor_copy[4] ;
   wire \divisor_copy[3] ;
   wire \divisor_copy[2] ;
   wire \divisor_copy[1] ;
   wire N3;
   wire N5;
   wire N6;
   wire N7;
   wire N8;
   wire N9;
   wire N10;
   wire N11;
   wire N12;
   wire N13;
   wire N14;
   wire N15;
   wire N16;
   wire N17;
   wire N18;
   wire N19;
   wire N20;
   wire N21;
   wire N22;
   wire N23;
   wire N24;
   wire N25;
   wire N26;
   wire N27;
   wire N28;
   wire N29;
   wire N30;
   wire N31;
   wire N32;
   wire N33;
   wire N34;
   wire N35;
   wire N36;
   wire N37;
   wire n80;
   wire n90;
   wire n100;
   wire n110;
   wire n120;
   wire [15:0] counter;

   DFCN2Q out_clk_reg ( .Q (BAUDOUT_CLK),
	.D (N21),
	.CP (CLK),
	.CDN (n120) );
   AN2D1 U3 ( .Z (N9),
	.A2 (N26),
	.A1 (n80) );
   AN2D1 U4 ( .Z (N8),
	.A2 (N3),
	.A1 (N25) );
   AN2D1 U8 ( .Z (N7),
	.A2 (n80),
	.A1 (N24) );
   AN2D1 U9 ( .Z (N6),
	.A2 (N3),
	.A1 (N23) );
   AN2D1 U10 ( .Z (N5),
	.A2 (n80),
	.A1 (N22) );
   AN2D1 U11 ( .Z (N20),
	.A2 (N3),
	.A1 (N37) );
   AN2D1 U12 ( .Z (N19),
	.A2 (N3),
	.A1 (N36) );
   AN2D1 U13 ( .Z (N18),
	.A2 (N3),
	.A1 (N35) );
   AN2D1 U14 ( .Z (N17),
	.A2 (n80),
	.A1 (N34) );
   AN2D1 U15 ( .Z (N16),
	.A2 (n80),
	.A1 (N33) );
   AN2D1 U16 ( .Z (N15),
	.A2 (n80),
	.A1 (N32) );
   AN2D1 U17 ( .Z (N14),
	.A2 (n80),
	.A1 (N31) );
   AN2D1 U18 ( .Z (N13),
	.A2 (N3),
	.A1 (N30) );
   AN2D1 U19 ( .Z (N12),
	.A2 (N3),
	.A1 (N29) );
   AN2D1 U20 ( .Z (N11),
	.A2 (N3),
	.A1 (N28) );
   AN2D1 U21 ( .Z (N10),
	.A2 (N3),
	.A1 (N27) );
   BUF1 U22 ( .Z (n80),
	.I (N3) );
   BUF1 U23 ( .Z (n90),
	.I (RST_N) );
   BUF1 U24 ( .Z (n100),
	.I (RST_N) );
   BUF1 U25 ( .Z (n110),
	.I (RST_N) );
   BUF1 U26 ( .Z (n120),
	.I (RST_N) );
   DFCN1Q \counter_reg[1]  ( .Q (counter[1]),
	.D (N6),
	.CP (CLK),
	.CDN (n110) );
   DFCN1Q \counter_reg[2]  ( .Q (counter[2]),
	.D (N7),
	.CP (CLK),
	.CDN (n110) );
   DFCN1Q \counter_reg[0]  ( .Q (counter[0]),
	.D (N5),
	.CP (CLK),
	.CDN (n120) );
   DFCN1Q \counter_reg[4]  ( .Q (counter[4]),
	.D (N9),
	.CP (CLK),
	.CDN (n110) );
   DFCN1Q \counter_reg[3]  ( .Q (counter[3]),
	.D (N8),
	.CP (CLK),
	.CDN (n110) );
   DFCN1Q \counter_reg[5]  ( .Q (counter[5]),
	.D (N10),
	.CP (CLK),
	.CDN (n110) );
   DFCN1Q \counter_reg[9]  ( .Q (counter[9]),
	.D (N14),
	.CP (CLK),
	.CDN (n110) );
   DFCN1Q \counter_reg[6]  ( .Q (counter[6]),
	.D (N11),
	.CP (CLK),
	.CDN (n110) );
   DFCN1Q \counter_reg[8]  ( .Q (counter[8]),
	.D (N13),
	.CP (CLK),
	.CDN (n110) );
   DFCN1Q \counter_reg[7]  ( .Q (counter[7]),
	.D (N12),
	.CP (CLK),
	.CDN (n110) );
   DFCN1Q \counter_reg[13]  ( .Q (counter[13]),
	.D (N18),
	.CP (CLK),
	.CDN (n100) );
   DFCN1Q \counter_reg[10]  ( .Q (counter[10]),
	.D (N15),
	.CP (CLK),
	.CDN (n110) );
   DFCN1Q \counter_reg[14]  ( .Q (counter[14]),
	.D (N19),
	.CP (CLK),
	.CDN (n100) );
   DFCN1Q \counter_reg[12]  ( .Q (counter[12]),
	.D (N17),
	.CP (CLK),
	.CDN (n100) );
   DFCN1Q \counter_reg[15]  ( .Q (counter[15]),
	.D (N20),
	.CP (CLK),
	.CDN (n100) );
   DFCN1Q \counter_reg[11]  ( .Q (counter[11]),
	.D (N16),
	.CP (CLK),
	.CDN (n100) );
   DFCN1Q \divisor_copy_reg[9]  ( .Q (\divisor_copy[9] ),
	.D (DIVISOR[9]),
	.CP (CLK),
	.CDN (n90) );
   DFCN1Q \divisor_copy_reg[8]  ( .Q (\divisor_copy[8] ),
	.D (DIVISOR[8]),
	.CP (CLK),
	.CDN (n90) );
   DFCN1Q \divisor_copy_reg[5]  ( .Q (\divisor_copy[5] ),
	.D (DIVISOR[5]),
	.CP (CLK),
	.CDN (n100) );
   DFCN1Q \divisor_copy_reg[4]  ( .Q (\divisor_copy[4] ),
	.D (DIVISOR[4]),
	.CP (CLK),
	.CDN (n100) );
   DFCN1Q \divisor_copy_reg[10]  ( .Q (\divisor_copy[10] ),
	.D (DIVISOR[10]),
	.CP (CLK),
	.CDN (n90) );
   DFCN1Q \divisor_copy_reg[6]  ( .Q (\divisor_copy[6] ),
	.D (DIVISOR[6]),
	.CP (CLK),
	.CDN (n90) );
   DFCN1Q \divisor_copy_reg[11]  ( .Q (\divisor_copy[11] ),
	.D (DIVISOR[11]),
	.CP (CLK),
	.CDN (n90) );
   DFCN1Q \divisor_copy_reg[7]  ( .Q (\divisor_copy[7] ),
	.D (DIVISOR[7]),
	.CP (CLK),
	.CDN (n90) );
   DFCN1Q \divisor_copy_reg[3]  ( .Q (\divisor_copy[3] ),
	.D (DIVISOR[3]),
	.CP (CLK),
	.CDN (n100) );
   DFCN1Q \divisor_copy_reg[2]  ( .Q (\divisor_copy[2] ),
	.D (DIVISOR[2]),
	.CP (CLK),
	.CDN (n100) );
   DFCN1Q \divisor_copy_reg[1]  ( .Q (\divisor_copy[1] ),
	.D (DIVISOR[1]),
	.CP (CLK),
	.CDN (n100) );
   DFCN1Q \divisor_copy_reg[12]  ( .Q (\divisor_copy[12] ),
	.D (DIVISOR[12]),
	.CP (CLK),
	.CDN (n90) );
   DFCN1Q \divisor_copy_reg[13]  ( .Q (\divisor_copy[13] ),
	.D (DIVISOR[13]),
	.CP (CLK),
	.CDN (n90) );
   DFCN1Q \divisor_copy_reg[15]  ( .Q (\divisor_copy[15] ),
	.D (DIVISOR[15]),
	.CP (CLK),
	.CDN (n90) );
   DFCN1Q \divisor_copy_reg[14]  ( .Q (\divisor_copy[14] ),
	.D (DIVISOR[14]),
	.CP (CLK),
	.CDN (n90) );
   baud_rate_DW01_cmp2_16_1 lte_54 ( .A ({ counter[15],
		counter[14],
		counter[13],
		counter[12],
		counter[11],
		counter[10],
		counter[9],
		counter[8],
		counter[7],
		counter[6],
		counter[5],
		counter[4],
		counter[3],
		counter[2],
		counter[1],
		counter[0] }),
	.B ({ 1'b0,
		\divisor_copy[15] ,
		\divisor_copy[14] ,
		\divisor_copy[13] ,
		\divisor_copy[12] ,
		\divisor_copy[11] ,
		\divisor_copy[10] ,
		\divisor_copy[9] ,
		\divisor_copy[8] ,
		\divisor_copy[7] ,
		\divisor_copy[6] ,
		\divisor_copy[5] ,
		\divisor_copy[4] ,
		\divisor_copy[3] ,
		\divisor_copy[2] ,
		\divisor_copy[1]  }),
	.LT_LE (N21) );
   baud_rate_DW01_inc_16_0 add_42 ( .A ({ counter[15],
		counter[14],
		counter[13],
		counter[12],
		counter[11],
		counter[10],
		counter[9],
		counter[8],
		counter[7],
		counter[6],
		counter[5],
		counter[4],
		counter[3],
		counter[2],
		counter[1],
		counter[0] }),
	.SUM ({ N37,
		N36,
		N35,
		N34,
		N33,
		N32,
		N31,
		N30,
		N29,
		N28,
		N27,
		N26,
		N25,
		N24,
		N23,
		N22 }) );
   baud_rate_DW01_cmp2_16_0 lt_41 ( .A ({ counter[15],
		counter[14],
		counter[13],
		counter[12],
		counter[11],
		counter[10],
		counter[9],
		counter[8],
		counter[7],
		counter[6],
		counter[5],
		counter[4],
		counter[3],
		counter[2],
		counter[1],
		counter[0] }),
	.B ({ DIVISOR[15],
		DIVISOR[14],
		DIVISOR[13],
		DIVISOR[12],
		DIVISOR[11],
		DIVISOR[10],
		DIVISOR[9],
		DIVISOR[8],
		DIVISOR[7],
		DIVISOR[6],
		DIVISOR[5],
		DIVISOR[4],
		DIVISOR[3],
		DIVISOR[2],
		DIVISOR[1],
		DIVISOR[0] }),
	.LT_LE (N3) );
endmodule

module txblock (
	clk16,
	paral_data,
	serial_data,
	tx_en,
	tx_end,
	rst_n,
	clk_tx );
   input clk16;
   input [7:0] paral_data;
   output serial_data;
   input tx_en;
   output tx_end;
   input rst_n;
   output clk_tx;

   // Internal wires
   wire \clk_counter[2] ;
   wire \clk_counter[1] ;
   wire \clk_counter[0] ;
   wire \para_data_copy1[8] ;
   wire N4;
   wire \para_data_copy[9] ;
   wire te_flag;
   wire N5;
   wire rst_flag;
   wire N10;
   wire N11;
   wire N12;
   wire N13;
   wire N16;
   wire N17;
   wire N18;
   wire n40;
   wire n7;
   wire n9;
   wire n15;
   wire n160;
   wire n170;
   wire n180;
   wire n20;
   wire n21;
   wire n22;
   wire n23;
   wire n24;
   wire n25;
   wire n26;
   wire n28;
   wire n29;
   wire n30;
   wire n31;
   wire n32;
   wire n33;
   wire n34;
   wire n35;
   wire n36;
   wire [3:0] tx_counter;

   INV2 U18 ( .ZN (tx_end),
	.I (n25) );
   AN4D1 U23 ( .Z (n9),
	.A4 (n24),
	.A3 (n23),
	.A2 (n22),
	.A1 (n21) );
   DFCN1 rst_flag_reg ( .QN (n31),
	.Q (rst_flag),
	.D (n26),
	.CP (tx_en),
	.CDN (n28) );
   DFCN2Q \clk_counter_reg[3]  ( .Q (clk_tx),
	.D (N13),
	.CP (clk16),
	.CDN (n28) );
   ND2D0 U30 ( .ZN (n170),
	.A2 (n40),
	.A1 (N17) );
   NR2D0 U31 ( .ZN (n36),
	.A2 (n34),
	.A1 (n35) );
   BUF1 U32 ( .Z (n28),
	.I (rst_n) );
   BUF1 U33 ( .Z (n29),
	.I (clk_tx) );
   IND2D2 U34 ( .ZN (serial_data),
	.B1 (tx_end),
	.A1 (\para_data_copy[9] ) );
   DFCN1Q \para_data_copy1_reg[8]  ( .Q (\para_data_copy1[8] ),
	.D (paral_data[7]),
	.CP (tx_en),
	.CDN (n28) );
   XOR2D1 U35 ( .Z (N17),
	.A2 (n23),
	.A1 (n32) );
   XOR2D1 U36 ( .Z (N12),
	.A2 (n34),
	.A1 (n35) );
   ND2D0 U37 ( .ZN (n32),
	.A2 (tx_counter[0]),
	.A1 (tx_counter[1]) );
   IND2D1 U38 ( .ZN (n15),
	.B1 (n40),
	.A1 (tx_counter[0]) );
   ND2D0 U39 ( .ZN (n160),
	.A2 (n40),
	.A1 (N16) );
   ND2D0 U40 ( .ZN (n180),
	.A2 (n40),
	.A1 (N18) );
   NR2D0 U41 ( .ZN (n33),
	.A2 (n23),
	.A1 (n32) );
   INV0 U42 ( .ZN (n40),
	.I (n20) );
   OAI21D0 U43 ( .ZN (N5),
	.B (tx_counter[3]),
	.A2 (tx_counter[1]),
	.A1 (tx_counter[2]) );
   ND2D0 U44 ( .ZN (n35),
	.A2 (\clk_counter[0] ),
	.A1 (\clk_counter[1] ) );
   XOR2D1 U45 ( .Z (N13),
	.A2 (clk_tx),
	.A1 (n36) );
   INV0 U46 ( .ZN (n34),
	.I (\clk_counter[2] ) );
   INV0 U47 ( .ZN (N10),
	.I (\clk_counter[0] ) );
   ND2D0 U48 ( .ZN (n20),
	.A2 (rst_flag),
	.A1 (te_flag) );
   NR2D0 U49 ( .ZN (n30),
	.A2 (te_flag),
	.A1 (n31) );
   NR2D0 U50 ( .ZN (N4),
	.A2 (n7),
	.A1 (n9) );
   INV0 U51 ( .ZN (tx_counter[1]),
	.I (n22) );
   INV0 U52 ( .ZN (tx_counter[2]),
	.I (n23) );
   INV0 U53 ( .ZN (tx_counter[3]),
	.I (n24) );
   INV0 U54 ( .ZN (tx_counter[0]),
	.I (n21) );
   ND2D0 U55 ( .ZN (n26),
	.A2 (n31),
	.A1 (n7) );
   INV0 U56 ( .ZN (n7),
	.I (tx_en) );
   DFCN1Q \clk_counter_reg[0]  ( .Q (\clk_counter[0] ),
	.D (N10),
	.CP (clk16),
	.CDN (rst_n) );
   DFCN1Q \clk_counter_reg[1]  ( .Q (\clk_counter[1] ),
	.D (N11),
	.CP (clk16),
	.CDN (rst_n) );
   DFCN1Q \clk_counter_reg[2]  ( .Q (\clk_counter[2] ),
	.D (N12),
	.CP (clk16),
	.CDN (rst_n) );
   DFF1Q te_flag_reg ( .Q (te_flag),
	.D (N5),
	.CP (tx_en) );
   DFCSN1N \tx_counter_reg[3]  ( .SDN (n180),
	.QN (n24),
	.D (1'b1),
	.CP (clk_tx),
	.CDN (n30) );
   DFCSN1N \tx_counter_reg[0]  ( .SDN (n15),
	.QN (n21),
	.D (1'b1),
	.CP (n29),
	.CDN (n30) );
   DFCSN1N \tx_counter_reg[2]  ( .SDN (n170),
	.QN (n23),
	.D (1'b1),
	.CP (clk_tx),
	.CDN (n30) );
   DFCSN1N \tx_counter_reg[1]  ( .SDN (n160),
	.QN (n22),
	.D (1'b1),
	.CP (n29),
	.CDN (n30) );
   DFCSN1N tx_end_flag_reg ( .SDN (n20),
	.QN (n25),
	.D (1'b0),
	.CP (n29),
	.CDN (rst_flag) );
   DFCNS1Q \para_data_copy_reg[9]  ( .S (N4),
	.Q (\para_data_copy[9] ),
	.D (\para_data_copy1[8] ),
	.CP (n29),
	.CDN (1'b1) );
   XOR2D1 U57 ( .Z (N16),
	.A2 (tx_counter[0]),
	.A1 (tx_counter[1]) );
   XNR2D1 U58 ( .ZN (N18),
	.A2 (n24),
	.A1 (n33) );
   XOR2D1 U59 ( .Z (N11),
	.A2 (\clk_counter[0] ),
	.A1 (\clk_counter[1] ) );
endmodule

module rxblock_DW01_inc_8_0 (
	A,
	SUM );
   input [7:0] A;
   output [7:0] SUM;

   // Internal wires
   wire \carry[7] ;
   wire \carry[6] ;
   wire \carry[5] ;
   wire \carry[4] ;
   wire \carry[3] ;
   wire \carry[2] ;

   HA1D1 U1_1_2 ( .S (SUM[2]),
	.CO (\carry[3] ),
	.B (\carry[2] ),
	.A (A[2]) );
   HA1D1 U1_1_3 ( .S (SUM[3]),
	.CO (\carry[4] ),
	.B (\carry[3] ),
	.A (A[3]) );
   HA1D1 U1_1_4 ( .S (SUM[4]),
	.CO (\carry[5] ),
	.B (\carry[4] ),
	.A (A[4]) );
   HA1D1 U1_1_5 ( .S (SUM[5]),
	.CO (\carry[6] ),
	.B (\carry[5] ),
	.A (A[5]) );
   HA1D1 U1_1_1 ( .S (SUM[1]),
	.CO (\carry[2] ),
	.B (A[0]),
	.A (A[1]) );
   HA1D1 U1_1_6 ( .S (SUM[6]),
	.CO (\carry[7] ),
	.B (\carry[6] ),
	.A (A[6]) );
   INV0 U5 ( .ZN (SUM[0]),
	.I (A[0]) );
   XOR2D1 U6 ( .Z (SUM[7]),
	.A2 (A[7]),
	.A1 (\carry[7] ) );
endmodule

module rxblock (
	clk16,
	serial_data_in,
	rst_n,
	para_data_out,
	receive_flag );
   input clk16;
   input serial_data_in;
   input rst_n;
   output [7:0] para_data_out;
   output receive_flag;

   // Internal wires
   wire N10;
   wire \output_regs[8] ;
   wire \output_regs[7] ;
   wire \output_regs[6] ;
   wire \output_regs[5] ;
   wire \output_regs[4] ;
   wire \output_regs[3] ;
   wire \output_regs[2] ;
   wire \output_regs[1] ;
   wire N51;
   wire N55;
   wire N59;
   wire N63;
   wire N67;
   wire N71;
   wire N75;
   wire N79;
   wire N88;
   wire N89;
   wire N90;
   wire N91;
   wire N92;
   wire N93;
   wire N94;
   wire N95;
   wire n9;
   wire n100;
   wire n13;
   wire n14;
   wire n15;
   wire n16;
   wire n17;
   wire n18;
   wire n20;
   wire n22;
   wire n24;
   wire n25;
   wire n26;
   wire n27;
   wire n28;
   wire n29;
   wire n30;
   wire n31;
   wire n32;
   wire n33;
   wire n34;
   wire n35;
   wire n36;
   wire n37;
   wire n38;
   wire n39;
   wire n40;
   wire n41;
   wire n42;
   wire n43;
   wire n44;
   wire n45;
   wire n46;
   wire n47;
   wire n48;
   wire n49;
   wire n50;
   wire n510;
   wire [7:0] clk_sample_counter;

   AN3D1 U15 ( .Z (N79),
	.A3 (n44),
	.A2 (n43),
	.A1 (n29) );
   IND3D1 U18 ( .ZN (n13),
	.B2 (n44),
	.B1 (n28),
	.A1 (clk_sample_counter[6]) );
   AN3D1 U19 ( .Z (N67),
	.A3 (n16),
	.A2 (n15),
	.A1 (n14) );
   AN3D1 U20 ( .Z (N63),
	.A3 (n16),
	.A2 (n15),
	.A1 (clk_sample_counter[4]) );
   AN3D1 U21 ( .Z (N59),
	.A3 (n16),
	.A2 (n14),
	.A1 (clk_sample_counter[5]) );
   AN3D1 U22 ( .Z (N55),
	.A3 (n16),
	.A2 (n29),
	.A1 (clk_sample_counter[5]) );
   IND2D1 U24 ( .ZN (n17),
	.B1 (n44),
	.A1 (n18) );
   AN4D1 U28 ( .Z (N51),
	.A4 (n14),
	.A3 (n43),
	.A2 (n26),
	.A1 (n22) );
   INV0 U40 ( .ZN (n25),
	.I (n24) );
   BUF1 U41 ( .Z (n26),
	.I (clk_sample_counter[7]) );
   BUF1 U42 ( .Z (n27),
	.I (n38) );
   INV0 U43 ( .ZN (n28),
	.I (n15) );
   INV0 U44 ( .ZN (n29),
	.I (n46) );
   AN7D1 U45 ( .Z (n30),
	.A7 (clk_sample_counter[4]),
	.A6 (clk_sample_counter[3]),
	.A5 (clk_sample_counter[7]),
	.A4 (n43),
	.A3 (n25),
	.A2 (n100),
	.A1 (n9) );
   INV0 U46 ( .ZN (n31),
	.I (n30) );
   INV0 U47 ( .ZN (n32),
	.I (n30) );
   INV0 U48 ( .ZN (n33),
	.I (serial_data_in) );
   INV0 U49 ( .ZN (n34),
	.I (n33) );
   INV0 U50 ( .ZN (n35),
	.I (n33) );
   INV0 U51 ( .ZN (n36),
	.I (rst_n) );
   INV0 U52 ( .ZN (n37),
	.I (n36) );
   INV0 U53 ( .ZN (n38),
	.I (n36) );
   INV0 U54 ( .ZN (n39),
	.I (n36) );
   INR2D2 U55 ( .ZN (para_data_out[7]),
	.B1 (n31),
	.A1 (\output_regs[8] ) );
   INR2D2 U56 ( .ZN (para_data_out[6]),
	.B1 (n32),
	.A1 (\output_regs[7] ) );
   INR2D2 U57 ( .ZN (para_data_out[5]),
	.B1 (n31),
	.A1 (\output_regs[6] ) );
   INR2D2 U58 ( .ZN (para_data_out[4]),
	.B1 (n32),
	.A1 (\output_regs[5] ) );
   INR2D2 U59 ( .ZN (para_data_out[3]),
	.B1 (n31),
	.A1 (\output_regs[4] ) );
   INR2D2 U60 ( .ZN (para_data_out[2]),
	.B1 (n31),
	.A1 (\output_regs[3] ) );
   ND2D0 U61 ( .ZN (n40),
	.A2 (N10),
	.A1 (receive_flag) );
   INV0 U62 ( .ZN (n41),
	.I (n40) );
   INV0 U63 ( .ZN (n42),
	.I (n40) );
   INR2D2 U64 ( .ZN (para_data_out[1]),
	.B1 (n32),
	.A1 (\output_regs[2] ) );
   DFCN2 re_flag_reg ( .Q (receive_flag),
	.D (N10),
	.CP (n33),
	.CDN (n37) );
   INR2D2 U65 ( .ZN (para_data_out[0]),
	.B1 (n32),
	.A1 (\output_regs[1] ) );
   AN2D1 U66 ( .Z (n43),
	.A2 (n15),
	.A1 (n18) );
   INV0 U67 ( .ZN (n16),
	.I (n17) );
   NR2D0 U68 ( .ZN (N71),
	.A2 (n13),
	.A1 (n14) );
   INV0 U69 ( .ZN (n15),
	.I (clk_sample_counter[5]) );
   INV0 U70 ( .ZN (n9),
	.I (clk_sample_counter[1]) );
   INV0 U71 ( .ZN (n100),
	.I (clk_sample_counter[2]) );
   INV0 U72 ( .ZN (n18),
	.I (clk_sample_counter[6]) );
   ND4D0 U73 ( .ZN (n20),
	.A4 (n100),
	.A3 (n9),
	.A2 (n24),
	.A1 (clk_sample_counter[3]) );
   INV0 U74 ( .ZN (n24),
	.I (clk_sample_counter[0]) );
   NR3D0 U75 ( .ZN (n47),
	.A3 (clk_sample_counter[1]),
	.A2 (clk_sample_counter[2]),
	.A1 (clk_sample_counter[0]) );
   NR2D0 U76 ( .ZN (n44),
	.A2 (n26),
	.A1 (n20) );
   OAI21D0 U77 ( .ZN (N10),
	.B (clk_sample_counter[7]),
	.A2 (n49),
	.A1 (n50) );
   ND2D0 U78 ( .ZN (n49),
	.A2 (n18),
	.A1 (n48) );
   NR3D0 U79 ( .ZN (n50),
	.A3 (n45),
	.A2 (n46),
	.A1 (n47) );
   NR2D0 U80 ( .ZN (N75),
	.A2 (n13),
	.A1 (n29) );
   INV0 U81 ( .ZN (n14),
	.I (n29) );
   INV0 U82 ( .ZN (n22),
	.I (n20) );
   INV0 U83 ( .ZN (n46),
	.I (clk_sample_counter[4]) );
   INV0 U84 ( .ZN (n45),
	.I (clk_sample_counter[3]) );
   INV0 U85 ( .ZN (n48),
	.I (n28) );
   DFCNS1Q \clk_sample_counter_reg[5]  ( .S (n42),
	.Q (clk_sample_counter[5]),
	.D (N93),
	.CP (n510),
	.CDN (rst_n) );
   DFCNS1Q \clk_sample_counter_reg[4]  ( .S (n41),
	.Q (clk_sample_counter[4]),
	.D (N92),
	.CP (n510),
	.CDN (rst_n) );
   DFCNS1Q \clk_sample_counter_reg[0]  ( .S (n42),
	.Q (clk_sample_counter[0]),
	.D (N88),
	.CP (clk16),
	.CDN (n37) );
   DFCNS1Q \clk_sample_counter_reg[7]  ( .S (n41),
	.Q (clk_sample_counter[7]),
	.D (N95),
	.CP (clk16),
	.CDN (n39) );
   DFCNS1Q \clk_sample_counter_reg[6]  ( .S (n41),
	.Q (clk_sample_counter[6]),
	.D (N94),
	.CP (clk16),
	.CDN (n39) );
   DFCNS1Q \clk_sample_counter_reg[3]  ( .S (n42),
	.Q (clk_sample_counter[3]),
	.D (N91),
	.CP (clk16),
	.CDN (n37) );
   DFCNS1Q \clk_sample_counter_reg[1]  ( .S (n42),
	.Q (clk_sample_counter[1]),
	.D (N89),
	.CP (n510),
	.CDN (n27) );
   DFCNS1Q \clk_sample_counter_reg[2]  ( .S (n41),
	.Q (clk_sample_counter[2]),
	.D (N90),
	.CP (n510),
	.CDN (n39) );
   DFCNS1Q \output_regs_reg[1]  ( .S (N79),
	.Q (\output_regs[1] ),
	.D (n34),
	.CP (n510),
	.CDN (n37) );
   DFCNS1Q \output_regs_reg[2]  ( .S (N75),
	.Q (\output_regs[2] ),
	.D (n35),
	.CP (n510),
	.CDN (n38) );
   DFCNS1Q \output_regs_reg[3]  ( .S (N71),
	.Q (\output_regs[3] ),
	.D (serial_data_in),
	.CP (n510),
	.CDN (n27) );
   DFCNS1Q \output_regs_reg[4]  ( .S (N67),
	.Q (\output_regs[4] ),
	.D (n35),
	.CP (clk16),
	.CDN (n38) );
   DFCNS1Q \output_regs_reg[5]  ( .S (N63),
	.Q (\output_regs[5] ),
	.D (n34),
	.CP (clk16),
	.CDN (n39) );
   DFCNS1Q \output_regs_reg[6]  ( .S (N59),
	.Q (\output_regs[6] ),
	.D (n35),
	.CP (clk16),
	.CDN (n38) );
   DFCNS1Q \output_regs_reg[7]  ( .S (N55),
	.Q (\output_regs[7] ),
	.D (serial_data_in),
	.CP (clk16),
	.CDN (rst_n) );
   DFCNS1Q \output_regs_reg[8]  ( .S (N51),
	.Q (\output_regs[8] ),
	.D (n34),
	.CP (clk16),
	.CDN (n27) );
   rxblock_DW01_inc_8_0 add_59 ( .A ({ clk_sample_counter[7],
		clk_sample_counter[6],
		clk_sample_counter[5],
		clk_sample_counter[4],
		clk_sample_counter[3],
		clk_sample_counter[2],
		clk_sample_counter[1],
		clk_sample_counter[0] }),
	.SUM ({ N95,
		N94,
		N93,
		N92,
		N91,
		N90,
		N89,
		N88 }) );
   BUF1 U86 ( .Z (n510),
	.I (clk16) );
endmodule

module uart (
	clk,
	divisor,
	rst_n,
	para_data_in,
	serial_out,
	tx_en,
	tx_flag,
	serial_in,
	para_data_out,
	rx_flag,
	clk_tx );
   input clk;
   input [15:0] divisor;
   input rst_n;
   input [7:0] para_data_in;
   output serial_out;
   input tx_en;
   output tx_flag;
   input serial_in;
   output [7:0] para_data_out;
   output rx_flag;
   output clk_tx;

   // Internal wires
   wire baudout_clk;
   wire n1;
   wire n2;

   baud_rate u1 ( .CLK (clk),
	.DIVISOR ({ divisor[15],
		divisor[14],
		divisor[13],
		divisor[12],
		divisor[11],
		divisor[10],
		divisor[9],
		divisor[8],
		divisor[7],
		divisor[6],
		divisor[5],
		divisor[4],
		divisor[3],
		divisor[2],
		divisor[1],
		divisor[0] }),
	.RST_N (n2),
	.BAUDOUT_CLK (baudout_clk) );
   txblock u2 ( .clk16 (baudout_clk),
	.paral_data ({ para_data_in[7],
		para_data_in[6],
		para_data_in[5],
		para_data_in[4],
		para_data_in[3],
		para_data_in[2],
		para_data_in[1],
		para_data_in[0] }),
	.serial_data (serial_out),
	.tx_en (tx_en),
	.tx_end (tx_flag),
	.rst_n (rst_n),
	.clk_tx (clk_tx) );
   rxblock u3 ( .clk16 (n1),
	.serial_data_in (serial_in),
	.rst_n (rst_n),
	.para_data_out ({ para_data_out[7],
		para_data_out[6],
		para_data_out[5],
		para_data_out[4],
		para_data_out[3],
		para_data_out[2],
		para_data_out[1],
		para_data_out[0] }),
	.receive_flag (rx_flag) );
   BUF1 U10 ( .Z (n1),
	.I (baudout_clk) );
   BUF1 U20 ( .Z (n2),
	.I (rst_n) );
endmodule

