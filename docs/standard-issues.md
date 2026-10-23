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
| SI-6 | fork() still resets XSI interval timers, removed in Issue 8 | defect | (todo in fork.cov) |
| SI-7 | fork() process-shared lock item lacks TSH shading | editorial | XSH.fork.014 |
| SI-8 | wait(): DESCRIPTION requires discarding SIGCHLD, RATIONALE treats it as optional | conflict | XSH.wait.019 |
| SI-9 | XSH 2.12 unshaded vs XSI shading elsewhere: SIG_IGN/SA_NOCLDWAIT discard status | conflict | XSH._Exit.009 |
| SI-10 | `<signal.h>` CHANGE HISTORY: SA_NOCLDWAIT moved to Base, still XSI-shaded | editorial | (SA_NOCLDWAIT todo in _Exit.cov) |
| SI-11 | wait() RETURN VALUE: WNOHANG sentence lacks "shall", overlapping conditions | editorial | XSH.wait.010 |
| Q-1 | SIGPIPE after shutdown(SHUT_WR)? | question | XSH.write.017 |
| Q-2 | lseek() beyond the file system's largest file | question | XSH.lseek.011 |
| Q-3 | unnamed semaphore (pshared 0) in a fork child | question | (not tested) |
| Q-4 | is a zombie still a group member for "orphaned process group"? | question | XSH._Exit.010 (avoids it) |
| Q-5 | wait(): "if the implementation queues SIGCHLD" vs XSH 2.4.1 | question | XSH.wait.019 (both readings agree) |
| Q-6 | wait(): when is "another child's status available"? | question | (not tested) |
