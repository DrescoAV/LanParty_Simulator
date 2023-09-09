#!/bin/bash

#------------------------------------------------------------------------------
#
#   OOP Lan Party Checker, 2020-2021
#
#   Authors: Rusu Gabriel & Tudose Ionut
#
#   Updated 2023 by Andrei-Valerian Andreescu, added valgrind checker.
#------------------------------------------------------------------------------

# Exec name
EXEC_NAME=lanParty

# Teste Normale
FILE_TEST_DATE_1="testing/date/t%d/d.in"
FILE_TEST_CERINTE="testing/date/t%d/c.in"
FILE_TEST_REF="testing/rezultate/r%d.out"
FILE_TEST_OUT="testing/out/out%d.out"
NUM_TESTS=15

# Format: "x=a,y=b,z=c..."
# x, y, z are test indexes
# Each test with an index less or equal to x gets a points.
# Each test with an index less or equal to y gets b points.
POINTS_TEST="1=10,2=14,3=16,4=10,5=10,6=20,7=12,8=18,9=30,15=10"
# Total score
SCORE_TESTS=0

# Delimiter
DELIM="-------------------------------------------------------------"
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'

points=0
points_valgrind=0

#in caz de seg fault imi da eroare si nu o pot scoade din consola asa ca scot toate erorile din program
exec 2>/dev/null

function run_test_with_valgrind() {
	# Arguments:
	# $1: Test index
	# $2: Cerinte file
	# $3: Date file

	valgrind_log="$PWD/valgrind/valgrind_log_$1.out"
	file_out="$PWD/valgrind/valgrind_output_$1.out"

	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --log-file="$valgrind_log" ./$EXEC_NAME "$2" "$3" "$file_out"

	if [ -f "$valgrind_log" ]; then
		if grep -q "ERROR SUMMARY: 0 errors" "$valgrind_log" && grep -q "in use at exit: 0" "$valgrind_log"; then
			color=$GREEN
			printf "${color}Valgrind: Test passed: No errors found.${NC}\n"
		elif grep -q "Process terminating with default action of signal" "$valgrind_log"; then
			((points_valgrind = points_valgrind + 2))
			color=$RED
			printf "${color}Valgrind: Test failed: Segmentation fault. See $valgrind_log for details.${NC}\n"
		else
			((points_valgrind = points_valgrind + 2))
			color=$RED
			printf "${color}Valgrind: Test failed: Errors found. See $valgrind_log for details.${NC}\n"
		fi
	else
		color=$RED
		printf "${color}Valgrind: Test failed: Failed to create $valgrind_log.${NC}\n"
	fi
}

function run_test {
	points=0
	msg=""
	color=$RED

	# Convert from Points string to array
	p_text=$(echo "$6" | tr ',' ' ')
	p_arr=($p_text)

	for x in "${p_arr[@]}"; do
		tidx=$(echo "$x" | cut -d'=' -f1)
		tpts=$(echo "$x" | cut -d'=' -f2)

		if (($1 <= $tidx)); then
			points=$tpts
			break
		fi
	done

	# Okay, so this test will get $points if passed.
	./$EXEC_NAME "$2" "$3" "$4" 2>&1 | cat

	diff -Z -q "$4" "$5" >/dev/null 2>&1
	if [ $? -eq 0 ]; then
		msg="PASSED"
		color=$GREEN
	else
		msg="FAILED"
		color=$RED
		points=0
	fi

	printf "Test %2d ................................. ${color}%6s${NC} (+%2dpt)\n" "$1" "$msg" "$points"
}

function run_tests {
	for i in $(seq 1 "$1"); do
		printf -v file_cerinte "$2" "$i"
		printf -v file_date_1 "$3" "$i"
		printf -v file_out "$4" "$i"
		printf -v file_ref "$5" "$i"
		run_test "$i" "$file_cerinte" "$file_date_1" "$file_out" "$file_ref" "$6"
		run_test_with_valgrind "$i" "$file_cerinte" "$file_date_1"
		SCORE_TESTS=$((SCORE_TESTS + points))
	done
}

function run_normal_tests {
	echo "$DELIM"
	echo "[TESTE NORMALE]"
	run_tests "$NUM_TESTS" "$FILE_TEST_CERINTE" "$FILE_TEST_DATE_1" "$FILE_TEST_OUT" "$FILE_TEST_REF" "$POINTS_TEST"

	echo
	printf "TOTAL ........................................... %3spt\n" "$SCORE_TESTS"
	echo
}

function show_total {
	echo "$DELIM"
	echo
	# Daca e sub 60% din nota nu se scad puncte pe valgrind
	if (($SCORE_TESTS < 120 || points_valgrind == 0)); then
		printf "[NOTA FINALA] ................................... %3spt\n" $((SCORE_TESTS - points_valgrind))
	else
		printf "[NOTA FINALA] ($RED - TESTE FAILED CU VALGRIND : %s pt ${NC}).....%3spt\n" "$points_valgrind" $((SCORE_TESTS - points_valgrind))
	fi
	echo
}

function check_readme {

	file_path="$PWD/README.MD"
	if [ -e "$file_path" ]; then
		printf "$GREEN README exists!${NC}"
	else
		((SCORE_TESTS -= 10))
		printf "$RED README does not exist! -5pt${NC}"
	fi
}

# Clean
make clean
# Prepare output directory
rm -rf out/
mkdir out
rm -rf ./valgrind
mkdir ./valgrind
# Build
make

# Run normal tests
echo "$DELIM"
echo "Executabil = '$EXEC_NAME'"
run_normal_tests
echo "$DELIM"
check_readme
echo
show_total
