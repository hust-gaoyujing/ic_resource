# specify the libraries
set search_path      [list /export/home/forest/cyh/roic_tsmc35/lib /export/home/forest/xs]
set target_library   [list tcb773pbc.db]

set link_library     [list {*} tcb773pbc.db]

set symbol_library   [list tcb773s.sdb]
#
# Directory Structure
set MAIN_DIR /export/home/forest/xs
set UNMAPPED_DIR $MAIN_DIR/unmapped
set SCRIPT_DIR $MAIN_DIR/scripts
set MAPPED_DIR $MAIN_DIR/mapped
set REPORTS_DIR $MAIN_DIR/reports

# read the design
source $SCRIPT_DIR/Baud_rate_generator.tcl
source $SCRIPT_DIR/rxblock.tcl
source $SCRIPT_DIR/txblock.tcl



analyze -format verilog $UNMAPPED_DIR/uart.v

elaborate uart

current_design uart
link

# define the design environment
set_operating_conditions BCCOM
set_wire_load_mode top
set_wire_load_model -name TSMC16K_Conservative
#set_wire_load_model -name 0x150k -lib WORST -max
#set_driving_cell -lib_cell DFNTNQ -pin Q [all_inputs]
set_drive 0 [list clk rst_n]
set_load 2.0 [all_outputs]
set_max_fanout 10 uart
source $SCRIPT_DIR/name_rules_chen.scr

# set the optimization constraints
create_clock -period 100 -name clock [get_ports clk]
#create_generated_clock -name "clk_col" -source [get_ports {clk}] -divide_by 4 [get_pins {clock_gen/U92/ZN}]
#create_generated_clock -name "clk_row" -source [get_ports {clk}] -divide_by 400 [get_pins {clock_gen/clk_row_reg/Q}]
#create_generated_clock -name "clk_row_delay" -source [get_ports {clk}] -divide_by 400 [get_pins {clock_gen/clk_row_delay_reg/Q}]

#set_false_path -from [get_ports clk] -to [get_ports reset]
set_dont_touch_network [list clk]
set_clock_uncertainty 2 [get_clocks clock]
set_clock_transition 2 [get_clocks clock]
set_clock_uncertainty -hold 0.3 [get_clocks clock]
set_input_delay 40 -clock clock [all_inputs]
set_output_delay 40 -clock clock [all_outputs]
#set_max_area 1000000

#ideal_net
#set_ideal_net [list clk_col clk_row clk_row_delay]
#set size_only_on_ideal_boundary_cells true

# map and optimize the design
uniquify
set_fix_multiple_port_nets -all -feedthroughs
set_fix_multiple_port_nets -all -buffer_constants
#set_fix_hold clock
compile
remove_attribute [get_designs rxblock] dont_touch
remove_attribute [get_designs txblock] dont_touch
remove_attribute [get_designs baud_rate] dont_touch
 

compile -incremental -only_design_rule
change_names -rules valid_name  -hierarchy
remove_unconnected_ports [get_cells *]

# analyze and debug the design and save report to reports directory
redirect $REPORTS_DIR/all_violators.rpt {report_constrain -all_violators}
redirect $REPORTS_DIR/area.rpt {report_area}
redirect $REPORTS_DIR/resource.rpt {report_resources}
redirect $REPORTS_DIR/timing.rpt {report_timing}
#redirect $REPORTS_DIR/timing_h.rpt {report_timing}

# save the design database
write -format db -hierarchy -output $MAPPED_DIR/uart.db
write -format verilog -hierarchy -output $MAPPED_DIR/uart.v
write_sdf $MAPPED_DIR/uart.sdf
write_sdc $MAPPED_DIR/uart.sdc
#write_constraints -format sdf-v2.1 -hierarchy -output $MAPPED_DIR/top2.sdf
write_constraints -format sdf-v2.1 -max_nets 0.05 -max_path_timing -max_paths 1 -hierarchy -output $MAPPED_DIR/uart2.sdf
