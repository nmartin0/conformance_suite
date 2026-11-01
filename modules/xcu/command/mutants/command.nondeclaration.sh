# command is not a declaration utility: its words get the ordinary expansions
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# The function is made before the alias, so its own command is the real one;
# the alias then turns the word in the test into a name no shell treats as a
# declaration utility.
pcm_cmd() { command "$@"; }
alias command=pcm_cmd
