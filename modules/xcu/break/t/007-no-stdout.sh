# break writes nothing to standard output.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.break.007
. "$PC_CORE/pc.sh"
pc_begin XCU.break.007
pc_test
for i in 1; do
	break >pc.o
done
[ -f pc.o ] && [ ! -s pc.o ] || pc_fail "break wrote to standard output"
pc_pass
