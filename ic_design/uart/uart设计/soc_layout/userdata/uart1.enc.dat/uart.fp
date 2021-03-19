######################################################
#                                                    #
#  Cadence Design Systems                            #
#  FirstEncounter Floor Plan Information             #
#                                                    #
######################################################
# Created by First Encounter v05.20-p002_1 on Fri Dec  5 10:26:29 2008

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
GCellGrid: X 364.7000 2 6.9700
GCellGrid: X 0.7000 27 14.0000
GCellGrid: X 0.0000 2 0.7000
GCellGrid: Y 301.2500 2 11.2500
GCellGrid: Y 1.2500 25 12.5000
GCellGrid: Y 0.0000 2 1.2500

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
Guide: u3/add_59 -2147392.6480 -2147246.1480 -2147367.4480 -2147146.1480 0
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
