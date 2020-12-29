#####################################################

#  Created by Design Compiler write_sdc on Fri Dec  5 06:08:14 2008

#####################################################
set sdc_version 1.4

create_clock -name "clock" -period 50000 -waveform {0 25000} [get_ports {clk16}]
set_input_delay 40 -clock "clock" [get_ports {rst_n}]
set_input_delay 40 -clock "clock" [get_ports {serial_data_in}]
set_input_delay 40 -clock "clock" [get_ports {clk16}]
set_output_delay 40 -clock "clock" [get_ports {receive_flag}]
set_output_delay 40 -clock "clock" [get_ports {para_data_out[0]}]
set_output_delay 40 -clock "clock" [get_ports {para_data_out[1]}]
set_output_delay 40 -clock "clock" [get_ports {para_data_out[2]}]
set_output_delay 40 -clock "clock" [get_ports {para_data_out[3]}]
set_output_delay 40 -clock "clock" [get_ports {para_data_out[4]}]
set_output_delay 40 -clock "clock" [get_ports {para_data_out[5]}]
set_output_delay 40 -clock "clock" [get_ports {para_data_out[6]}]
set_output_delay 40 -clock "clock" [get_ports {para_data_out[7]}]
set_clock_transition -rise 2 [get_clocks {clock}]
set_clock_transition -fall 2 [get_clocks {clock}]
set_clock_uncertainty  2 -setup [get_clocks {clock}]
set_clock_uncertainty  0.3 -hold [get_clocks {clock}]
set_max_fanout 4 [current_design]
set_operating_conditions "BCCOM" -library "tcb773pbc"
set_wire_load_model  -name "TSMC16K_Conservative" -library "tcb773pbc"
set_wire_load_mode "top" 
set_load -pin_load 2 [get_ports {para_data_out[7]}]
set_load -min -pin_load 2 [get_ports {para_data_out[7]}]
set_load -pin_load 2 [get_ports {para_data_out[6]}]
set_load -min -pin_load 2 [get_ports {para_data_out[6]}]
set_load -pin_load 2 [get_ports {para_data_out[5]}]
set_load -min -pin_load 2 [get_ports {para_data_out[5]}]
set_load -pin_load 2 [get_ports {para_data_out[4]}]
set_load -min -pin_load 2 [get_ports {para_data_out[4]}]
set_load -pin_load 2 [get_ports {para_data_out[3]}]
set_load -min -pin_load 2 [get_ports {para_data_out[3]}]
set_load -pin_load 2 [get_ports {para_data_out[2]}]
set_load -min -pin_load 2 [get_ports {para_data_out[2]}]
set_load -pin_load 2 [get_ports {para_data_out[1]}]
set_load -min -pin_load 2 [get_ports {para_data_out[1]}]
set_load -pin_load 2 [get_ports {para_data_out[0]}]
set_load -min -pin_load 2 [get_ports {para_data_out[0]}]
set_load -pin_load 2 [get_ports {receive_flag}]
set_load -min -pin_load 2 [get_ports {receive_flag}]
