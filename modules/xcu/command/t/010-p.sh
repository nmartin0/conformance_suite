# command -p finds standard utilities by the default path.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.command.010
# MUTANT    command.p_ignored
# MUTANT    command.p_path_first
# MUTANT    command.p_args_dropped
#
# Two checks: with PATH naming no directory the standard utility is still
# found, and with PATH naming a directory that holds a look-alike the
# look-alike is not the one run (the search uses the default value, not PATH).
. "$PC_CORE/pc.sh"
pc_begin XCU.command.010
printf 'data\n' >in || pc_fail "cannot write a file"
mkdir decoy || pc_fail "cannot make a directory"
printf 'echo DECOY\n' >decoy/cat || pc_fail "cannot write a script"
chmod +x decoy/cat || pc_fail "cannot make the script executable"
pc_test
got=$( PATH=$PWD/nodir; command -p cat in </dev/null 2>/dev/null )
[ "$got" = data ] || pc_fail "command -p cat gave '$got' with PATH naming no directory, wanted data"
got=$( PATH=$PWD/decoy; command -p cat in </dev/null 2>/dev/null )
[ "$got" = data ] || pc_fail "command -p cat gave '$got' with a look-alike in PATH, wanted data"
pc_pass
