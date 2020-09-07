#!/bin/bash

test_num=0
REPORT_FILE=test_report.txt
export COLUMNS
export LINES
echo $COLUMNS 
echo $LINES
echo "Test run starting at $(date)" >>${REPORT_FILE}

#==============================================================================
#	Operations available in the Vacuum simulation.
#==============================================================================
function set_timeout () {
	printf "o${1} "
}
function set_delay () {
	printf "m${1} "
}
function vacuum_motion () {
	printf "p "
}
function set_weight () {
	printf "w${1} "
}
function return_to_base () {
	printf "b "
}
function drop_dust () {
	printf "d${1},${2}"
}
function drop_slime () {
	printf "s${1},${2}"
}
function drop_trash () {
	printf "t${1},${2}"
}
function set_vacuum_motion () {
	printf "v${1},${2},${3}"
}
function display_help_menu (){
	printf "? "
}
function set_battery_level (){
	printf "y${1}"
}
function reset () {
	printf "r${1} "
}
function left () {
	printf "j "
}
function right () {
	printf "l "
}
function up () {
	printf "i "
}
function down () {
	printf "k "
}
function quit () {
	printf "q"
}

function loop () {
	if (( $# >= 1 )) && (( $1 > 0 )) 
	then
		for (( i = 0; i < $1; i++ ))
		do
			printf "!"
		done
	else
		printf "!"
	fi
}

function enter () {
	printf "${1} "
}

#========================================================================
#	Run a single test.
#========================================================================	

function run_test () {
	(( test_num ++ ))
	echo "Test    : ${test_num}"
	echo "Category: ${category}"
	echo "Details : ${details}"
	echo "Expect  : ${expect}"
	
	read -p "Press r to run the test or s to skip..." run_or_skip
	
	if [ "${run_or_skip}" == "r" ]
	then
		echo ${cmd} | ./CAB202_n10026924
		read -p "Did the test pass or fail (p/f)? " result
		echo "Test ${test_num}: ${result}" >>${REPORT_FILE}
	fi
}
#==============================================================================
category="Test Loop"
details="Testing loop function "
expect="Vacuum spawns. No Rubbish. Loop set to 2 seconds. Simulation quits."

cmd=$(

	loop 200
	quit
	
)

run_test 
#==============================================================================
category="Test Command Input - Timeout"
details="Enter timeout period. "
expect="Vacuum spawns. No Rubbish. Set timeout to 5 seconds. Simulation quits after 5 seconds."

cmd=$(
	enter 0
	enter 0
	enter 0
	set_timeout 5
	
)

run_test 
#==============================================================================
category="Test Vacuum - Spawn"
details="Spawn Vacuum. "
expect="Vacuum spawns in the centre of the screen. No rubbish. Quits after 5 seconds."

cmd=$(
	enter 0
	enter 0
	enter 0
	loop 500
	quit
	
)

run_test
#==============================================================================
category="Test Vacuum"
details="Automatic Movement 1 "
expect="Vacuum spawns in the centre of the screen. No rubbish. Vacuum starts moving
		after 3 seconds. Simulation quits after 10 seconds."

cmd=$(
	enter 0
	enter 0
	enter 0
	set_timeout 10
	vacuum_motion
	
)

run_test
#==============================================================================
category="Test Vacuum - Manual Movement"
details="Move Left."
expect="Vacuum spawns. Waits. Moves left until it stops at the wall boundery."

cmd=$(
	enter 0
	enter 0
	enter 0
	set_timeout 5
	for (( i = 0; i < 100; i++ )) ; do left ; done
)

run_test 
#==============================================================================
category="Test Vacuum - Manual Movement"
details="Move right."
expect="Vacuum spawns. Waits. Moves right until it stops at the wall boundery."

cmd=$(
	enter 0
	enter 0
	enter 0
	set_timeout 5
	for (( i = 0; i < 100; i++ )) ; do right ; done
)

run_test 
#==============================================================================
category="Test Vacuum - Manual Movement"
details="Move up."
expect="Vacuum spawns. Waits. Moves up until it stops at the wall boundery."

cmd=$(
	enter 0
	enter 0
	enter 0
	set_timeout 5
	for (( i = 0; i < 100; i++ )) ; do up ; done
)

run_test 
#==============================================================================
category="Test Vacuum - Manual Movement"
details="Move down."
expect="Vacuum spawns. Waits. Moves down until it stops at the wall boundery."

cmd=$(
	enter 0
	enter 0
	enter 0
	set_timeout 5
	for (( i = 0; i < 100; i++ )) ; do down ; done
)

run_test
#==============================================================================
category="Test Vacuum - Manual Movement"
details=" Move Vacuum right, left, down and up."
expect="Vacuum spawns. Moves 10 points right, left, down and up."

cmd=$(
	enter 0
	enter 0
	enter 0
	set_timeout 6
	for (( i = 0; i < 10; i++ )) ; do right ; done
	loop 50
	for (( i = 0; i < 10; i++ )) ; do left ; done
	loop 50
	for (( i = 0; i < 10; i++ )) ; do down ; done
	loop 50
	for (( i = 0; i < 10; i++ )) ; do up ; done
	
)

run_test 
 
#==============================================================================
category="Test Vacuum - Return to base"
details="After command input vacuum will return to base"
expect="Vacuum spawned. After 1 seconds it will return to base. Then start automatically moving."

cmd=$(
	enter 0
	enter 0
	enter 0
	loop 100
	return_to_base
	set_timeout 10
	
)

run_test 
#==============================================================================
category="Test Command Input"
details="Change Weight of vacuum 1"
expect="Change weight of vacuum to 40 g. Vacuum stays in place."
cmd=$(
	enter 0
	enter 0
	enter 0
	set_timeout 5
	loop 200
	set_weight 40
)

run_test 
#==============================================================================
category="Test Command Input"
details="Change Weight of vacuum 2"
expect="Change weight of vacuum to 40 g. Vacuum returns to base."

cmd=$(
	enter 0
	enter 0
	enter 0
	set_timeout 5
	loop 200
	set_weight 50
)

run_test 
#==============================================================================
category="Test Command Input"
details="Change Battery Level 1"
expect="Change battery level of vacuum to 50. Vacuum stays in place."

cmd=$(
	enter 0
	enter 0
	enter 0
	set_timeout 5
	loop 200
	set_battery_level 50
)

run_test 
#==============================================================================
category="Test Command Input"
details="Change Battery Level 2"
expect="Change battery level of vacuum to 20. Vacuum returns to base."

cmd=$(
	enter 0
	enter 0
	enter 0
	set_timeout 5
	loop 200
	set_battery_level 20
)

run_test 
#==============================================================================
category="Test Command Input "
details="New Vacuum location"
expect="Set vacuum location to (10,10)."

cmd=$(
	enter 0
	enter 0
	enter 0
	set_timeout 10
	loop 200
	set_vacuum_motion 10 10 0
	
)

run_test 
#==============================================================================
category="Test Command Input "
details="New Vacuum location and direction"
expect="set vacuum location to (10,10) and direction to 60 degrees."
cmd=$(
	enter 0
	enter 0
	enter 0
	set_timeout 10
	loop 200
	set_vacuum_motion 10 10 60
	
)

run_test 
#==============================================================================
category="Test Command Input"
details="Reset"
expect="Simulation resets after 2 seconds."

cmd=$(
	enter 0
	enter 0
	enter 0
	loop 200
	reset 0
	enter 0
	loop 200
	set_timeout 2
	
)

run_test 
#==============================================================================
category="Test Command Input"
details="Set Delay. Low Value. "
expect="Enter millisecond delay for loop function. Vacuum moves faster around screen."

cmd=$(
	enter 0
	enter 0
	enter 0
	set_delay 1
	vacuum_motion
	set_timeout 10
	
)

run_test 
#==============================================================================
category="Test Command Input"
details="Set Delay. High Value. "
expect="Enter millisecond delay for loop function. Vacuum moves slowly around screen."

cmd=$(
	enter 0
	enter 0
	enter 0
	set_delay 100
	vacuum_motion
	set_timeout 10
	
)

run_test 
#==============================================================================
category="Test Command Inpt - Help Menu"
details="Display Help Menu"
expect="Vacuum spawn. No Rubbish. Wait 5 seconds. Help Menu Appears."

cmd=$(
	enter 0
	enter 0
	enter 0
	set_timeout 5
	loop 200
	enter ?
)

run_test 
#==============================================================================
category="Test Command Input - Spawn Rubbish"
details="Spawn dust"
expect="Spawn dust at (40, 30)"
cmd=$(
	enter 0
	enter 0
	enter 0
	set_timeout 5
	loop 200
	drop_dust 10 10
)

run_test 
#==============================================================================
category="Test Command Input - Spawn Rubbish"
details="Spawn slime"
expect=" Spawn slime at (20, 30)"
cmd=$(
	enter 0
	enter 0
	enter 0
	set_timeout 5
	loop 200
	drop_slime 20 20
)

run_test 
#==============================================================================
category="Test Command Input - Spawn Rubbish"
details="Spawn trash "
expect=" Spawn trash at (20, 10)"
cmd=$(
	enter 0
	enter 0
	enter 0
	set_timeout 5
	loop 200
	drop_trash 30 30
)

run_test 
#==============================================================================
category="Test Rubbish - Spawn MAX Dust"
details="Enter MAX amount of Dust"
expect="Dust is spawned randomly around the vacuum, charging station. Vacuum is stationary. Simulation quits after 3 seconds."

cmd=$(
	enter 0
	enter 0
	enter 1000
	loop 150
	reset 0
	enter 0
	enter 1000
	loop 150
	reset 0
	enter 0
	enter 1000
	loop 150
	reset 0
	enter 0
	enter 1000
	loop 150
	reset 0
	enter 0
	enter 1000
	set_timeout 3
	
)

run_test 

#==============================================================================
category="Test Rubbish - Spawn MAX Slime"
details="Enter MAX amount of slime"
expect="Slime is spawned randomly around the Vacuum and Charging Station. Vacuum is stationary. Then 'Quits' after 3 seconds."

cmd=$(
	enter 10
	enter 0
	enter 0
	loop 100
	reset 10
	enter 0
	enter 0
	loop 100
	reset 10
	enter 0
	enter 0
	loop 100
	reset 10
	enter 0
	enter 0
	loop 100
	reset 10
	enter 0
	enter 0
	set_timeout 3
	
)

run_test 

#==============================================================================
category="Test Rubbish - Spawn MAX Trash"
details="Enter MAX amount of trash"
expect="Trash is spawned randomly around the Vacuum and Charging Station. Vacuum is stationary. Then 'Quits' after short duration."

cmd=$(
	enter 0
	enter 5
	enter 0
	loop 100
	reset 0
	enter 5
	enter 0
	loop 100
	reset 0
	enter 5
	enter 0
	loop 100
	reset 0
	enter 5
	enter 0
	loop 100
	reset 0
	enter 5
	enter 0
	set_timeout 3
	
)

run_test 
#==============================================================================
category="Spawn ALL Rubbish"
details="All rubbish is spawned"
expect="Dust, slime and trash is spawned randomly avoiding the vacuum and the charging station."

cmd=$(
	enter 10
	enter 5
	enter 1000
	loop 200
	reset 10
	enter 5
	enter 1000
	loop 200
	reset 10
	enter 5
	enter 1000
	loop 200
	reset 10
	enter 5
	enter 1000
	loop 200
	reset 10
	enter 5
	enter 1000
	set_timeout 3
)

run_test 
#==============================================================================
category="Test Vacuum Pump"
details="Collect Dust"
expect="Spawns 1 dust next to vacuum. Vacuum collects dust."

(( CX = $COLUMNS / 4, CY = $LINES / 2 ))

cmd=$(
	enter 0
	enter 0
	enter 0
	set_timeout 10
	drop_dust ${CX} ${CY}
	for (( i = 0; i < 50; i++ )) ; do left ; done
	
)

run_test 
#==============================================================================
category="Test Vacuum Pump"
details="Collect Slime"
expect="Spawns 1 slime next to vacuum. Vacuum collects slime."

(( CX = $COLUMNS / 4, CY = $LINES / 2 ))

cmd=$(
	enter 0
	enter 0
	enter 0
	set_timeout 10
	drop_slime ${CX} ${CY}
	for (( i = 0; i < 50; i++ )) ; do left ; done
	
)

run_test 
#==============================================================================
category="Test Vacuum Pump"
details="Collect Trash"
expect="Spawns 1 trash next to vacuum. Vacuum collects trash."

(( CX = $COLUMNS / 4, CY = $LINES / 2))

cmd=$(
	enter 0
	enter 0
	enter 0
	set_timeout 10
	drop_trash ${CX} ${CY}
	for (( i = 0; i < 50; i++ )) ; do left ; done
	
	
)

run_test 
#==============================================================================
category="Test Vacuum Pump"
details="Turn pump off when in return to base mode"
expect="MAX rubbish is spawned. Vacuum moving automatically. Vacuum pump is active. When weight limit is reached pump is turned off."

cmd=$(
	enter 10
	enter 5
	enter 1000
	set_timeout 30
	vacuum_motion
	
	
)

run_test 
#==============================================================================
category="Test Pixel Collision"
details="Vacuum sits next to charging staton - WEST side"
expect="Move vacuum next to charging station without activating bounce."

(( CX = $COLUMNS / 4, CY = ($LINES / 3)+3))

cmd=$(
	enter 0
	enter 0
	enter 0
	set_timeout 5
	for (( i = 0; i < 6; i++ )) ; do left ; done
	for (( i = 0; i < ${CY}; i++ )) ; do up ; done
	
	
)

run_test 
#==============================================================================
category="Test Pixel Collision"
details="Vacuum sits next to charging staton - EAST side"
expect="Move vacuum next to charging station without activating bounce."

(( CX = $COLUMNS / 4, CY = ($LINES / 3)+3 ))

cmd=$(
	enter 0
	enter 0
	enter 0
	set_timeout 5
	for (( i = 0; i < 6; i++ )) ; do right ; done
	for (( i = 0; i < ${CY}; i++ )) ; do up ; done
	
	
)

run_test 
#==============================================================================
category="Test Pixel Collision"
details="Vacuum sits next to charging staton - NORTH side"
expect="Move vacuum next to charging station without activating bounce."

(( CX = $COLUMNS / 4, CY = ($LINES / 3)+2 ))

cmd=$(
	enter 0
	enter 0
	enter 0
	set_timeout 5
	for (( i = 0; i < ${CY}; i++ )) ; do up ; done
	
	
)

run_test 
#==============================================================================
category="Test Pixel Collision"
details="Vacuum sits next to dust - WEST side"
expect="Move vacuum next to dust without colliding."

(( CX = $COLUMNS / 4, CY = ($LINES / 2), LD = CX - 3))

cmd=$(
	enter 0
	enter 0
	enter 0
	set_timeout 5
	drop_dust ${CX} ${CY}
	for (( i = 0; i < ${LD}; i++ )) ; do left ; done
	
	
)

run_test 
#==============================================================================
category="Test Pixel Collision"
details="Vacuum sits next to slime - WEST side"
expect="Move vacuum next to slime without colliding."

(( CX = $COLUMNS / 4, CY = ($LINES / 2), LD = CX - 7 ))

cmd=$(
	enter 0
	enter 0
	enter 0
	set_timeout 5
	drop_slime ${CX} ${CY}
	for (( i = 0; i < ${LD}; i++ )) ; do left ; done
	
	
)

run_test 
#==============================================================================
category="Test Pixel Collision"
details="Vacuum sits next to trash - WEST side"
expect="Move vacuum next to trash without colliding."

(( CX = $COLUMNS / 4, CY = ($LINES / 2), LD = CX - 9 ))

cmd=$(
	enter 0
	enter 0
	enter 0
	set_timeout 5
	drop_trash ${CX} ${CY}
	for (( i = 0; i < ${LD}; i++ )) ; do left ; done
	
	
)

run_test 
#==============================================================================
category="Test Pixel Collision"
details="Vacuum sits next to dust - EAST side"
expect="Move vacuum next to dust without colliding."

(( CX = ($COLUMNS / 2)+($COLUMNS / 4), CY = ($LINES / 2), LD = ($COLUMNS / 4) - 4))

cmd=$(
	enter 0
	enter 0
	enter 0
	set_timeout 5
	drop_dust ${CX} ${CY}
	for (( i = 0; i < ${LD}; i++ )) ; do right ; done
	
	
)

run_test 
#==============================================================================
category="Test Pixel Collision"
details="Vacuum sits next to slime - EAST side"
expect="Move vacuum next to slime without colliding."

(( CX = ($COLUMNS / 2)+($COLUMNS / 4), CY = ($LINES / 2), LD = ($COLUMNS / 4) - 4))

cmd=$(
	enter 0
	enter 0
	enter 0
	set_timeout 5
	drop_slime ${CX} ${CY}
	for (( i = 0; i < ${LD}; i++ )) ; do right ; done
	
	
)

run_test 
#==============================================================================
category="Test Pixel Collision"
details="Vacuum sits next to trash - EAST side"
expect="Move vacuum next to trash without colliding."

(( CX = ($COLUMNS / 2)+($COLUMNS / 4), CY = ($LINES / 2), LD = ($COLUMNS / 4)))

cmd=$(
	enter 0
	enter 0
	enter 0
	set_timeout 5
	drop_trash ${CX} ${CY}
	for (( i = 0; i < ${LD}; i++ )) ; do right ; done
	
	
)

run_test 
#==============================================================================
category="Test Pixel Collision"
details="Vacuum sits next to dust - NORTH side"
expect="Move vacuum next to dust without colliding."

(( CX = ($COLUMNS / 2), CY = ($LINES / 4), LD = ($LINES / 4) - 3))

cmd=$(
	enter 0
	enter 0
	enter 0
	set_timeout 5
	drop_dust ${CX} ${CY}
	for (( i = 0; i < ${LD}; i++ )) ; do up ; done
	
	
)

run_test 
#==============================================================================
category="Test Pixel Collision"
details="Vacuum sits next to slime - NORTH side"
expect="Move vacuum next to slime without colliding."

(( CX = ($COLUMNS / 2)+1, CY = ($LINES / 4), LD = ($LINES / 4) - 6))

cmd=$(
	enter 0
	enter 0
	enter 0
	set_timeout 5
	drop_slime ${CX} ${CY}
	for (( i = 0; i < ${LD}; i++ )) ; do up ; done
	
	
)

run_test 
#==============================================================================
category="Test Pixel Collision"
details="Vacuum sits next to trash - NORTH side"
expect="Move vacuum next to trash without colliding."

(( CX = ($COLUMNS / 2), CY = ($LINES / 4), LD = ($LINES / 4) - 8))

cmd=$(
	enter 0
	enter 0
	enter 0
	set_timeout 5
	drop_trash ${CX} ${CY}
	for (( i = 0; i < ${LD}; i++ )) ; do up ; done
	
	
)

run_test 
#==============================================================================
category="Test Pixel Collision"
details="Vacuum sits next to dust - SOUTH side"
expect="Move vacuum next to dust without colliding."

(( CX = ($COLUMNS / 2), CY = ($LINES / 2)+($LINES/4), LD = ($LINES / 4)-4))

cmd=$(
	enter 0
	enter 0
	enter 0
	set_timeout 5
	drop_dust ${CX} ${CY}
	for (( i = 0; i < ${LD}; i++ )) ; do down ; done
	
	
)

run_test 
#==============================================================================
category="Test Pixel Collision"
details="Vacuum sits next to slime - SOUTH side"
expect="Move vacuum next to slime without colliding."

(( CX = ($COLUMNS / 2)+1, CY = ($LINES / 2)+($LINES/4), LD = ($LINES / 4)-3))

cmd=$(
	enter 0
	enter 0
	enter 0
	set_timeout 5
	drop_slime ${CX} ${CY}
	for (( i = 0; i < ${LD}; i++ )) ; do down ; done
	
	
)

run_test 
#==============================================================================
category="Test Pixel Collision"
details="Vacuum sits next to trash - SOUTH side"
expect="Move vacuum next to trash without colliding."

(( CX = ($COLUMNS / 2), CY = ($LINES / 2)+($LINES/4), LD = ($LINES / 4)))

cmd=$(
	enter 0
	enter 0
	enter 0
	set_timeout 5
	drop_trash ${CX} ${CY}
	for (( i = 0; i < ${LD}; i++ )) ; do down ; done
	
	
)

run_test 

#==============================================================================
category="Run Simulation"
details="Full simulation play"
expect="Run simulation for 60 seconds."

cmd=$(
	enter 10
	enter 5
	enter 1000
	set_delay 10
	set_timeout 60
	for (( i = 0; i < 14; i++ )) ; do left ; done
	for (( i = 0; i < 10; i++ )) ; do down ; done
	vacuum_motion
	for (( i = 0; i < 2; i++ )) ; do right ; done
	vacuum_motion
)

run_test