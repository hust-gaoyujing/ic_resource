# specify the libraries
set search_path      [list /export/home/forest/xs/lib /export/home/forest/xs]
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
analyze -format verilog $UNMAPPED_DIR/txblock.v

elaborate txblock

current_design txblock
link

# define the design environment
set_operating_conditions BCCOM
set_wire_load_mode top
set_wire_load_model -name TSMC16K_Conservative
#set_wire_load_model -name 0x150k -lib WORST -max
#set_driving_cell -lib_cell DFNTNQ -pin Q [all_inputs]
set_drive 0 [list clk16 rst_n]
set_load 2.0 [all_outputs]
set_max_fanout 4 txblock
#source $SCRIPT_DIR/name_rules_chen.scr

# set the optimization constraints
create_clock -period 50000 -name clock [get_ports clk16]
#set_false_path -from [get_ports clk16] -to [get_ports rst_n]
set_dont_touch_network [list clk16]
set_clock_uncertainty 2 [get_clocks clock]
set_clock_transition 2 [get_clocks clock]
set_clock_uncertainty -hold 0.3 [get_clocks clock]
set_input_delay 40 -clock clock [all_inputs]
set_output_delay 40 -clock clock [all_outputs]
#set_max_area 1000000


# map and optimize the design
uniquify
set_fix_multiple_port_nets -all -feedthroughs
set_fix_multiple_port_nets -all -buffer_constants
#set_fix_hold clock
compile
#compile -incremental -only_design_rule
#check_test

#change_names -rules valid_name  -hierarchy
remove_unconnected_ports [get_cells *]

set_dont_touch txblock 

# analyze and debug the design and save report to reports directory
redirect $REPORTS_DIR/charge_row_violators.rpt {report_constrain -all_violators}
redirect $REPORTS_DIR/charge_row_area.rpt {report_area}
redirect $REPORTS_DIR/charge_row_resource.rpt {report_resources}
redirect $REPORTS_DIR/charge_row_timing.rpt {report_timing}
#redirect $REPORTS_DIR/timing_h.rpt {report_timing -delay_min}

# save the design database
write -format db -hierarchy -output $MAPPED_DIR/txblock.db
write -format verilog -hierarchy -output $MAPPED_DIR/txblock.v
write_sdf $MAPPED_DIR/txblock.sdf
write_sdc $MAPPED_DIR/txblock.sdc
#write_constraints -format sdf-v2.1 -hierarchy -output $MAPPED_DIR/txblock.sdf
write_constraints -format sdf-v2.1 -max_nets 0.05 -max_path_timing -max_paths 1 -hierarchy -output $MAPPED_DIR/txblock.sdf