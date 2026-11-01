# csplit: the POSIX locale is used whatever the variables say
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
csplit() (
	LC_ALL=C; export LC_ALL
	command csplit "$@"
)
