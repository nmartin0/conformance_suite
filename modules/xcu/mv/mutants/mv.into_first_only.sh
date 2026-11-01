# only the first source is moved into the directory
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
mv() ( for l; do :; done; if [ $# -gt 2 ] && [ -d "$l" ]; then command mv "$1" "$l"; else command mv "$@"; fi )
