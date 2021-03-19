--FullAdder.vhd
Library IEEE;
use IEEE.std_logic_1164.all;

Entity FullAdder is
port(x,y,Cin:in std_logic;
     S,Cout:out std_logic
     );
End Entity FullAdder;

Architecture rtl of FullAdder is
Begin
S<=x xor y xor Cin;
Cout<=(x and y) or(x and Cin) or (y and Cin);
End Architecture rtl;

