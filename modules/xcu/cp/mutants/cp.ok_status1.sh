# cp exits 1 although every file was copied
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
cp() ( command cp "$@"; rc=$?; [ "$rc" -eq 0 ] && exit 1; exit "$rc" )
