#!/bin/bash

test_num=0
REPORT_FILE=test_report.txt

echo "Remember to do these two commands before running this script:
export COLUMNS
export LINES
"
echo "Test run starting at $(date)" >>${REPORT_FILE}

#==============================================================================
#	Operations available in the Robovac simulation.
#==============================================================================
function set_timeout () {
	printf "o${1} "
}
function set_delay () {
	printf "m${1} "
}
function move_robo () {
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
function set_robo_loc () {
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
		echo ${cmd} | ./a1_n5377544
		read -p "Did the test pass or fail (p/f)? " result
		echo "Test ${test_num}: ${result}" >>${REPORT_FILE}
	fi
}
#==============================================================================
category="Initialisation"
details="Testing screen setup and initialisation. FS 1, 2, 4, 5, 8."
expect="Robovac generates. Screen HUD, dock and border is drawn. No rubbish. Simulation quits after 10 seconds.
 NOTE: WILL NEED TO PRESS CTRL-C TO END TEST."

cmd=$(

	loop 1000
	quit
	enter q
	
)

run_test 
#==============================================================================
category="Test Loop"
details="Testing loop function. "
expect="Robovac generates. No Rubbish. Loop set to 2 seconds. Simulation quits after 2 seconds. 
NOTE: WILL NEED TO PRESS CTRL-C TO END TEST."

cmd=$(

	loop 200
	quit
	enter q
	
)

run_test 
#==============================================================================
category="Test Command Input - Timeout Value"
details="Enter timeout period. FS 3, 15.4, "
expect="Robovac generates. No Rubbish. Set timeout to 5 seconds. Simulation quits after 5 seconds."

cmd=$(
	enter 0
	enter 0
	enter 0
	set_timeout 5
	
)

run_test 
#==============================================================================
category="Test Robovac Generation"
details="Generate Robovac. FS 5"
expect="Robovac generates in the centre of the screen. No rubbish. Loop quits after 5 seconds. 
NOTE: WILL NEED TO PRESS CTRL-C TO END TEST."

cmd=$(
	enter 0
	enter 0
	enter 0
	loop 500
	quit
	
)

run_test
#==============================================================================
category="Test Robovac - Automated Movement"
details="Robovac Movement 1. FS 6.1, 6.2, 6.3 "
expect="Robovac generates in the centre of the screen. No rubbish. Robovac starts moving. 
Simulation quits after 15 seconds."

cmd=$(
	enter 0
	enter 0
	enter 0
	set_timeout 15
	move_robo
	
)

run_test
#==============================================================================
category="Test Robovac - Manual Movement"
details="Robovac Movement 2 - Move Left. FS 6, 15.2  "
expect="Robovac generates in the centre of the room. It moves left until it stops at the wall boundary."

cmd=$(
	enter 0
	enter 0
	enter 0
	set_timeout 5
	for (( i = 0; i < 100; i++ )) ; do left ; done
)

run_test 
#==============================================================================
category="Test Robovac - Manual Movement"
details="Robovac Movement 3 - Move right. FS 6, 15.2"
expect="Robovac generates in the centre of the room. It moves right until it stops at the wall boundary."

cmd=$(
	enter 0
	enter 0
	enter 0
	set_timeout 5
	for (( i = 0; i < 100; i++ )) ; do right ; done
)

run_test 
#==============================================================================
category="Test Robovac - Manual Movement"
details="Robovac Movement 4 - Move up. FS 6, 8.4, 8.5, 15.2"
expect="Robovac generates in the centre of the room. It moves up until it stops at the dock."

cmd=$(
	enter 0
	enter 0
	enter 0
	set_timeout 5
	for (( i = 0; i < 100; i++ )) ; do up ; done
)

run_test 
#==============================================================================

category="Test Robovac - Manual Movement"
details="Robovac Movement 5 - Move up. FS 6, 15.2"
expect="Robovac generates in the centre of the room. It moves left from the dock, then up until it stops at the wall boundary."

cmd=$(
	enter 0
	enter 0
	enter 0
	set_timeout 5
	for (( i = 0; i < 11; i++ )) ; do left ; done
	for (( i = 0; i < 100; i++ )) ; do up ; done
)

run_test 
#==============================================================================
category="Test Robovac - Manual Movement"
details="Robovac Movement 6 - Move down. FS 6, 15.2"
expect="Robovac generates in the centre of the room. It moves down until it stops at the wall boundary."

cmd=$(
	enter 0
	enter 0
	enter 0
	set_timeout 5
	for (( i = 0; i < 100; i++ )) ; do down ; done
)

run_test
#==============================================================================
category="Test Robovac - Manual Movement"
details="Robovac Movement 7 - Move Robovac right, left, down and up. FS 6, 15.2"
expect="Robovac generates in the centre of the room. It moves 15 points right, left, down and up."

cmd=$(
	enter 0
	enter 0
	enter 0
	set_timeout 6
	for (( i = 0; i < 15; i++ )) ; do right ; done
	loop 50
	for (( i = 0; i < 15; i++ )) ; do left ; done
	loop 50
	for (( i = 0; i < 15; i++ )) ; do down ; done
	loop 50
	for (( i = 0; i < 15; i++ )) ; do up ; done
	
)

run_test 
#==============================================================================
category="Test Robovac - Pausing"
details="Robovac Pausing"
expect="Robovac generates in the centre of the room. It starts standard movement, then pauses."

cmd=$(
	enter 0
	enter 0
	enter 0
	move_robo
	loop 50
	move_robo

	
)

run_test  
#==============================================================================
category="Test Command Input - Return To Base"
details="After command input Robovac will return to base. FS 9"
expect="Robovac generates at centre of room. It will start with standard movement, then return to base, recharge battery, then resume standard movement."

cmd=$(
	enter 0
	enter 0
	enter 0
	loop 100
	move_robo
	return_to_base
	set_timeout 10
	
)

run_test 
#==============================================================================
category="Test Command Input - Over-ride Weight"
details="Change Payload 1. FS 9, 11, 15.8"
expect="Change weight of Robovac to 30g. Robovac continues standard movement."
cmd=$(
	enter 0
	enter 0
	enter 0
	move_robo
	set_timeout 5
	loop 200
	set_weight 30
)

run_test 
#==============================================================================
category="Test Command Input - Over-ride Weight"
details="Change Payload 2. FS 9, 11, 15.8"
expect="Change weight of Robovac to 65g. Robovac returns to base, offloads payload and recharges battery, then continues standard movement."

cmd=$(
	enter 0
	enter 0
	enter 0
	move_robo
	set_timeout 10
	loop 200
	set_weight 65
)

run_test 
#==============================================================================
category="Test Command Input - Over-ride Battery"
details="Change Battery 1. FS 7, 9, 15.9"
expect="Vaccum begins standard movement. Change battery level to 40%. Robovac continues standard movement."

cmd=$(
	enter 0
	enter 0
	enter 0
	move_robo
	set_timeout 5
	loop 200
	set_battery_level 40
)

run_test 
#==============================================================================
category="Test Command Input - Over-ride Battery"
details="Change Battery 2. FS 7, 9, 15.9"
expect="Robovac begines standard movement. Change battery level to 20%. Robovac returns to base, recharges, and continues standard movement."

cmd=$(
	enter 0
	enter 0
	enter 0
	move_robo
	set_timeout 15
	loop 200
	set_battery_level 20
)

run_test 
#==============================================================================
category="Test Command Input - Over-ride Robovac Location"
details="New Robovac location and direction 1. FS 15.7, 6.5"
expect="Robovac is stationary. Set Robovac location to (10,10) with a heading of 0. Vacumm remains stationary."

cmd=$(
	enter 0
	enter 0
	enter 0
	set_timeout 5
	loop 200
	set_robo_loc 10 10 0
	
)

run_test 
#==============================================================================
category="Test Command Input - Over-ride Robovac Location"
details="New Robovac location and direction 2. FS 15.7, 6.5"
expect="set Robovac location to (10,10) and heading to 30 degrees."
cmd=$(
	enter 0
	enter 0
	enter 0
	set_timeout 5
	loop 200
	set_robo_loc 10 10 30
	
)

run_test 
#==============================================================================
category="Test Command Input - Reset Simulation"
details="Reset Simulation. FS 2.4, 5.4, 5.5, 11.1, 15.6"
expect="Simulation resets after 2 seconds."

cmd=$(
	enter 0
	enter 0
	enter 0
	loop 200
	reset 0
	enter 0
	enter 0
	loop 200
	set_timeout 2
	
)

run_test 
#==============================================================================
category="Test Command Input - Delay Over-Ride"
details="Set Low-Value Delay. FS 2.4, 7.1, 15.3 "
expect="Enter millisecond delay for loop function. Robovac moves faster around screen."

cmd=$(
	enter 0
	enter 0
	enter 0
	set_delay 1
	move_robo
	set_timeout 10
	
)

run_test 
#==============================================================================
category="Test Command Input - Delay Over-Ride"
details="Set High-Value Delay. FS 2.4, 7.1, 15.3 "
expect="Enter millisecond delay for loop function. Robovac moves slowly around screen."

cmd=$(
	enter 0
	enter 0
	enter 0
	set_delay 100
	move_robo
	set_timeout 10
	
)

run_test 
#==============================================================================
category="Test Command Inpt - Help Menu"
details="Display Help Menu. FS 15"
expect="Robovac generates at the centre of the screen. No Rubbish. Wait 5 seconds. Help Menu Appears."

cmd=$(
	enter 0
	enter 0
	enter 0
	set_timeout 50
	loop 200
	enter ?
)

run_test 
#==============================================================================
category="Test Command Input - Generate Rubbish"
details="Generate Dust Mote. FS 12, 15.1"
expect="Generate dust mote at (20, 20)"
cmd=$(
	enter 0
	enter 0
	enter 0
	set_timeout 5
	loop 200
	drop_dust 20 20
)

run_test 
#==============================================================================
category="Test Command Input - Generate Rubbish"
details="Generate Slime Pile. FS 12, 15.1"
expect=" Spawn slime at (20, 20)"
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
category="Test Command Input - Generate Litter"
details="Generate Litter Pile. FS 12, 15.1"
expect=" Spawn litter at (20, 20)"
cmd=$(
	enter 0
	enter 0
	enter 0
	set_timeout 5
	loop 200
	drop_trash 20 20
)

run_test 
#==============================================================================
category="Test Rubbish - Generate maximum amount of dust"
details="Enter maximum amount of Dust. FS 12"
expect="Dust is generated randomly around the Robovac and dock 5 times. Robovac is stationary. Simulation quits after 3 seconds."

cmd=$(
	enter 1000
	enter 0
	enter 0
	loop 150
	reset 1000
	enter 0
	enter 0
	loop 150
	reset 1000
	enter 0
	enter 0
	loop 150
	reset 1000
	enter 0
	enter 0
	loop 150
	reset 1000
	enter 0
	enter 0
	set_timeout 3
	
)

run_test 

#==============================================================================
category="Test Rubbish - Spawn maximum amount of Slime"
details="Enter maximum amount of slime. FS 12"
expect="Slime is spawned randomly around the Robovac and Charging Station. Robovac is stationary. Then 'Loop quits' after 3 seconds."

cmd=$(
	enter 0
	enter 10
	enter 0
	loop 100
	reset 0
	enter 10
	enter 0
	loop 100
	reset 0
	enter 10
	enter 0
	loop 100
	reset 0
	enter 10
	enter 0
	loop 100
	reset 0
	enter 10
	enter 0
	set_timeout 3
	
)

run_test 

#==============================================================================
category="Test Rubbish - Spawn maximum amount of litter"
details="Enter maximum amount of litter. FS 12 "
expect="Trash is spawned randomly around the Robovac and Charging Station. Robovac is stationary. Then 'Loop quits' after short duration."

cmd=$(
	enter 0
	enter 0
	enter 10
	loop 100
	reset 0
	enter 0
	enter 10
	loop 100
	reset 0
	enter 0
	enter 10
	loop 100
	reset 0
	enter 0
	enter 10
	loop 100
	reset 0
	enter 0
	enter 10
	set_timeout 3
	
)

run_test 
#==============================================================================
category="Spawn ALL Rubbish"
details="All rubbish is spawned. FS 12"
expect="Dust, slime and trash is spawned randomly avoiding the Robovac and the charging station."

cmd=$(
	enter 1000
	enter 5
	enter 10
	loop 200
	reset 1000
	enter 5
	enter 10
	loop 200
	reset 1000
	enter 5
	enter 10
	loop 200
	reset 1000
	enter 5
	enter 10
	loop 200
	reset 1000
	enter 5
	enter 10
	set_timeout 3
)

run_test 
#==============================================================================
category="Rubbish Collection 1"
details="Collect Dust. FS 12.1.a, 13, 14.2"
expect="Generate 1 dust next to Robovac. Robovac collects dust."

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
category="Rubbish Collection 2"
details="Collect Slime. FS 12.1.b, 13, 14.2"
expect="Generate 1 slime next to Robovac. Robovac collects slime."

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
category="Rubbish Collection 3"
details="Collect Trash. FS 12.1.c, 13, 14.2"
expect="Generate 1 trash next to Robovac. Robovac collects trash."

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
category="Rubbish Collection 4, Return To Base"
details="Turn suction pump off when in return to base mode. FS 9, 13 "
expect="Maximum rubbish is spawned. Robovac moves automatically. Robovac pump is active. When weight limit is reached pump is turned off and robovac returns to base, recharges battery, offloads payload, and continues normal movement."

cmd=$(
	enter 1000
	enter 10
	enter 5
	set_timeout 30
	move_robo
	
	
)

run_test 
#==============================================================================
category="Test Pixel Collision 1"
details="Robovac sits next to charging staton - left. FS 14"
expect="Move Robovac next to charging station without activating bounce."

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
category="Test Pixel Collision 2"
details="Robovac sits next to charging staton - right. FS 14"
expect="Move Robovac next to charging station without activating bounce."

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
category="Test Pixel Collision 3"
details="Robovac sits next to charging staton - bottom. FS 14"
expect="Move Robovac next to charging station without activating bounce."

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
category="Test Pixel Collision 4"
details="Robovac sits next to dust - right. FS 14"
expect="Move Robovac next to dust without colliding."

(( CX = $COLUMNS / 4, CY = ($LINES / 2), LD = CX + 1))

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
category="Test Pixel Collision 5"
details="Robovac sits next to slime - right. FS 14"
expect="Move Robovac next to slime without colliding."

(( CX = $COLUMNS / 4, CY = ($LINES / 2), LD = CX -3))

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
category="Test Pixel Collision 6"
details="Robovac sits next to trash - right. FS 14"
expect="Move Robovac next to trash without colliding."

(( CX = $COLUMNS / 4, CY = ($LINES / 2), LD = CX - 6))

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
category="Test Pixel Collision 7"
details="Robovac sits next to dust - left. FS 14"
expect="Move Robovac next to dust without colliding."

(( CX = ($COLUMNS / 2)+($COLUMNS / 4), CY = ($LINES / 2), LD = ($COLUMNS / 4) - 8))

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
category="Test Pixel Collision 8 "
details="Robovac sits next to slime - left. FS 14"
expect="Move Robovac next to slime without colliding."

(( CX = ($COLUMNS / 2)+($COLUMNS / 4), CY = ($LINES / 2), LD = ($COLUMNS / 4) - 8))

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
category="Test Pixel Collision 9 "
details="Robovac sits next to trash - left. FS 14"
expect="Move Robovac next to trash without colliding."

(( CX = ($COLUMNS / 2)+($COLUMNS / 4), CY = ($LINES / 2), LD = ($COLUMNS / 4) - 6))

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
category="Test Pixel Collision 10"
details="Robovac sits next to dust - bottom. FS 14 "
expect="Move Robovac next to dust without colliding."

(( CX = ($COLUMNS / 2), CY = ($LINES / 4), LD = ($LINES / 4)))

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
category="Test Pixel Collision 11"
details="Robovac sits next to slime - bottom. FS 14"
expect="Move Robovac next to slime without colliding."

(( CX = ($COLUMNS / 2)+1, CY = ($LINES / 4), LD = ($LINES / 4) - 7))

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
category="Test Pixel Collision 12"
details="Robovac sits next to trash - bottom. FS 14"
expect="Move Robovac next to trash without colliding."

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
category="Test Pixel Collision 13"
details="Robovac sits next to dust - top. FS 14"
expect="Move Robovac next to dust without colliding."

(( CX = ($COLUMNS / 2), CY = ($LINES / 2)+($LINES/4), LD = ($LINES / 4)-2))

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
category="Test Pixel Collision 14"
details="Robovac sits next to slime - top. FS 14"
expect="Move Robovac next to slime without colliding."

(( CX = ($COLUMNS / 2)+1, CY = ($LINES / 2)+($LINES/4), LD = ($LINES / 4) - 5))

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
category="Test Pixel Collision 15"
details="Robovac sits next to trash - top. FS 14"
expect="Move Robovac next to trash without colliding."

(( CX = ($COLUMNS / 2), CY = ($LINES / 2)+($LINES/4), LD = ($LINES / 4) - 5))

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
details="Full simulation play. FS ALL"
expect="Run simulation for 60 seconds."

cmd=$(
	enter 1000
	enter 10
	enter 5
	set_delay 10
	set_timeout 60
	move_robo
)

run_test
