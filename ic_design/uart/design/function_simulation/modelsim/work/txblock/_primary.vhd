library verilog;
use verilog.vl_types.all;
entity txblock is
    port(
        clk16           : in     vl_logic;
        paral_data      : in     vl_logic_vector(7 downto 0);
        serial_data     : out    vl_logic;
        tx_en           : in     vl_logic;
        tx_end          : out    vl_logic;
        rst_n           : in     vl_logic;
        clk_tx          : out    vl_logic
    );
end txblock;
