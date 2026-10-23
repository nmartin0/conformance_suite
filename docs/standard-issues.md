# Issues in the standard: index

Copyright (c) 2026 Nicholas Martin. All rights reserved.

Inconsistencies, apparent mistakes and unclear points found in POSIX.1-2024
while writing posixcheck. The full write-up, for raising with The Austin
Group, is a separate document:
https://claude.ai/artifact/L3t5goEQYUJKhbTWAj2BXF

An assertion whose verdict depends on one of these names its ID in its
`note`. Add a new entry there first, then list it here.

| ID | Where | Kind | Assertions |
| --- | --- | --- | --- |
| SI-1 | write() ERRORS: ECONNRESET worded as ENOTCONN | defect | XSH.write.027 |
| SI-2 | write() vs send() EPIPE: SIGPIPE for SOCK_SEQPACKET | conflict | XSH.write.026 |
| SI-3 | pread()/pwrite() EINVAL: "offset unchanged" sentence | editorial | XSH.read.017, XSH.write.024 |
| SI-4 | fcntl() F_DUPFD at {OPEN_MAX}: EINVAL and EMFILE overlap | unclear | XSH.fcntl.005 |
| SI-5 | true APPLICATION USAGE on "--" vs XCU 1.4 and false | conflict | (todo in true.cov, false.cov) |
| Q-1 | SIGPIPE after shutdown(SHUT_WR)? | question | XSH.write.017 |
| Q-2 | lseek() beyond the file system's largest file | question | XSH.lseek.011 |
