--HalfAdder.vhd
Library IEEE;
use IEEE.std_logic_1164.all;

Entity HalfAdder is
port(x,y:in std_logic;
     HS,CO:out std_logic
     );
End Entity HalfAdder;

Architecture rtl of HalfAdder is
Begin
HS<=x xor y;
CO<=x and y;
End Architecture rtl;

