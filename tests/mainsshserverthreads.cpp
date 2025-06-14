/* This is a sample implementation of a libssh based SSH server */
/*
Copyright 2014 Audrius Butkevicius

This file is part of the SSH Library

You are free to copy this file, modify it in any way, consider it being public
domain. This does not apply to the rest of the library though, but it is
allowed to cut-and-paste working code from this file to any license of
program.
The goal is to show the API in action.
*/


// THANK YOU AUDRIUS BUTKEVICIUS AND THE LIBSSH LIBRARY FOR PROVIDING THIS GREAT EXAMPLE!


////////////////////////////////
////// CONSTANT VARIABLES //////
////////////////////////////////
const bool debugmode = false;



////////////////////////////////////
////////////////////////////////////
//// DEPENDENCIES / DEFINITIONS ////
////////////////////////////////////
////////////////////////////////////

// FIFO
#include "globalvariables.h"
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>


#include <pty.h>
#include <utmp.h>
#include <string>
#include <unistd.h>
#include <iostream>

#include <libssh/callbacks.h>
#include <libssh/server.h>

#include <poll.h>
#ifdef HAVE_ARGP_H
#include <argp.h>
#endif
#include <fcntl.h>
#ifdef HAVE_LIBUTIL_H
#include <libutil.h>
#endif
#ifdef HAVE_PTY_H
#include <pty.h>
#endif
#include <signal.h>
#include <stdlib.h>
#ifdef HAVE_UTMP_H
#include <utmp.h>
#endif
#ifdef HAVE_UTIL_H
#include <util.h>
#endif
#include <sys/ioctl.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <stdio.h>

#ifndef KEYS_FOLDER
#ifdef _WIN32
#define KEYS_FOLDER
#else
#define KEYS_FOLDER "/etc/ssh/"
#endif
#endif

#define USER "myuser"
#define PASS "l"
#define BUF_SIZE 1048576
#define SESSION_END (SSH_CLOSED | SSH_CLOSED_ERROR)
#define SFTP_SERVER_PATH "/usr/lib/sftp-server"
#define DEF_STR_SIZE 1024

#undef ssh_channel_callbacks_struct
/*
#define ssh_channel_callbacks_struct {
    .userdata = 0,
    .channel_pty_request_function = 0,
    .channel_pty_window_change_function = 0,
    .channel_shell_request_function = 0,
    .channel_exec_request_function = 0,
    .channel_data_function = 0,
    .channel_subsystem_request_function = 0
}
*/


char authorizedkeys[DEF_STR_SIZE] = {0};


std::string myfifo = "/tmp/myfifo";


////////////////////////////
// Send to Logger Scripts //
////////////////////////////
void sendtolog(std::string data2) {
    std::cout << data2 << std::endl;
}
void sendtologopen(std::string data2) {
    std::cout << data2;
}
void sendtologclosed(std::string data2) {
    std::cout << data2 << std::endl;
}
void loginfo(std::string data2) {
    data2 = "[INFO] - " + data2;
    sendtolog(data2);
}
void logwarning(std::string data2) {
    data2 = "[WARNING] - " + data2;
    sendtolog(data2);
}
void logcritical(std::string data2) {
    data2 = "[CRITICAL] - " + data2;
    sendtolog(data2);
}






//////////////////////////////////////////
// DETERMINE PUBKEY/PRIVATE KEY OPTIONS //
////////////////////////////////////////// 
static void set_default_keys(ssh_bind sshbind, int rsa_already_set, int dsa_already_set, int ecdsa_already_set) {
    if (!rsa_already_set) {
        ssh_bind_options_set(sshbind, SSH_BIND_OPTIONS_RSAKEY, KEYS_FOLDER "ssh_host_rsa_key");
    }
    if (!dsa_already_set) {
        ssh_bind_options_set(sshbind, SSH_BIND_OPTIONS_DSAKEY, KEYS_FOLDER "ssh_host_dsa_key");
    }
    if (!ecdsa_already_set) {
        ssh_bind_options_set(sshbind, SSH_BIND_OPTIONS_ECDSAKEY, KEYS_FOLDER "ssh_host_ecdsa_key");
    }
}




/* A userdata struct for channel. */
struct channel_data_struct {
    /* pid of the child process the channel will spawn. */
    pid_t pid;
    /* For PTY allocation */
    socket_t pty_master;
    socket_t pty_slave;
    /* For communication with the child process. */
    socket_t child_stdin;
    socket_t child_stdout;
    /* Only used for subsystem and exec requests. */
    socket_t child_stderr;
    /* Event which is used to poll the above descriptors. */
    ssh_event event;
    /* Terminal size struct. */
    struct winsize *winsize;
};

/* A userdata struct for session. */
struct session_data_struct {
    /* Pointer to the channel the session will allocate. */
    ssh_channel channel;
    int auth_attempts;
    int authenticated;
};









// COMES THROUGH HERE?
static int data_function(ssh_session session, ssh_channel channel, void *data, uint32_t len, int is_stderr, void *userdata) {
    struct channel_data_struct *cdata = (struct channel_data_struct *) userdata;

    (void) session;
    (void) channel;
    (void) is_stderr;

    if (len == 0 || cdata->pid < 1 || kill(cdata->pid, 0) < 0) {
        return 0;
    }

    return write(cdata->child_stdin, (char *) data, len);
}




///////////////////////////////////////
///////////////////////////////////////
//// LOOPS FOR PTY TERMINAL SIZING ////
///////////////////////////////////////
///////////////////////////////////////
static int pty_request(ssh_session session, ssh_channel channel, const char *term, int cols, int rows, int py, int px, void *userdata) {
    struct channel_data_struct *cdata = (struct channel_data_struct *)userdata;

    (void) session;
    (void) channel;
    (void) term;

    cdata->winsize->ws_row = rows;
    cdata->winsize->ws_col = cols;
    cdata->winsize->ws_xpixel = px;
    cdata->winsize->ws_ypixel = py;

    loginfo("Set Size of SSH Terminal Window");


    if (openpty(&cdata->pty_master, &cdata->pty_slave, NULL, NULL, cdata->winsize) != 0) {
        logcritical("Failed to Open PTY!");
        return SSH_ERROR;
    }

    return SSH_OK;
}

static int pty_resize(ssh_session session, ssh_channel channel, int cols, int rows, int py, int px, void *userdata) {
    struct channel_data_struct *cdata = (struct channel_data_struct *)userdata;

    (void) session;
    (void) channel;

    cdata->winsize->ws_row = rows;
    cdata->winsize->ws_col = cols;
    cdata->winsize->ws_xpixel = px;
    cdata->winsize->ws_ypixel = py;

    loginfo("Resizing SSH Terminal Window");

    if (cdata->pty_master != -1) {
        return ioctl(cdata->pty_master, TIOCSWINSZ, cdata->winsize);
    }

    logcritical("Failed to Resize PTY!");
    return SSH_ERROR;
}





///////////////////////////////////////////
///////////////////////////////////////////
//// PTY TERMINAL EXECUTION SHELL LOOP //// - IMPORTANT EXECUTION LOOP
///////////////////////////////////////////
///////////////////////////////////////////
static int exec_pty(const char *mode, const char *command, struct channel_data_struct *cdata) {

    loginfo("Starting PTY SSH Terminal");
    switch(cdata->pid = fork()) {
        case -1:
            close(cdata->pty_master);
            close(cdata->pty_slave);
            logcritical("Failed to Fork PTY!");
            return SSH_ERROR;
        case 0:
            
            close(cdata->pty_master);
            if (login_tty(cdata->pty_slave) != 0) {
                exit(1);
            }

            // PUT CHANGE IN PIPING HERE?






            // NEW LOOP OR NEW FILE EVEN!!!




            // HANDOFF!!!

            fprintf(stderr, "Start of new command line\n");
            fprintf(stderr, "Hello, World!\n");
            loginfo("TEST123");
            

            //execl("/bin/sh", "sh", mode, command, NULL);
            exit(0);
        default:
            close(cdata->pty_slave);
            /* pty fd is bi-directional */
            cdata->child_stdout = cdata->child_stdin = cdata->pty_master;
    }
    return SSH_OK;
}








/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
//// PTY TERMINAL SINGLE COMMAND EXECUTION SHELL ////
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
static int exec_nopty(const char *command, struct channel_data_struct *cdata) {
    int in[2], out[2], err[2];

    /* Do the plumbing to be able to talk with the child process. */
    if (pipe(in) != 0) {
        goto stdin_failed;
    }
    if (pipe(out) != 0) {
        goto stdout_failed;
    }
    if (pipe(err) != 0) {
        goto stderr_failed;
    }

    fprintf(stderr, "29\n");

    switch(cdata->pid = fork()) {
        case -1:
            goto fork_failed;
        case 0:
            /* Finish the plumbing in the child process. */
            close(in[1]);
            close(out[0]);
            close(err[0]);
            dup2(in[0], STDIN_FILENO);
            dup2(out[1], STDOUT_FILENO);
            dup2(err[1], STDERR_FILENO);
            close(in[0]);
            close(out[1]);
            close(err[1]);
            /* exec the requested command. */
            fprintf(stderr, "test\n");
            execl("/bin/sh", "sh", "-c", command, NULL);
            exit(0);
    }

    close(in[0]);
    close(out[1]);
    close(err[1]);

    cdata->child_stdin = in[1];
    cdata->child_stdout = out[0];
    cdata->child_stderr = err[0];

    return SSH_OK;

fork_failed:
    close(err[0]);
    close(err[1]);
stderr_failed:
    close(out[0]);
    close(out[1]);
stdout_failed:
    close(in[0]);
    close(in[1]);
stdin_failed:
    return SSH_ERROR;
}






/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
//// PTY TERMINAL SINGLE COMMAND EXECUTION SHELL ////
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
static int exec_request(ssh_session session, ssh_channel channel, const char *command, void *userdata) {
    struct channel_data_struct *cdata = (struct channel_data_struct *) userdata;

    fprintf(stderr, "32\n");

    (void) session;
    (void) channel;

    if(cdata->pid > 0) {
        return SSH_ERROR;
    }

    if (cdata->pty_master != -1 && cdata->pty_slave != -1) {
        return exec_pty("-c", command, cdata);
    }
    return exec_nopty(command, cdata);
}






/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
//// SHELL REQUEST START SHELL EXECUTION LOOP ///////
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
static int shell_request(ssh_session session, ssh_channel channel, void *userdata) {
    struct channel_data_struct *cdata = (struct channel_data_struct *) userdata;

    (void) session;
    (void) channel;

    fprintf(stderr, "72\n");

    if(cdata->pid > 0) {
        return SSH_ERROR;
    }

    if (cdata->pty_master != -1 && cdata->pty_slave != -1) {
        return exec_pty("-l", NULL, cdata);
    }
    /* Client requested a shell without a pty, let's pretend we allow that */
    return SSH_OK;
}







///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
//// SUBSYSTEM REQUESTED LOOP (NOT CURRENTLY SUPPORTED) ///////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
static int subsystem_request(ssh_session session, ssh_channel channel, const char *subsystem, void *userdata) {
    /* subsystem requests behave simillarly to exec requests. */
    if (strcmp(subsystem, "sftp") == 0) {
        return exec_request(session, channel, SFTP_SERVER_PATH, userdata);
    }
    return SSH_ERROR;
}






//////////////////////////////////////////////////
//////////////////////////////////////////////////
//// AUTHORIZE SSH THROUGH PASSWORD METHOD ///////
//////////////////////////////////////////////////
//////////////////////////////////////////////////
static int auth_password(ssh_session session, const char *user, const char *pass, void *userdata) {
    struct session_data_struct *sdata = (struct session_data_struct *) userdata;

    (void) session;

    if (strcmp(user, USER) == 0 && strcmp(pass, PASS) == 0) {
        sdata->authenticated = 1;
        return SSH_AUTH_SUCCESS;
    }

    sdata->auth_attempts++;
    return SSH_AUTH_DENIED;
}







////////////////////////////////////////////////////
////////////////////////////////////////////////////
//// AUTHORIZE SSH THROUGH PUBLIC KEY METHOD ///////
////////////////////////////////////////////////////
////////////////////////////////////////////////////
static int auth_publickey(ssh_session session, const char *user, struct ssh_key_struct *pubkey, char signature_state, void *userdata) {
    struct session_data_struct *sdata = (struct session_data_struct *) userdata;

    (void) user;
    (void) session;

    if (signature_state == SSH_PUBLICKEY_STATE_NONE) {
        return SSH_AUTH_SUCCESS;
    }

    if (signature_state != SSH_PUBLICKEY_STATE_VALID) {
        return SSH_AUTH_DENIED;
    }

    // valid so far.  Now look through authorized keys for a match
    if (authorizedkeys[0]) {
        ssh_key key = NULL;
        int result;
        struct stat buf;

        if (stat(authorizedkeys, &buf) == 0) {
            result = ssh_pki_import_pubkey_file( authorizedkeys, &key );
            if ((result != SSH_OK) || (key==NULL)) {
                fprintf(stderr,
                        "Unable to import public key file %s\n",
                        authorizedkeys);
            } else {
                result = ssh_key_cmp( key, pubkey, SSH_KEY_CMP_PUBLIC );
                ssh_key_free(key);
                if (result == 0) {
                    sdata->authenticated = 1;
                    return SSH_AUTH_SUCCESS;
                }
            }
        }
    }

    // no matches
    sdata->authenticated = 0;
    return SSH_AUTH_DENIED;
}








/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
//// OPEN SSH CHANNELS UPON RECEIVING CONNECTION ////
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
static ssh_channel channel_open(ssh_session session, void *userdata) {
    struct session_data_struct *sdata = (struct session_data_struct *) userdata;

    sdata->channel = ssh_channel_new(session);
    return sdata->channel;
}








//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
//// SEND UPDATED INFORMATION BACK ON SSH TUNNEL (STDOUT) ////
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
static int process_stdout(socket_t fd, int revents, void *userdata) {
    char buf[BUF_SIZE];
    int n = -1;
    
    ssh_channel channel = (ssh_channel) userdata;

    if (channel != NULL && (revents & POLLIN) != 0) {
        n = read(fd, buf, BUF_SIZE);
        std::string buffer = buf;
        if (n > 0) {
            fprintf(stderr, buffer.c_str());
            ssh_channel_write(channel, buf, n);
            memset(buf, 0, BUF_SIZE);
        }
    }

    return n;
}








//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
//// SEND UPDATED INFORMATION BACK ON SSH TUNNEL (STDERR) ////
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
static int process_stderr(socket_t fd, int revents, void *userdata) {
    char buf[BUF_SIZE];
    int n = -1;
    ssh_channel channel = (ssh_channel) userdata;

    if (channel != NULL && (revents & POLLIN) != 0) {
        n = read(fd, buf, BUF_SIZE);
        if (n > 0) {
            ssh_channel_write_stderr(channel, buf, n);
        }
    }

    return n;
}








static void handle_session(ssh_event event, ssh_session session) {
    int n;
    int rc = 0;

    /* Structure for storing the pty size. */
    struct winsize wsize = {
        .ws_row = 0,
        .ws_col = 0,
        .ws_xpixel = 0,
        .ws_ypixel = 0
    };

    /* Our struct holding information about the channel. */
    struct channel_data_struct cdata = {
        .pid = 0,
        .pty_master = -1,
        .pty_slave = -1,
        .child_stdin = -1,
        .child_stdout = -1,
        .child_stderr = -1,
        .event = NULL,
        .winsize = &wsize
    };

    /* Our struct holding information about the session. */
    struct session_data_struct sdata = {
        .channel = NULL,
        .auth_attempts = 0,
        .authenticated = 0
    };

     
    struct ssh_channel_callbacks_struct channel_cb = {
        // REORDERED STRUCT
        .userdata = &cdata,
        .channel_data_function = data_function,
        .channel_pty_request_function = pty_request,
        .channel_shell_request_function = shell_request,
        .channel_pty_window_change_function = pty_resize,
        .channel_exec_request_function = exec_request,
        .channel_subsystem_request_function = subsystem_request

    };

    struct ssh_server_callbacks_struct server_cb = {
        .userdata = &sdata,
        .auth_password_function = auth_password,
        .channel_open_request_session_function = channel_open,
    };

    if (authorizedkeys[0]) {
        server_cb.auth_pubkey_function = auth_publickey;
        ssh_set_auth_methods(session, SSH_AUTH_METHOD_PASSWORD | SSH_AUTH_METHOD_PUBLICKEY);
    } else
        ssh_set_auth_methods(session, SSH_AUTH_METHOD_PASSWORD);

    ssh_callbacks_init(&server_cb);
    ssh_callbacks_init(&channel_cb);

    ssh_set_server_callbacks(session, &server_cb);


    if (ssh_handle_key_exchange(session) != SSH_OK) {
        fprintf(stderr, "%s\n", ssh_get_error(session));
        return;
    }

    ssh_event_add_session(event, session);

    n = 0;
    while (sdata.authenticated == 0 || sdata.channel == NULL) {
        /* If the user has used up all attempts, or if he hasn't been able to
         * authenticate in 10 seconds (n * 100ms), disconnect. */
        if (sdata.auth_attempts >= 3 || n >= 100) {
            return;
        }

        if (ssh_event_dopoll(event, 100) == SSH_ERROR) {
            fprintf(stderr, "%s\n", ssh_get_error(session));
            return;
        }
        n++;
    }

    // AUTHENTICATE UP
    // RUN SESSION BELOW?
    
    fprintf(stderr, "reached here\n");

    ssh_set_channel_callbacks(sdata.channel, &channel_cb);

    do {
        /* Poll the main event which takes care of the session, the channel and
         * even our child process's stdout/stderr (once it's started). */
        if (ssh_event_dopoll(event, -1) == SSH_ERROR) {
            fprintf(stderr, "reached here23\n");
          ssh_channel_close(sdata.channel);
        }

        /* If child process's stdout/stderr has been registered with the event,
         * or the child process hasn't started yet, continue. */
        if (cdata.event != NULL || cdata.pid == 0) {
            continue;
            
            fprintf(stderr, "reached here2\n");
        }
        /* Executed only once, once the child process starts. */







        cdata.event = event; // DECLARATION?







        // POLLING FOR COMMAND/RUNNING COMMAND?
        /* If stdout valid, add stdout to be monitored by the poll event. */
        if (cdata.child_stdout != -1) {
            if (ssh_event_add_fd(event, cdata.child_stdout, POLLIN, process_stdout,
                                 sdata.channel) != SSH_OK) {
                fprintf(stderr, "Failed to register stdout to poll context\n");
                ssh_channel_close(sdata.channel);
            }
        }



        
        // NOT NEEDED IN CONTEXT?
        /* If stderr valid, add stderr to be monitored by the poll event. 
        if (cdata.child_stderr != -1){
            if (ssh_event_add_fd(event, cdata.child_stderr, POLLIN, process_stderr,
                                 sdata.channel) != SSH_OK) {
                fprintf(stderr, "Failed to register stderr to poll context\n");
                ssh_channel_close(sdata.channel);
            }
        }
        */
        fprintf(stderr, "reached here3\n");
    } 
    
    
    // WAIT STATE
    while(ssh_channel_is_open(sdata.channel) && (cdata.pid == 0 || waitpid(cdata.pid, &rc, WNOHANG) == 0));

    close(cdata.pty_master);
    close(cdata.child_stdin);
    close(cdata.child_stdout);
    close(cdata.child_stderr);

    /* Remove the descriptors from the polling context, since they are now
     * closed, they will always trigger during the poll calls. */
    ssh_event_remove_fd(event, cdata.child_stdout);
    ssh_event_remove_fd(event, cdata.child_stderr);

    /* If the child process exited. */
    
    fprintf(stderr, "reached here4\n");
    if (kill(cdata.pid, 0) < 0 && WIFEXITED(rc)) {
        rc = WEXITSTATUS(rc);
        ssh_channel_request_send_exit_status(sdata.channel, rc);
    /* If client terminated the channel or the process did not exit nicely,
     * but only if something has been forked. */
    } else if (cdata.pid > 0) {
        kill(cdata.pid, SIGKILL);
    }

    ssh_channel_send_eof(sdata.channel);
    ssh_channel_close(sdata.channel);

    /* Wait up to 5 seconds for the client to terminate the session. */
    for (n = 0; n < 50 && (ssh_get_status(session) & SESSION_END) == 0; n++) {
        ssh_event_dopoll(event, 100);
    }
}






/* SIGCHLD handler for cleaning up dead children. */
static void sigchld_handler(int signo) {
    (void) signo;
    while (waitpid(-1, NULL, WNOHANG) > 0);
}






int main(int argc, char **argv) {
    ssh_bind sshbind;
    ssh_session session;
    ssh_event event;
    struct sigaction sa;
    int rc;

    //FIFO
    int fd;
    mkfifo(myfifo.c_str(), 0666);
    std::cout << "MADE PIPE1" << std::endl;


    /* Set up SIGCHLD handler. */
    sa.sa_handler = sigchld_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART | SA_NOCLDSTOP;
    if (sigaction(SIGCHLD, &sa, NULL) != 0) {
        fprintf(stderr, "Failed to register SIGCHLD handler\n");
        return 1;
    }

    rc = ssh_init();
    if (rc < 0) {
        fprintf(stderr, "ssh_init failed\n");
        return 1;
    }

    sshbind = ssh_bind_new();
    if (sshbind == NULL) {
        fprintf(stderr, "ssh_bind_new failed\n");
        return 1;
    }

#ifdef HAVE_ARGP_H
    argp_parse(&argp, argc, argv, 0, 0, sshbind);
#else
    (void) argc;
    (void) argv;

    set_default_keys(sshbind, 0, 0, 0);
#endif /* HAVE_ARGP_H */

    if(ssh_bind_listen(sshbind) < 0) {
        fprintf(stderr, "%s\n", ssh_get_error(sshbind));
        return 1;
    }

    while (1) {
        session = ssh_new();
        if (session == NULL) {
            fprintf(stderr, "Failed to allocate session\n");
            continue;
        }

        /* Blocks until there is a new incoming connection. */
        if(ssh_bind_accept(sshbind, session) != SSH_ERROR) {
            switch(fork()) {
                case 0:
                    /* Remove the SIGCHLD handler inherited from parent. */
                    sa.sa_handler = SIG_DFL;
                    sigaction(SIGCHLD, &sa, NULL);
                    /* Remove socket binding, which allows us to restart the
                     * parent process, without terminating existing sessions. */
                    ssh_bind_free(sshbind);

                    // CONVERT INTO NONBLOCKING EVENT
                    event = ssh_event_new();
                    if (event != NULL) {
                        /* Blocks until the SSH session ends by either
                         * child process exiting, or client disconnecting. */
                        handle_session(event, session);
                        ssh_event_free(event);
                    } else {
                        fprintf(stderr, "Could not create polling context\n");
                    }
                    ssh_disconnect(session);
                    ssh_free(session);

                    exit(0);
                case -1:
                    fprintf(stderr, "Failed to fork\n");
            }
        } else {
            fprintf(stderr, "%s\n", ssh_get_error(sshbind));
        }
        /* Since the session has been passed to a child fork, do some cleaning
         * up at the parent process. */
        ssh_disconnect(session);
        ssh_free(session);
    }

    ssh_bind_free(sshbind);
    ssh_finalize();
    return 0;
}