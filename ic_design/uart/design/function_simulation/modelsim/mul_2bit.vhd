--Mul_2bit.vhd
Library IEEE;
use IEEE.std_logic_1164.all;

Entity mul_2bit is
port(
 A,B:in std_logic_vector(1 downto 0);
 S0,S1,S2,S3:out std_logic
);
End Entity mul_2bit;

Architecture structural of mul_2bit is

Component And2 is
port(x,y: in std_logic;
     output: out std_logic
     );
End Component And2;

Component HalfAdder is
port(x,y:in std_logic;
     HS,CO:out std_logic
     );
End Component HalfAdder;

Signal internalAndOut:std_logic_vector(3 downto 0);
Signal internalCO: std_logic;

Begin
myAnd2_1:component And2 
  port map(x=>A(0),y=>B(0),output=>internalAndOut(0));
myAnd2_2:component And2
  port map(x=>A(1),y=>B(0),output=>internalAndOut(1));
myAnd2_3:component And2
  port map(x=>A(0),y=>B(1),output=>internalAndOut(2));
myAnd2_4:component And2
  port map(x=>A(1),y=>B(1),output=>internalAndOut(3));

S0<=internalAndOut(0);
myHalfAdder_1: component HalfAdder
  port map(x=>internalAndOut(1),y=>internalAndOut(2),
           HS=>S1,CO=>internalCO);
myHalfAdder_2:component HalfAdder
  port map(x=>internalCO,y=>internalAndOut(3),
           HS=>S2,CO=>S3);
End Architecture structural;
      


