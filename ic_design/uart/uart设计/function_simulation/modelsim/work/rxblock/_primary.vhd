library verilog;
use verilog.vl_types.all;
entity rxblock is
    port(
        clk16           : in     vl_logic;
        serial_data_in  : in     vl_logic;
        rst_n           : in     vl_logic;
        para_data_out   : out    vl_logic_vector(7 downto 0);
        receive_flag    : out    vl_logic
    );
end rxblock;
