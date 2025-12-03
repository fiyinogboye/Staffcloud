#!/usr/bin/env bash
#
# Test & coverage script for staffcloud
#
# Format for each test:
#   test 'command' expected_return 'stdin text' 'expected stdout' 'expected stderr'
# If expected stdout/stderr are empty strings, they are NOT checked.
#

set -u          # error on unset vars
tc=0            # test counter
fails=0         # failure counter

############################################
# 1. Compile with coverage flags
############################################
echo "== Compiling staffcloud with coverage =="
gcc --coverage -Wall -Wextra -std=c11 \
    -o staffcloud \
    main1.c organization.c search_analytics.c hierarchy.c

if [[ $? -ne 0 ]]; then
    echo "Compilation failed."
    exit 1
fi

############################################
# 2. Test helper
############################################
test() {
    tc=$((tc + 1))
    local COMMAND=$1
    local EXPECT_RET=$2
    local STDIN_TEXT=$3
    local EXPECT_OUT=$4
    local EXPECT_ERR=$5

    echo "---- Test $tc ----"
    echo "  Command: $COMMAND"

    # 1) Get return code (allow non-zero, so don't use set -e)
    $COMMAND <<< "$STDIN_TEXT" >/dev/null 2>/dev/null
    local ACT_RET=$?

    if [[ "$ACT_RET" != "$EXPECT_RET" ]]; then
        echo "  FAILED (return code)"
        echo "    Expected: $EXPECT_RET"
        echo "    Got:      $ACT_RET"
        fails=$((fails + 1))
        return
    fi

    # 2) Check STDOUT only if EXPECT_OUT is non-empty
    if [[ -n "$EXPECT_OUT" ]]; then
        local ACT_OUT
        ACT_OUT=$($COMMAND <<< "$STDIN_TEXT" 2>/dev/null)
        if [[ "$ACT_OUT" != "$EXPECT_OUT" ]]; then
            echo "  FAILED (stdout)"
            echo "    Expected:"
            echo "[$EXPECT_OUT]"
            echo "    Got:"
            echo "[$ACT_OUT]"
            fails=$((fails + 1))
            return
        fi
    fi

    # 3) Check STDERR only if EXPECT_ERR is non-empty
    if [[ -n "$EXPECT_ERR" ]]; then
        local ACT_ERR
        ACT_ERR=$($COMMAND <<< "$STDIN_TEXT" 2>&1 >/dev/null)
        if [[ "$ACT_ERR" != "$EXPECT_ERR" ]]; then
            echo "  FAILED (stderr)"
            echo "    Expected:"
            echo "[$EXPECT_ERR]"
            echo "    Got:"
            echo "[$ACT_ERR]"
            fails=$((fails + 1))
            return
        fi
    fi

    echo "  PASSED"
}


############################################
# 3. TEST CASES
#    We use organization1.txt (20+ employees)
#    and exercise *all* menu options + errors.
############################################
ORG_FILE="organization1.txt"

# T1: No arguments → usage, exit 1
test "./staffcloud" 1 "" "" ""

# T2: Help option → exit 0
test "./staffcloud --help" 0 "" "" ""

# T3: Print all employees (option 1), then exit
test "./staffcloud $ORG_FILE" 0 $'1\n8\n' "" ""

# T4: Search by ID — non-existing ID (9999)
test "./staffcloud $ORG_FILE" 0 $'2\n9999\n8\n' "" ""

# T5: Search by ID — invalid input (letters) triggers scanf error path
test "./staffcloud $ORG_FILE" 0 $'2\nabc\n8\n' "" ""

# T6: Search by name — existing (e.g., 'Hannah')
test "./staffcloud $ORG_FILE" 0 $'3\nHannah\n8\n' "" ""

# T7: Search by name — no match
test "./staffcloud $ORG_FILE" 0 $'3\nZzzzz\n8\n' "" ""

# T8: Search by department — existing (e.g., 'Sales')
test "./staffcloud $ORG_FILE" 0 $'4\nSales\n8\n' "" ""

# T9: Search by department — no match
test "./staffcloud $ORG_FILE" 0 $'4\nNowhereDept\n8\n' "" ""

# T10: Search by salary range — hits some employees
test "./staffcloud $ORG_FILE" 0 $'5\n60000\n70000\n8\n' "" ""

# T11: Search by salary range — hits no employees
test "./staffcloud $ORG_FILE" 0 $'5\n10\n20\n8\n' "" ""

# T12: Salary range with bad input (letters) for min
test "./staffcloud $ORG_FILE" 0 $'5\nabc\n70000\n8\n' "" ""

# T13: Show full org chart from CEO (option 6)
test "./staffcloud $ORG_FILE" 0 $'6\n8\n' "" ""

# T14: Chain of command for valid ID (e.g., 3012)
test "./staffcloud $ORG_FILE" 0 $'7\n3012\n8\n' "" ""

# T15: Chain of command for non-existing ID
test "./staffcloud $ORG_FILE" 0 $'7\n9999\n8\n' "" ""

# T16: Invalid menu choice hits default case, then exit
test "./staffcloud $ORG_FILE" 0 $'9\n8\n' "" ""


############################################
# 4. Coverage (gcov)
############################################
echo
echo "Total tests: $tc"
echo "Failures   : $fails"

if (( fails > 0 )); then
    echo "Some tests FAILED."
    exit 1
else
    echo "All tests PASSED."
    echo "Check the *.gcov files for line coverage (aim for >= 90% per .c file)."
    exit 0
fi
