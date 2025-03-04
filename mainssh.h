// HEADER FILE FOR SSH FUNCTIONS
//#include "globalvariables.h"

//#include "mainssh.cpp"
#include <string>
#include <libssh/libssh.h>
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




#ifndef MAINSSH_H
#define MAINSSH_H

// SET RANDOM RSA KEYS  
void set_default_keys(ssh_bind, int, int, int);

// READ DATA FUNCTION FROM SSH
static int data_function(ssh_session, ssh_channel, void*, uint32_t, int, void*);

// MAIN EXECUTING LOOP FUNCTION
static int exec_pty(const char*, const char*, struct channel_data_struct);

// PASSWORD SSH
static int auth_password(ssh_session, const char*, const char*, void*);

// AUTHORIZING SSH THROUGH PUBKEY METHOD
static int auth_publickey(ssh_session, const char*, struct ssh_key_struct*, char, void*);

// MAIN HANDLE FOR SSH SESSION
void handle_session(ssh_event, ssh_session);


#endif