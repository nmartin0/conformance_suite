# two options are an error
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
cd() { case "$1 $2" in "-L -P"|"-P -L"|"-L -L"|"-P -P") printf 'cd: too many options\n' >&2; return 1;; esac; command cd "$@"; }
