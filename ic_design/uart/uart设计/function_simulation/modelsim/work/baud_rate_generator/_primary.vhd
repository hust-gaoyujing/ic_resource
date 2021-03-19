library verilog;
use verilog.vl_types.all;
entity baud_rate_generator is
    port(
        CLK             : in     vl_logic;
        DIVISOR         : in     vl_logic_vector(15 downto 0);
        RST_N           : in     vl_logic;
        BAUDOUT_CLK     : out    vl_logic
    );
end baud_rate_generator;
