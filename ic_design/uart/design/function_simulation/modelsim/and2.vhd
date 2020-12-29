--And2.vhd
Library IEEE;
use IEEE.std_logic_1164.all;

Entity And2 is
port( x,y:in std_logic;
      output: out std_logic
    );
End Entity And2;

Architecture rtl of And2 is
Begin
output<=x and y;
End Architecture rtl;
