#####################################################

#  Created by Design Compiler write_sdc on Fri Dec  5 11:09:40 2008

#####################################################
set sdc_version 1.4

create_clock -name "clock" -period 50000 -waveform {0 25000} [get_ports {clk}]
set_input_delay 40 -clock "clock" [get_ports {duty[0]}]
set_input_delay 40 -clock "clock" [get_ports {duty[1]}]
set_input_delay 40 -clock "clock" [get_ports {duty[2]}]
set_input_delay 40 -clock "clock" [get_ports {duty[3]}]
set_input_delay 40 -clock "clock" [get_ports {clk_in}]
set_input_delay 40 -clock "clock" [get_ports {clk}]
set_input_delay 40 -clock "clock" [get_ports {limit[0]}]
set_input_delay 40 -clock "clock" [get_ports {limit[1]}]
set_input_delay 40 -clock "clock" [get_ports {limit[2]}]
set_input_delay 40 -clock "clock" [get_ports {limit[3]}]
set_input_delay 40 -clock "clock" [get_ports {reset}]
set_output_delay 40 -clock "clock" [get_ports {clk_out}]
set_clock_transition -rise 2 [get_clocks {clock}]
set_clock_transition -fall 2 [get_clocks {clock}]
set_clock_uncertainty  2 -setup [get_clocks {clock}]
set_clock_uncertainty  0.3 -hold [get_clocks {clock}]
set_max_fanout 4 [current_design]
set_operating_conditions "BCCOM" -library "tcb773pbc"
set_wire_load_model  -name "TSMC16K_Conservative" -library "tcb773pbc"
set_wire_load_mode "top" 
set_load -pin_load 2 [get_ports {clk_out}]
set_load -min -pin_load 2 [get_ports {clk_out}]
