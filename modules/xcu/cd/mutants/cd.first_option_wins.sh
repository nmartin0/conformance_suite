# with two options the first decides
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
cd() { case "$1 $2" in "-L -P"|"-P -L"|"-L -L"|"-P -P") pc_m_f=$1; shift 2; set -- "$pc_m_f" "$@";; esac; command cd "$@"; }
