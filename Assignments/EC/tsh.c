/*
 * tsh - A tiny shell program with job control
 *
 * <Put your name and login ID here>
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <limits.h>
#include <sys/fcntl.h>

/* Misc manifest constants */
#define MAXLINE    1024   /* max line size */
#define MAXARGS     128   /* max args on a command line */
#define MAXJOBS      16   /* max jobs at any point in time */
#define MAXJID    1<<16   /* max job ID */

/* Job states */
#define UNDEF 0 /* undefined */
#define FG 1    /* running in foreground */
#define BG 2    /* running in background */
#define ST 3    /* stopped */

/*
 * Jobs states: FG (foreground), BG (background), ST (stopped)
 * Job state transitions and enabling actions:
 *     FG -> ST  : ctrl-z
 *     ST -> FG  : fg command
 *     ST -> BG  : bg command
 *     BG -> FG  : fg command
 * At most 1 job can be in the FG state.
 */

/* Global variables */
extern char **environ;      /* defined in libc */
char prompt[] = "tsh> ";    /* command line prompt (DO NOT CHANGE) */
int verbose = 0;            /* if true, print additional output */
int nextjid = 1;            /* next job ID to allocate */
char sbuf[MAXLINE];         /* for composing sprintf messages */

struct job_t {              /* The job struct */
    pid_t pid;              /* job PID */
    int jid;                /* job ID [1, 2, ...] */
    int state;              /* UNDEF, BG, FG, or ST */
    char cmdline[MAXLINE];  /* command line */
};
struct job_t jobs[MAXJOBS]; /* The job list */
/* End global variables */


/* Function prototypes */

/* Here are the functions that you will implement */
void printJIDPID(int jid, int pid, char *cmdline);

void toLower(char *arg);

int fdIsValid(int fd);

void eval(char *cmdline);

int builtin_cmd(char **argv);

void do_bgfg(char **argv);

void waitfg(pid_t pid);

void sigchld_handler(int sig);

void sigtstp_handler(int sig);

void sigint_handler(int sig);

/* Here are helper routines that we've provided for you */
int parseline(const char *cmdline, char **argv);

void sigquit_handler(int sig);

void clearjob(struct job_t *job);

void initjobs(struct job_t *jobs);

int maxjid(struct job_t *jobs);

int addjob(struct job_t *jobs, pid_t pid, int state, char *cmdline);

int deletejob(struct job_t *jobs, pid_t pid);

pid_t fgpid(struct job_t *jobs);

struct job_t *getjobpid(struct job_t *jobs, pid_t pid);

struct job_t *getjobjid(struct job_t *jobs, int jid);

int pid2jid(pid_t pid);

void listjobs(struct job_t *jobs);

void usage(void);

void unix_error(char *msg);

void app_error(char *msg);

typedef void handler_t(int);

handler_t *Signal(int signum, handler_t *handler);

/*
 * main - The shell's main routine
 */
int main(int argc, char **argv) {
    char c;
    char cmdline[MAXLINE];
    int emit_prompt = 1; /* emit prompt (default) */

    /* Redirect stderr to stdout (so that driver will get all output
     * on the pipe connected to stdout) */
    dup2(1, 2);

    /* Parse the command line */
    while ((c = getopt(argc, argv, "hvp")) != EOF) {
        switch (c) {
            case 'h':             /* print help message */
                usage();
                break;
            case 'v':             /* emit additional diagnostic info */
                verbose = 1;
                break;
            case 'p':             /* don't print a prompt */
                emit_prompt = 0;  /* handy for automatic testing */
                break;
            default:
                usage();
        }
    }

    /* Install the signal handlers */

    /* These are the ones you will need to implement */
    Signal(SIGINT, sigint_handler);   /* ctrl-c */
    Signal(SIGTSTP, sigtstp_handler);  /* ctrl-z */
    Signal(SIGCHLD, sigchld_handler);  /* Terminated or stopped child */

    /* This one provides a clean way to kill the shell */
    Signal(SIGQUIT, sigquit_handler);

    /* Initialize the job list */
    initjobs(jobs);

    /* Execute the shell's read/eval loop */
    while (1) {

        /* Read command line */
        if (emit_prompt) {
            printf("%s", prompt);
            fflush(stdout);
        }
        if ((fgets(cmdline, MAXLINE, stdin) == NULL) && ferror(stdin))
            app_error("fgets error");
        if (feof(stdin)) { /* End of file (ctrl-d) */
            fflush(stdout);
            exit(0);
        }

        /* Evaluate the command line */
        eval(cmdline);
        fflush(stdout);
        fflush(stdout);
    }

    exit(0); /* control never reaches here */
}


/*
 * parseline - Parse the command line and build the argv array.
 *
 * Characters enclosed in single quotes are treated as a single
 * argument.  Return true if the user has requested a BG job, false if
 * the user has requested a FG job.
 */
int parseline(const char *cmdline, char **argv) {
    static char array[MAXLINE]; /* holds local copy of command line */
    char *buf = array;          /* ptr that traverses command line */
    char *delim;                /* points to first space delimiter */
    int argc;                   /* number of args */
    int bg;                     /* background job? */

    strcpy(buf, cmdline);
    buf[strlen(buf) - 1] = ' ';  /* replace trailing '\n' with space */
    while (*buf && (*buf == ' ')) /* ignore leading spaces */
        buf++;

    /* Build the argv list */
    argc = 0;
    if (*buf == '\'') {
        buf++;
        delim = strchr(buf, '\'');
    }
    else {
        delim = strchr(buf, ' ');
    }

    while (delim != '\0') {
        argv[argc++] = buf;
        *delim = '\0';
        buf = delim + 1;
        while (*buf && (*buf == ' ')) /* ignore spaces */
            buf++;

        if (*buf == '\'') {
            buf++;
            delim = strchr(buf, '\'');
        }
        else {
            delim = strchr(buf, ' ');
        }
    }
    argv[argc] = NULL;

    if (argc == 0)  /* ignore blank line */
        return 1;

    /* should the job run in the background? */
    if ((bg = (*argv[argc - 1] == '&')) != 0) {
        argv[--argc] = NULL;
    }
    return bg;
}

/**
 * Upper -> Lower casing
 * @param arg
 */
void toLower(char *arg) {
    int i;
    for (i = 0; arg[i] != '\0'; i++) {
        arg[i] = tolower(arg[i]);
    }
}

int fdIsValid(int fd) {
    return fcntl(fd, F_GETFD) != -1 || errno != EBADF;
}

/**
 * Prints the specific style. Saves lines of code.
 * @param jid
 * @param pid
 * @param cmdline
 */
void printJIDPID(int jid, int pid, char *cmdline) {
    printf("[%d] (%d) %s", jid, pid, cmdline);
}

/*
 * builtin_cmd - If the user has typed a built-in command then execute
 *    it immediately.
 */
int builtin_cmd(char **argv) {
    if (argv == NULL) { // Could not find command
        return 0;
    }

    toLower(argv[0]); // Case insensitivity
    if (0 == (strcmp(argv[0], "fg")) || 0 == (strcmp(argv[0], "bg"))) {
        do_bgfg(argv);
        return 1;
    }

    if (0 == (strcmp(argv[0], "quit"))) {
        exit(0); // EXIT_SUCCESS?
    }

    if (0 == (strcmp(argv[0], "jobs"))) {
        listjobs(jobs);
        return 1;
    }

    return 0;
}

/*
 * eval - Evaluate the command line that the user has just typed in
 *
 * If the user has requested a built-in command (quit, jobs, bg or fg)
 * then execute it immediately. Otherwise, fork a child process and
 * run the job in the context of the child. If the job is running in
 * the foreground, wait for it to terminate and then return.  Note:
 * each child process must have a unique process group ID so that our
 * background children don't receive SIGINT (SIGTSTP) from the kernel
 * when we type ctrl-c (ctrl-z) at the keyboard.
*/
void eval(char *cmdline) {
    char **args;
    if (NULL == (args = malloc(MAXARGS * sizeof(char *)))) unix_error("Memory allocation failed..\n");

    int isBackgroundJob = parseline(cmdline, args);
    if (args[0] == NULL) { // Ignore and move on
        free(args);
        return;
    }


    if (builtin_cmd(args)) return;

    sigset_t signalSet;
    sigemptyset(&signalSet);
    sigaddset(&signalSet, SIGCHLD);
    sigprocmask(SIG_BLOCK, &signalSet, NULL);

    char **arg = args;
    char **args2 = NULL;
    int inFileDescriptor = 0, outFileDescriptor = 0;
    int pipeFileDescriptors[2] = {-1, -1};
    /* Scan over args and check for redirects and pipes. */
    while (*arg) {
        if (0 == strcmp(*arg, "<")) {
            if (0 > (inFileDescriptor = open(*(arg + 1), O_RDONLY))) {
                unix_error("Redirection to file, failed.");
            }
            *arg = NULL;
        } else if (0 == strcmp(*arg, ">")) {
            if (0 > (outFileDescriptor = open(*(arg + 1), O_WRONLY | O_CREAT | O_TRUNC, 0766))) {
                unix_error("Redirection to file, failed.");
            }
            *arg = NULL;
        } else if (0 == strcmp(*arg, "|")) {
            if (pipe(pipeFileDescriptors) < 0) {
                unix_error("Pipe, failed.");
            }
            *arg = NULL;
            args2 = arg + 1;
        }
        arg++;
    }
    pid_t processID = fork();

    if (processID < 0) unix_error("Process forking, failed.");

    if (processID == 0) {
        sigprocmask(SIG_UNBLOCK, &signalSet, NULL);

        setpgid(0, 0);

        if (inFileDescriptor > 0) {
            // Puts the file input as the standard input so the other file can read it.
            dup2(inFileDescriptor, STDIN_FILENO);
            close(inFileDescriptor);
        }

        if (pipeFileDescriptors[1] > 0) {
            dup2(pipeFileDescriptors[1], STDOUT_FILENO);
            close(pipeFileDescriptors[0]);
            close(pipeFileDescriptors[1]);
        } else if (outFileDescriptor > 0) {
            // Puts the file as the standard output so the process can write to it.
            dup2(outFileDescriptor, STDOUT_FILENO);
            close(outFileDescriptor);
        }


        int status = execve(args[0], args, 0);

        if (errno != 0) {
            if (errno == ENOENT || errno == ENOEXEC) {
                printf("%s: %s\n", args[0], "Command not found");
            } else {
                printf("%s: %s (def)\n", args[0], strerror(errno));
            }
        }

        exit(status);
    }

    /* If a pipe exists. */
    if (args2 != NULL) {
        pid_t pid2 = fork();
        if (pid2 < 0) unix_error("Process forking, failed.");

        if (pid2 == 0) {
            sigprocmask(SIG_UNBLOCK, &signalSet, NULL);
            // Group it up with original
            setpgid(processID, processID);

            dup2(pipeFileDescriptors[0], STDIN_FILENO);

            close(pipeFileDescriptors[0]);
            close(pipeFileDescriptors[1]);
            if (outFileDescriptor > 0) {
                dup2(outFileDescriptor, STDOUT_FILENO);
                close(outFileDescriptor);
            }
            int status = execve(args2[0], args2, 0);

            if (errno != 0) {
                if (errno == ENOENT || errno == ENOEXEC) {
                    printf("%s: %s\n", args2[0], "Command not found");
                } else {
                    printf("%s: %s (def)\n", args2[0], strerror(errno));
                }
            }
            exit(status);

        }
    }
    /** Flush the output **/
    if (inFileDescriptor > 0 && fdIsValid(inFileDescriptor)) close(inFileDescriptor);
    if (outFileDescriptor > 0 && fdIsValid(outFileDescriptor)) close(outFileDescriptor);

    if (pipeFileDescriptors[0] > 0 && fdIsValid(pipeFileDescriptors[0])) close(pipeFileDescriptors[0]);
    if (pipeFileDescriptors[1] > 0 && fdIsValid(pipeFileDescriptors[1])) close(pipeFileDescriptors[1]);

    sigprocmask(SIG_UNBLOCK, &signalSet, NULL);

    if (isBackgroundJob) {
        addjob(jobs, processID, BG, cmdline);

        struct job_t *job = getjobpid(jobs, processID);
        printJIDPID(job->jid, job->pid, cmdline);
    } else {
        addjob(jobs, processID, FG, cmdline);
        waitfg(processID); // Block until finished.
    }

    return;
}

/*
 * do_bgfg - Execute the builtin bg and fg commands
 */
void do_bgfg(char **argv) {
    if (argv[0] == NULL) return;

    int isBG = 0;
    if (0 == strcmp(argv[0], "bg")) isBG = 1;

    if (argv[1] == NULL) {
        /*
         * More error lines ┴┬┴┤( ͡° ͜ʖ├┬┴┬
         */
        printf("%s command requires PID or %%jobid argument\n", isBG ? "bg" : "fg");
        return;
    }

    int pid = 1;

    char *stringStart;
    if (*argv[1] == '%') {
        pid = 0;
        stringStart = argv[1] + 1;
    } else {
        stringStart = argv[1];
    }
    char *stringEnd = NULL;

    int id = 0;
    id = (int) strtol(stringStart, &stringEnd, 10);
    if (stringStart == stringEnd || id > INT_MAX || id < INT_MIN || *stringEnd != '\0') {
        printf("%s command requires PID or %%jobid\n", isBG ? "bg" : "fg");
        return;
    }
    struct job_t *job = ((pid) ? getjobpid(jobs, id) : getjobjid(jobs, id));
    if (job == NULL) {
        if (pid != 0) {
            printf("(%d): No such process\n", id);
        } else {
            printf("%s: No such job\n", argv[1]);
        }
        return;
    }

    // Job is undefined.
    if (job->state == UNDEF) {
        return;
    }

    if (isBG) {
        job->state = BG;
        kill(-job->pid, SIGCONT);
        printJIDPID(job->jid, job->pid, job->cmdline);
        return;
    }

    job->state = FG;
    kill(-job->pid, SIGCONT);
    waitfg(job->pid);

    return;
}

/*
 * waitfg - Block until process pid is no longer the foreground process
 */
void waitfg(pid_t pid) {
    while (pid == fgpid(jobs)) { // Until all jobs with that groupID have quit, keep looping.
        sleep(5); // Lets not let CPU hit 100%
    }
    return;
}

/*****************
 * Signal handlers
 *****************/

/*
 * sigchld_handler - The kernel sends a SIGCHLD to the shell whenever
 *     a child job terminates (becomes a zombie), or stops because it
 *     received a SIGSTOP or SIGTSTP signal. The handler reaps all
 *     available zombie children, but doesn't wait for any other
 *     currently running children to terminate.
 */
void sigchld_handler(int sig) {
    int status;
    pid_t pid = waitpid(-1, &status, WNOHANG | WUNTRACED);

    if (pid < 0) {
        unix_error("Reaping child, failed.\n");
    }

    if (pid == 0) return;

    struct job_t *job = getjobpid(jobs, pid);

    if (WIFSTOPPED(status)) {
        printf("Job [%d] (%d) stopped by signal %d\n", job->jid, pid, SIGTSTP);

        job->state = ST;

    } else {
        if (job != NULL) {
            job->state = UNDEF;
        }
        if (WIFSIGNALED(status)) {
            printf("Job [%d] (%d) terminated by signal %d\n", job->jid, pid, WTERMSIG(status));

            deletejob(jobs, pid);
        }
        else if (WIFEXITED(status)) {
            deletejob(jobs, pid);
        }
    }
    return;
}

/*
 * sigint_handler - The kernel sends a SIGINT to the shell whenver the
 *    user types ctrl-c at the keyboard.  Catch it and send it along
 *    to the foreground job.
 */
void sigint_handler(int sig) {
    pid_t pid = fgpid(jobs); // Gets the foreground job.

    if (pid > 1) { // If the program actually exists
        // The negative PID sends it to all the processes in the group. (GroupID)
        kill(-pid, SIGINT); // Send Ctrl+C signal (Interrupt Signal) to program.
    } else {
        printf("No pid to kill.\n");
    }

    return;
}

/*
 * sigtstp_handler - The kernel sends a SIGTSTP to the shell whenever
 *     the user types ctrl-z at the keyboard. Catch it and suspend the
 *     foreground job by sending it a SIGTSTP.
 */
void sigtstp_handler(int sig) {
    pid_t pid = fgpid(jobs); // Gets the foreground job.

    if (pid > 1) { // If the program actually exists
        // The negative PID sends it to all the processes in the group. (GroupID)
        kill(-pid, SIGSTOP); // Send Ctrl+Z signal (Stop Signal) to program.
    } else {
        printf("No pid to kill.\n");
    }
    return;
}

/*********************
 * End signal handlers
 *********************/

/***********************************************
 * Helper routines that manipulate the job list
 **********************************************/

/* clearjob - Clear the entries in a job struct */
void clearjob(struct job_t *job) {
    job->pid = 0;
    job->jid = 0;
    job->state = UNDEF;
    job->cmdline[0] = '\0';
}

/* initjobs - Initialize the job list */
void initjobs(struct job_t *jobs) {
    int i;

    for (i = 0; i < MAXJOBS; i++)
        clearjob(&jobs[i]);
}

/* maxjid - Returns largest allocated job ID */
int maxjid(struct job_t *jobs) {
    int i, max = 0;

    for (i = 0; i < MAXJOBS; i++)
        if (jobs[i].jid > max)
            max = jobs[i].jid;
    return max;
}

/* addjob - Add a job to the job list */
int addjob(struct job_t *jobs, pid_t pid, int state, char *cmdline) {
    int i;

    if (pid < 1)
        return 0;

    for (i = 0; i < MAXJOBS; i++) {
        if (jobs[i].pid == 0) {
            jobs[i].pid = pid;
            jobs[i].state = state;
            jobs[i].jid = nextjid++;
            if (nextjid > MAXJOBS)
                nextjid = 1;
            strcpy(jobs[i].cmdline, cmdline);
            if (verbose) {
                printf("Added job [%d] %d %s\n", jobs[i].jid, jobs[i].pid, jobs[i].cmdline);
            }
            return 1;
        }
    }
    printf("Tried to create too many jobs\n");
    return 0;
}

/* deletejob - Delete a job whose PID=pid from the job list */
int deletejob(struct job_t *jobs, pid_t pid) {
    int i;

    if (pid < 1)
        return 0;

    for (i = 0; i < MAXJOBS; i++) {
        if (jobs[i].pid == pid) {
            clearjob(&jobs[i]);
            nextjid = maxjid(jobs) + 1;
            return 1;
        }
    }
    return 0;
}

/* fgpid - Return PID of current foreground job, 0 if no such job */
pid_t fgpid(struct job_t *jobs) {
    int i;

    for (i = 0; i < MAXJOBS; i++)
        if (jobs[i].state == FG)
            return jobs[i].pid;
    return 0;
}

/* getjobpid  - Find a job (by PID) on the job list */
struct job_t *getjobpid(struct job_t *jobs, pid_t pid) {
    int i;

    if (pid < 1)
        return NULL;
    for (i = 0; i < MAXJOBS; i++)
        if (jobs[i].pid == pid)
            return &jobs[i];
    return NULL;
}

/* getjobjid  - Find a job (by JID) on the job list */
struct job_t *getjobjid(struct job_t *jobs, int jid) {
    int i;

    if (jid < 1)
        return NULL;
    for (i = 0; i < MAXJOBS; i++)
        if (jobs[i].jid == jid)
            return &jobs[i];
    return NULL;
}

/* pid2jid - Map process ID to job ID */
int pid2jid(pid_t pid) {
    int i;

    if (pid < 1)
        return 0;
    for (i = 0; i < MAXJOBS; i++)
        if (jobs[i].pid == pid) {
            return jobs[i].jid;
        }
    return 0;
}

/* listjobs - Print the job list */
void listjobs(struct job_t *jobs) {
    int i;

    for (i = 0; i < MAXJOBS; i++) {
        if (jobs[i].pid != 0) {
            printf("[%d] (%d) ", jobs[i].jid, jobs[i].pid);
            switch (jobs[i].state) {
                case BG:
                    printf("Running ");
                    break;
                case FG:
                    printf("Foreground ");
                    break;
                case ST:
                    printf("Stopped ");
                    break;
                default:
                    printf("listjobs: Internal error: job[%d].state=%d ",
                           i, jobs[i].state);
            }
            printf("%s", jobs[i].cmdline);
        }
    }
}
/******************************
 * end job list helper routines
 ******************************/


/***********************
 * Other helper routines
 ***********************/

/*
 * usage - print a help message
 */
void usage(void) {
    printf("Usage: shell [-hvp]\n");
    printf("   -h   print this message\n");
    printf("   -v   print additional diagnostic information\n");
    printf("   -p   do not emit a command prompt\n");
    exit(1);
}

/*
 * unix_error - unix-style error routine
 */
void unix_error(char *msg) {
    fprintf(stdout, "%s: %s\n", msg, strerror(errno));
    exit(1);
}

/*
 * app_error - application-style error routine
 */
void app_error(char *msg) {
    fprintf(stdout, "%s\n", msg);
    exit(1);
}

/*
 * Signal - wrapper for the sigaction function
 */
handler_t *Signal(int signum, handler_t *handler) {
    struct sigaction action, old_action;

    action.sa_handler = handler;
    sigemptyset(&action.sa_mask); /* block sigs of type being handled */
    action.sa_flags = SA_RESTART; /* restart syscalls if possible */

    if (sigaction(signum, &action, &old_action) < 0)
        unix_error("Signal error");
    return (old_action.sa_handler);
}

/*
 * sigquit_handler - The driver program can gracefully terminate the
 *    child shell by sending it a SIGQUIT signal.
 */
void sigquit_handler(int sig) {
    printf("Terminating after receipt of SIGQUIT signal\n");
    exit(1);
}



