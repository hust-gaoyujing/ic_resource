######################################################
#                                                    #
#  Cadence Design Systems                            #
#  FirstEncounter Floor Plan Information             #
#                                                    #
######################################################
# Created by First Encounter v05.20-p002_1 on Fri Dec  5 11:01:47 2008

Version: 4

Head Box: 0.0000 0.0000 371.6700 312.5000
IO Box: 0.0000 0.0000 371.6700 312.5000
Core Box: 50.4000 50.0000 321.6700 262.5000
UseStdUtil: false

######################################################
#  Core Rows Parameters:                             #
######################################################
#  RowSpacing = 0.000000 		#
#  RowSpacingType = 2 		#
#  RowFlip = 2 		#
#  Core Row Site: core 		#

##############################################################################
#  DefRow: <name> <site> <x> <y> <orient> <num_x> <num_y> <step_x> <step_y>  #
##############################################################################
DefRow: ROW_0 core 50.4000 50.0000 FS 193 1 1.4000 0.0000
DefRow: ROW_1 core 50.4000 62.5000 N 193 1 1.4000 0.0000
DefRow: ROW_2 core 50.4000 75.0000 FS 193 1 1.4000 0.0000
DefRow: ROW_3 core 50.4000 87.5000 N 193 1 1.4000 0.0000
DefRow: ROW_4 core 50.4000 100.0000 FS 193 1 1.4000 0.0000
DefRow: ROW_5 core 50.4000 112.5000 N 193 1 1.4000 0.0000
DefRow: ROW_6 core 50.4000 125.0000 FS 193 1 1.4000 0.0000
DefRow: ROW_7 core 50.4000 137.5000 N 193 1 1.4000 0.0000
DefRow: ROW_8 core 50.4000 150.0000 FS 193 1 1.4000 0.0000
DefRow: ROW_9 core 50.4000 162.5000 N 193 1 1.4000 0.0000
DefRow: ROW_10 core 50.4000 175.0000 FS 193 1 1.4000 0.0000
DefRow: ROW_11 core 50.4000 187.5000 N 193 1 1.4000 0.0000
DefRow: ROW_12 core 50.4000 200.0000 FS 193 1 1.4000 0.0000
DefRow: ROW_13 core 50.4000 212.5000 N 193 1 1.4000 0.0000
DefRow: ROW_14 core 50.4000 225.0000 FS 193 1 1.4000 0.0000
DefRow: ROW_15 core 50.4000 237.5000 N 193 1 1.4000 0.0000
DefRow: ROW_16 core 50.4000 250.0000 FS 193 1 1.4000 0.0000

######################################################
#  Track: dir start number space layer_num layer1 ...#
######################################################
Track: Y 1.2500 249 1.2500 1 4
Track: X 0.7000 265 1.4000 1 4
Track: X 0.7000 265 1.4000 1 3
Track: Y 1.2500 249 1.2500 1 3
Track: Y 1.2500 249 1.2500 1 2
Track: X 0.7000 265 1.4000 1 2
Track: X 0.7000 265 1.4000 1 1
Track: Y 1.2500 249 1.2500 1 1

######################################################
#  GCellGrid: dir start number space                 #
######################################################
GCellGrid: Y 312.5050 1 36.8800
GCellGrid: Y 50.6250 10 25.0000
GCellGrid: Y -0.0050 2 25.6300
GCellGrid: X 371.6750 1 21.6000
GCellGrid: X 50.0750 13 25.0000
GCellGrid: X -0.0050 2 25.0800

######################################################
#  SpareCell: cellName                               #
#  SpareInst: instName                               #
######################################################

######################################################
#  ScanGroup: groupName startPin stopPin             #
######################################################

######################################################
#  JtagCell:  leafCellName                           #
#  JtagInst:  <instName | HInstName>                 #
######################################################

##############################################################################
#  BlackBox: -cell <cell_name> { -size <x> <y> | { -area <um^2> \            #
#    -gatecount <count> <areapergate> <utilization> } \                      #
#    [-minwidth <w> | -minheight <h> | -fixedwidh <w> | -fixedheight <h>] \  #
#    -aspectratio <ratio> }                                                  #
#    [-boxList <nrConstraintBox>                                             #
#    ConstraintBox: <llx> <lly> <urx> <ury>                                  #
#    ... ]                                                                   #
##############################################################################

#########################################################
#  PhysicalNet: <name> [-pwr|-gnd|-tiehi|-tielo]        #
#########################################################
PhysicalNet: _FILLS_RESERVED
PhysicalNet: GND -gnd
PhysicalNet: VDD -pwr

#########################################################
#  PhysicalInstance: <name> <cell> <orient> <llx> <lly> #
#########################################################

#####################################################################
#  Group: <group_name> <nrHinst> [-isPhyHier]                       #
#    <inst_name>                                                    #
#    ...                                                            #
#####################################################################

#####################################################################
#  Fence:  <name> <llx> <lly> <urx> <ury> <nrConstraintBox>         #
#    ConstraintBox: <llx> <lly> <urx> <ury>                         #
#    ...                                                            #
#  Region: <name> <llx> <lly> <urx> <ury> <nrConstraintBox>         #
#    ConstraintBox: <llx> <lly> <urx> <ury>                         #
#    ...                                                            #
#  Guide:  <name> <llx> <lly> <urx> <ury> <nrConstraintBox>         #
#    ConstraintBox: <llx> <lly> <urx> <ury>                         #
#    ...                                                            #
#  SoftGuide: <name>                                                #
#    ...                                                            #
#####################################################################
Guide: u3/add_59 50.4000 50.0000 50.4000 50.0000 0
Guide: u3 131.6000 175.0000 245.0000 250.0000 0
Guide: u2 198.8000 62.5000 287.0000 162.5000 0
Guide: u1/lt_41 282.8000 187.5000 312.2000 250.0000 0
Guide: u1/add_42 65.8000 125.0000 116.2000 250.0000 0
Guide: u1/lte_54 127.4000 75.0000 184.8000 150.0000 0

######################################################
#  NetGroup: <group_name> <nrNetsInGroup>            #
######################################################

######################################################
#  CellPtnCutList: <cellName> <num_ptn_cut>          #
######################################################

######################################################
#  Instance: <name> <orient> <llx> <lly>             #
######################################################
Instance: clk__L2_I0 MX 175.0000 150.0000
Instance: clk__L1_I0 MX 218.4000 250.0000
Instance: u1/out_clk_reg R180 280.0000 125.0000
Instance: u1/counter_reg[1] MY 285.6000 62.5000
Instance: u1/counter_reg[2] MX 89.6000 125.0000
Instance: u1/counter_reg[0] MY 252.0000 62.5000
Instance: u1/counter_reg[4] R0 117.6000 187.5000
Instance: u1/counter_reg[3] MX 144.2000 125.0000
Instance: u1/counter_reg[5] MX 54.6000 75.0000
Instance: u1/counter_reg[9] R0 51.8000 187.5000
Instance: u1/counter_reg[6] R0 84.0000 137.5000
Instance: u1/counter_reg[8] MY 85.4000 162.5000
Instance: u1/counter_reg[7] MX 54.6000 125.0000
Instance: u1/counter_reg[13] R0 120.4000 237.5000
Instance: u1/counter_reg[10] R0 123.2000 212.5000
Instance: u1/counter_reg[14] MY 151.2000 212.5000
Instance: u1/counter_reg[12] R0 82.6000 212.5000
Instance: u1/counter_reg[15] MX 149.8000 200.0000
Instance: u1/counter_reg[11] R0 89.6000 187.5000
Instance: u1/divisor_copy_reg[9] R180 236.6000 150.0000
Instance: u1/divisor_copy_reg[8] R180 294.0000 150.0000
Instance: u1/divisor_copy_reg[5] R180 292.6000 100.0000
Instance: u1/divisor_copy_reg[4] MY 259.0000 87.5000
Instance: u1/divisor_copy_reg[10] MY 273.0000 187.5000
Instance: u1/divisor_copy_reg[6] MY 246.4000 137.5000
Instance: u1/divisor_copy_reg[11] MY 294.0000 162.5000
Instance: u1/divisor_copy_reg[7] MY 294.0000 137.5000
Instance: u1/divisor_copy_reg[3] R180 261.8000 100.0000
Instance: u1/divisor_copy_reg[2] MX 294.0000 75.0000
Instance: u1/divisor_copy_reg[1] R180 253.4000 125.0000
Instance: u1/divisor_copy_reg[12] R180 294.0000 175.0000
Instance: u1/divisor_copy_reg[13] R180 285.6000 225.0000
Instance: u1/divisor_copy_reg[15] R180 294.0000 200.0000
Instance: u1/divisor_copy_reg[14] R0 263.2000 237.5000

#################################################################
#  Block: <name> <orient> [<llx> <lly>]                         #
#         [<haloLeftMargin>  <haloBottomMargin>                 #
#          <haloRightMargin> <haloTopMargin> <haloFromInstBox>] #
#         [<IsInstDefCovered> <IsInstPreplaced>]                #
#                                                               #
#  Block with INT_MAX loc is for recording the halo block with  #
#  non-prePlaced status                                         #
#################################################################

######################################################
#  BlockLayerObstruct: <name> <layerX> ...           #
######################################################

######################################################
#  FeedthroughBuffer: <instName>                     #
######################################################

###########################################################################
#  PlacementBlockage: <llx> <lly> <urx> <ury> <type> [-pushdown] "<name>" #
#       type is one of {-hard|-soft|-partial ratio}                       #
#       ratio is a number between 0 and 1.                                #
###########################################################################

######################################################################################
# RouteBlockage: <llx> <lly> <urx> <ury> <typeId>                                    #
#   { <layerId ..> | -cutLayer <layerId ..> | <layerId ..> -cutLayer <layerId ..> }  #
#   [-fills] [-pushdown] "<name>"                                                    #
# RouteBlockagePoly: NrPts <x0> <y0> <x1> <y1> ... <xn> <yn>                         #
# RouteBlkAttr: <typeId>                                                             #
#   { <layerId ..> | -cutLayer <layerId ..> | <layerId ..> -cutLayer <layerId ..> }  #
#   [-fills | -pushdown] "<name>"                                                    #
######################################################################################

######################################################
#  PrerouteAsObstruct: <layer_treated_as_obstruct>   #
######################################################
PrerouteAsObstruct: 0x3

######################################################
#  NetWeight: <net_name> <weight (in integer)>       #
######################################################

#################################################################
#  SprFile: <file_name>                                         #
#################################################################
SprFile: uart.fp.spr

#######################################################################################
#  IOPin: <pinName> <x> <y> <side> <layerId> <width> <depth> {placed|fixed|-} <nrBox> #
#    PinBox: <llx> <lly> <urx> <ury>                                                  #
#    PinPoly: <nrPt> <x1> <y1> <x2> <y2> ...<xn> <yn>                                 #
#######################################################################################
IOPin: clk 216.3000 312.5000 N 4 0.6000 0.6000 placed 1
  PinBox: 216.0000 311.9000 216.6000 312.5000
IOPin: divisor[15] 371.6700 206.2500 E 3 0.6000 0.6000 placed 1
  PinBox: 371.0700 205.9500 371.6700 206.5500
IOPin: divisor[14] 279.3000 312.5000 N 2 0.6000 0.6000 placed 1
  PinBox: 279.0000 311.9000 279.6000 312.5000
IOPin: divisor[13] 371.6700 226.2500 E 3 0.6000 0.6000 placed 1
  PinBox: 371.0700 225.9500 371.6700 226.5500
IOPin: divisor[12] 371.6700 197.5000 E 3 0.6000 0.6000 placed 1
  PinBox: 371.0700 197.2000 371.6700 197.8000
IOPin: divisor[11] 371.6700 185.0000 E 3 0.6000 0.6000 placed 1
  PinBox: 371.0700 184.7000 371.6700 185.3000
IOPin: divisor[10] 371.6700 190.0000 E 3 0.6000 0.6000 placed 1
  PinBox: 371.0700 189.7000 371.6700 190.3000
IOPin: divisor[9] 371.6700 160.0000 E 3 0.6000 0.6000 placed 1
  PinBox: 371.0700 159.7000 371.6700 160.3000
IOPin: divisor[8] 371.6700 156.2500 E 3 0.6000 0.6000 placed 1
  PinBox: 371.0700 155.9500 371.6700 156.5500
IOPin: divisor[7] 371.6700 147.5000 E 3 0.6000 0.6000 placed 1
  PinBox: 371.0700 147.2000 371.6700 147.8000
IOPin: divisor[6] 371.6700 143.7500 E 3 0.6000 0.6000 placed 1
  PinBox: 371.0700 143.4500 371.6700 144.0500
IOPin: divisor[5] 371.6700 113.7500 E 3 0.6000 0.6000 placed 1
  PinBox: 371.0700 113.4500 371.6700 114.0500
IOPin: divisor[4] 371.6700 110.0000 E 3 0.6000 0.6000 placed 1
  PinBox: 371.0700 109.7000 371.6700 110.3000
IOPin: divisor[3] 371.6700 116.2500 E 3 0.6000 0.6000 placed 1
  PinBox: 371.0700 115.9500 371.6700 116.5500
IOPin: divisor[2] 269.5000 0.0000 S 2 0.6000 0.6000 placed 1
  PinBox: 269.2000 0.0000 269.8000 0.6000
IOPin: divisor[1] 371.6700 118.7500 E 3 0.6000 0.6000 placed 1
  PinBox: 371.0700 118.4500 371.6700 119.0500
IOPin: divisor[0] 192.5000 0.0000 S 2 0.6000 0.6000 placed 1
  PinBox: 192.2000 0.0000 192.8000 0.6000
IOPin: rst_n 217.7000 312.5000 N 2 0.6000 0.6000 placed 1
  PinBox: 217.4000 311.9000 218.0000 312.5000
IOPin: para_data_in[7] 0.0000 120.0000 W 3 0.6000 0.6000 placed 1
  PinBox: 0.0000 119.7000 0.6000 120.3000
IOPin: para_data_in[6] 185.5000 312.5000 N 2 0.6000 0.6000 placed 1
  PinBox: 185.2000 311.9000 185.8000 312.5000
IOPin: para_data_in[5] 185.5000 312.5000 N 4 0.6000 0.6000 placed 1
  PinBox: 185.2000 311.9000 185.8000 312.5000
IOPin: para_data_in[4] 188.3000 312.5000 N 2 0.6000 0.6000 placed 1
  PinBox: 188.0000 311.9000 188.6000 312.5000
IOPin: para_data_in[3] 188.3000 312.5000 N 4 0.6000 0.6000 placed 1
  PinBox: 188.0000 311.9000 188.6000 312.5000
IOPin: para_data_in[2] 182.7000 312.5000 N 2 0.6000 0.6000 placed 1
  PinBox: 182.4000 311.9000 183.0000 312.5000
IOPin: para_data_in[1] 182.7000 312.5000 N 4 0.6000 0.6000 placed 1
  PinBox: 182.4000 311.9000 183.0000 312.5000
IOPin: para_data_in[0] 191.1000 312.5000 N 2 0.6000 0.6000 placed 1
  PinBox: 190.8000 311.9000 191.4000 312.5000
IOPin: serial_out 0.0000 130.0000 W 3 0.6000 0.6000 placed 1
  PinBox: 0.0000 129.7000 0.6000 130.3000
IOPin: tx_en 121.1000 0.0000 S 2 0.6000 0.6000 placed 1
  PinBox: 120.8000 0.0000 121.4000 0.6000
IOPin: tx_flag 129.5000 0.0000 S 2 0.6000 0.6000 placed 1
  PinBox: 129.2000 0.0000 129.8000 0.6000
IOPin: serial_in 224.7000 312.5000 N 2 0.6000 0.6000 placed 1
  PinBox: 224.4000 311.9000 225.0000 312.5000
IOPin: para_data_out[7] 160.3000 312.5000 N 2 0.6000 0.6000 placed 1
  PinBox: 160.0000 311.9000 160.6000 312.5000
IOPin: para_data_out[6] 221.9000 312.5000 N 2 0.6000 0.6000 placed 1
  PinBox: 221.6000 311.9000 222.2000 312.5000
IOPin: para_data_out[5] 214.9000 312.5000 N 2 0.6000 0.6000 placed 1
  PinBox: 214.6000 311.9000 215.2000 312.5000
IOPin: para_data_out[4] 228.9000 312.5000 N 2 0.6000 0.6000 placed 1
  PinBox: 228.6000 311.9000 229.2000 312.5000
IOPin: para_data_out[3] 210.7000 312.5000 N 2 0.6000 0.6000 placed 1
  PinBox: 210.4000 311.9000 211.0000 312.5000
IOPin: para_data_out[2] 179.9000 312.5000 N 2 0.6000 0.6000 placed 1
  PinBox: 179.6000 311.9000 180.2000 312.5000
IOPin: para_data_out[1] 213.5000 312.5000 N 4 0.6000 0.6000 placed 1
  PinBox: 213.2000 311.9000 213.8000 312.5000
IOPin: para_data_out[0] 202.3000 312.5000 N 2 0.6000 0.6000 placed 1
  PinBox: 202.0000 311.9000 202.6000 312.5000
IOPin: rx_flag 371.6700 167.5000 E 3 0.6000 0.6000 placed 1
  PinBox: 371.0700 167.2000 371.6700 167.8000
IOPin: clk_tx 184.1000 0.0000 S 2 0.6000 0.6000 placed 1
  PinBox: 183.8000 0.0000 184.4000 0.6000

###########################################################$############################################################################################
#  GlobalNetConnection: <net_name> {-pin|-inst|-net} <base_name_pattern> -type {pgpin|net|tiehi|tielo} {-all|-module <name>|-region <box>} [-override] #
########################################################################################################################################################
GlobalNetConnection: VDD -pin VDD -inst * -type pgpin -all
GlobalNetConnection: VDD -inst * -type tiehi -all
GlobalNetConnection: GND -pin VSS -inst * -type pgpin -all
GlobalNetConnection: GND -inst * -type tielo -all

################################################################################
#  NetProperties: <net_name> [-special] [-def_prop {int|dbl|str} <value>]...   #
################################################################################
