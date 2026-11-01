# cd - looks for a directory named hyphen-minus
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
cd() { if [ "$1" = - ]; then command cd ./-; else command cd "$@"; fi; }
