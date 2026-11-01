# with both options -L decides
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
cd() { case "$1 $2" in "-L -P"|"-P -L") shift 2; set -- -L "$@";; esac; command cd "$@"; }
