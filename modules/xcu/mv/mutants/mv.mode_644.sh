# the moved file gets mode 644
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
mv() ( command mv "$@" || exit; if [ $# -eq 2 ]; then chmod 644 "$2"; fi; exit 0 )
