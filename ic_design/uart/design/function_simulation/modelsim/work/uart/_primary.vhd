library verilog;
use verilog.vl_types.all;
entity uart is
    port(
        clk             : in     vl_logic;
        divisor         : in     vl_logic_vector(15 downto 0);
        rst_n           : in     vl_logic;
        para_data_in    : in     vl_logic_vector(7 downto 0);
        serial_out      : out    vl_logic;
        tx_en           : in     vl_logic;
        tx_flag         : out    vl_logic;
        serial_in       : in     vl_logic;
        para_data_out   : out    vl_logic_vector(7 downto 0);
        rx_flag         : out    vl_logic;
        clk_tx          : out    vl_logic
    );
end uart;
