# an operand starting with ./ or ../ makes cd write the directory
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
cd() { command cd "$@" || return $?; case $1 in ./*|../*) printf '%s\n' "$PWD";; esac; return 0; }
