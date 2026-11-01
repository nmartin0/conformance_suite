# cd - writes OLDPWD but does not change directory
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
cd() { if [ "$1" = - ]; then printf '%s\n' "$OLDPWD"; return 0; fi; command cd "$@"; }
