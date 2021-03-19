#####################################################

#  Created by Design Compiler write_sdc on Fri Dec  5 06:08:48 2008

#####################################################
set sdc_version 1.4

create_clock -name "clock" -period 50000 -waveform {0 25000} [get_ports {clk16}]
set_input_delay 40 -clock "clock" [get_ports {rst_n}]
set_input_delay 40 -clock "clock" [get_ports {tx_en}]
set_input_delay 40 -clock "clock" [get_ports {paral_data[0]}]
set_input_delay 40 -clock "clock" [get_ports {paral_data[1]}]
set_input_delay 40 -clock "clock" [get_ports {paral_data[2]}]
set_input_delay 40 -clock "clock" [get_ports {paral_data[3]}]
set_input_delay 40 -clock "clock" [get_ports {paral_data[4]}]
set_input_delay 40 -clock "clock" [get_ports {paral_data[5]}]
set_input_delay 40 -clock "clock" [get_ports {paral_data[6]}]
set_input_delay 40 -clock "clock" [get_ports {paral_data[7]}]
set_input_delay 40 -clock "clock" [get_ports {clk16}]
set_output_delay 40 -clock "clock" [get_ports {clk_tx}]
set_output_delay 40 -clock "clock" [get_ports {tx_end}]
set_output_delay 40 -clock "clock" [get_ports {serial_data}]
set_clock_transition -rise 2 [get_clocks {clock}]
set_clock_transition -fall 2 [get_clocks {clock}]
set_clock_uncertainty  2 -setup [get_clocks {clock}]
set_clock_uncertainty  0.3 -hold [get_clocks {clock}]
set_max_fanout 4 [current_design]
set_operating_conditions "BCCOM" -library "tcb773pbc"
set_wire_load_model  -name "TSMC16K_Conservative" -library "tcb773pbc"
set_wire_load_mode "top" 
set_load -pin_load 2 [get_ports {serial_data}]
set_load -min -pin_load 2 [get_ports {serial_data}]
set_load -pin_load 2 [get_ports {tx_end}]
set_load -min -pin_load 2 [get_ports {tx_end}]
set_load -pin_load 2 [get_ports {clk_tx}]
set_load -min -pin_load 2 [get_ports {clk_tx}]
