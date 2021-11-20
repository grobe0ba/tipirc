// Copyright (c) 2021 B. Atticus Grobe (grobe0ba@gmail.com)
//
// Permission to use, copy, modify, and distribute this software for any
// purpose with or without fee is hereby granted, provided that the above
// copyright notice and this permission notice appear in all copies.
//
// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
// WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
// ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
// WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
// ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
// OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

#include "tipirc.hh"

#include <gflags/gflags.h>
#include <glog/logging.h>
#include <uv.h>

#include <string>

#if INTERFACE
#include <uv.h>
#define CALLBACKS(X) \
  X(READY)           \
  X(PRIVMSG)         \
  X(WHOIS)           \
  X(CTCP)
#endif

#if EXPORT_INTERFACE
typedef enum {
  tipEvent_BAD = -1,
  tipEvent_Reply,
  tipEvent_Command,
  tipEvent_MAX
} tipEventType;
typedef struct tipEvent {
  tipEventType type;
  union {
    RPL rpl;
    CMD cmd;
  };
  void *data;
};

typedef void (*tipCallback)(void *, tipEvent *);

class tipirc {
 public:
  tipirc(void);
  virtual ~tipirc() = 0;
#define ACC(msg) \
  void setOn##msg(tipCallback f) { On##msg = f; };
  CALLBACKS(ACC)
#undef ACC
#define PROTO(com, args...) void cmd##com(args);
  CMDS(PROTO)
#undef PROTO
 private:
  uv_tcp_t *handle;
#define PROTO(msg) tipCallback On##msg;
  CALLBACKS(PROTO)
#undef PROTO
};

#endif

#define DEFAULT(msg)                            \
  void defaultOn##msg(void *ctx, tipEvent *e) { \
    LOG(INFO) << "Received message " << #msg;   \
  }
CALLBACKS(DEFAULT)
#undef DEFAULT

#define DEFEV(msg) void defaultOn##msg(void *, tipEvent *);
CALLBACKS(DEFEV)
#undef DEFEV

tipirc::tipirc(void) {
#define DEFAULT(msg) On##msg = defaultOn##msg;
  CALLBACKS(DEFAULT)
#undef DEFAULT
}

#ifdef __cplusplus
extern "C" {
#endif
void writeCallback(uv_write_t *r, int status) {}
#ifdef __cplusplus
}
#endif

void tipirc::cmdPASS(const char *password) {}

void tipirc::cmdNICK(const char *nickname) {}

void tipirc::cmdUSER(const char *user, const char *mode, const char *realname) {
}

void tipirc::cmdOPER(const char *name, const char *password) {}

void tipirc::cmdSERVICE(const char *nickname, const char *distribution,
                        const char *type, const char *info) {}

void tipirc::cmdQUIT(const char *message) {}

void tipirc::cmdSQUIT(const char *server, const char *comment) {}

void tipirc::cmdJOIN(const cmdList2 channels[]) {}

void tipirc::cmdPART(const cmdList2 channels[], const char *message) {}

void tipirc::cmdMODE(const char *channel, const char **modes,
                     const char **modeparams) {}
void tipirc::cmdTOPIC(const char *channel, const char *topic) {}

void tipirc::cmdNAMES(const cmdList2 channels[], const char *target) {}

void tipirc::cmdLIST(const cmdList2 channels[], const char *target) {}

void tipirc::cmdINVITE(const char *nickname, const char *channel) {}

void tipirc::cmdKICK(const cmdList2 channels[], const cmdList2 users[],
                     char *comment) {}

void tipirc::cmdPRIVMSG(const char *target, const char *message) {}

void tipirc::cmdNOTICE(const char *target, const char *text) {}

void tipirc::cmdMOTD(const char *target) {}

void tipirc::cmdLUSERS(const char *mask, const char *target) {}

void tipirc::cmdVERSION(const char *target) {}

void tipirc::cmdSTATS(const char *query, const char *target) {}

void tipirc::cmdLINKS(const char *remote_server, const char *server_mask) {}

void tipirc::cmdTIME(const char *target) {}

void tipirc::cmdCONNECT(const char *target_server, int port,
                        const char *remote_server) {}

void tipirc::cmdTRACE(const char *target) {}

void tipirc::cmdADMIN(const char *target) {}

void tipirc::cmdINFO(const char *target) {}

void tipirc::cmdSERVLIST(const char *mask, const char *type) {}

void tipirc::cmdSQUERY(const char *servicename, const char *text) {}

void tipirc::cmdWHO(const char *mask) {}

void tipirc::cmdWHOIS(const char *target, const cmdList2 masks[]) {}

void tipirc::cmdWHOWAS(const cmdList2 nicknames[], int count,
                       const char *target) {}

void tipirc::cmdKILL(const char *nickname, const char *comment) {}

void tipirc::cmdPING(const char *server1, const char *server2) {}

void tipirc::cmdPONG(const char *server1, const char *server2) {}

void tipirc::cmdERROR(const char *error_message) {}

void tipirc::cmdAWAY(const char *text) {}

void tipirc::cmdREHASH() {}

void tipirc::cmdDIE() {}

void tipirc::cmdRESTART() {}

void tipirc::cmdSUMMON(const char *user, const char *target,
                       const char *channel) {}
void tipirc::cmdUSERS(const char *target) {}

void tipirc::cmdWALLOPS(const char *text) {}

void tipirc::cmdUSERHOST(const cmdList2 nicknames[]) {}

void tipirc::cmdISON(const cmdList2 nicknames[]) {}

EXPORT int tipinit(int argc, char **argv) {
  google::InitGoogleLogging(argv[0]);
  gflags::SetUsageMessage("Rubbish Tip IRC library");
  gflags::ParseCommandLineFlags(&argc, &argv, true);
  return 0;
}
