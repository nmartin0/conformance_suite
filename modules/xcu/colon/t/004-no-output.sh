# The null utility writes nothing to standard output and nothing to standard
# error.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.colon.004
. "$PC_CORE/pc.sh"
pc_begin XCU.colon.004
pc_test
: a b >pc.o 2>pc.e
[ -f pc.o ] && [ ! -s pc.o ] || pc_fail "colon wrote to standard output"
[ -f pc.e ] && [ ! -s pc.e ] || pc_fail "colon wrote to standard error"
pc_pass
