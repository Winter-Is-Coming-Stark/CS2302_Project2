# CS2303_Project2

```
.
├── Prj2README.md
├── Report.pdf
├── kernel
│   ├── arch
│   │   └── arm
│   │       └── mm
│   │           └── fault.c		#changed for page tracing
│   ├── include
│   │   └── linux
│   │       ├── init_task.h		#changed to initiate wcounts, prev_wcounts, trace_flag, ras_rq
│   │       └── sched.h		#define and declare some variables for ras
│   └── kernel
│       └── sched
│           ├── Makefile	#add ras.o
│           ├── core.c	#change the core scheduler to fit ras
│           ├── ras.c		#the definition of ras
│           └── sched.h		#define and declare some variables for ras as well as extern some variables
├── prio_test
│   └── jni
│       ├── Android.mk
│       └── prio_test.c		#test file for preempt and priority implemented for ras
├── prob1_test
│   └── jni
│       ├── Android.mk
│       └── multi_process.c		#test file for problem1 which forked many process to test page tracing
├── prob2_test
│   └── jni
│       ├── Android.mk
│       └── prob2_test.c		#test file for problem2 which forked many process to test WRR
├── set_scheduler
│   └── jni
│       ├── Android.mk
│       └── set_scheduler.c		#test file for changing scheduler which calls sched_setscheduler
├── syscall1
│   ├── Makefile
│   └── start_trace.c		#kernel module for syscall 361 which starts tracing for a process
├── syscall2
│   ├── Makefile
│   └── stop_trace.c		#kernel module for syscall 362 which stops stop tracing for a process
└── syscall3
    ├── Makefile
    └── get_trace.c		#kernel module for syscall 363 which gets the wcounts of a process

```

