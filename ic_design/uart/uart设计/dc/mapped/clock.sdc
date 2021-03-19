#####################################################

#  Created by Design Compiler write_sdc on Fri Dec  5 06:07:49 2008

#####################################################
set sdc_version 1.4

create_clock -name "clock" -period 50 -waveform {0 25} [get_ports {CLK}]
set_input_delay 20 -clock "clock" [get_ports {RST_N}]
set_input_delay 20 -clock "clock" [get_ports {DIVISOR[0]}]
set_input_delay 20 -clock "clock" [get_ports {DIVISOR[1]}]
set_input_delay 20 -clock "clock" [get_ports {DIVISOR[2]}]
set_input_delay 20 -clock "clock" [get_ports {DIVISOR[3]}]
set_input_delay 20 -clock "clock" [get_ports {DIVISOR[4]}]
set_input_delay 20 -clock "clock" [get_ports {DIVISOR[5]}]
set_input_delay 20 -clock "clock" [get_ports {DIVISOR[6]}]
set_input_delay 20 -clock "clock" [get_ports {DIVISOR[7]}]
set_input_delay 20 -clock "clock" [get_ports {DIVISOR[8]}]
set_input_delay 20 -clock "clock" [get_ports {DIVISOR[9]}]
set_input_delay 20 -clock "clock" [get_ports {DIVISOR[10]}]
set_input_delay 20 -clock "clock" [get_ports {DIVISOR[11]}]
set_input_delay 20 -clock "clock" [get_ports {DIVISOR[12]}]
set_input_delay 20 -clock "clock" [get_ports {DIVISOR[13]}]
set_input_delay 20 -clock "clock" [get_ports {DIVISOR[14]}]
set_input_delay 20 -clock "clock" [get_ports {DIVISOR[15]}]
set_input_delay 20 -clock "clock" [get_ports {CLK}]
set_output_delay 20 -clock "clock" [get_ports {BAUDOUT_CLK}]
set_clock_transition -rise 2 [get_clocks {clock}]
set_clock_transition -fall 2 [get_clocks {clock}]
set_clock_uncertainty  2 -setup [get_clocks {clock}]
set_clock_uncertainty  0.3 -hold [get_clocks {clock}]
set_max_fanout 10 [current_design]
set_operating_conditions "BCCOM" -library "tcb773pbc"
set_wire_load_model  -name "TSMC16K_Conservative" -library "tcb773pbc"
set_wire_load_mode "top" 
set_load -pin_load 2 [get_ports {BAUDOUT_CLK}]
set_load -min -pin_load 2 [get_ports {BAUDOUT_CLK}]
