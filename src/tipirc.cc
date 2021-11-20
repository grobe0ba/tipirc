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

#define WBUFSZ 512

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
#define PROTO(com, fmt, args...) void cmd##com(args);
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
void writeCallback(uv_write_t *r, int status) {
  free(r->data);
  free(r);
}
#ifdef __cplusplus
}
#endif

// #define HANDLEMSG(com, fmt, args...)                                          \
//   void tipirc::cmd##com(args) {                                               \
//     char *buf;                                                                \
//     CHECK((buf = static_cast<char *>(calloc(WBUFSZ, sizeof(char)))) != NULL); \
//     snprintf(buf, WBUFSZ, #com fmt "\r\n", args);                              \
//     uv_buf_t b = uv_buf_init(strndup(buf, WBUFSZ), strlen(buf) + 1);          \
//     uv_write_t *w;                                                            \
//     CHECK((w = static_cast<uv_write_t *>(malloc(sizeof(uv_write_t)))) != NULL); \
//     w->data = buf;                                                            \
//     uv_write(w, (uv_stream_t*)this->handle, &b, 1, writeCallback);                          \
//   }
//
// CMDS(HANDLEMSG)

void tipirc::cmdPASS(const char *password) {
  char *buf;
  CHECK((buf = static_cast<char *>(calloc(512, sizeof(char)))) != nullptr);
  snprintf(buf, 512,
           "PASS"
           "%s"
           "\r\n",
           password);
  uv_buf_t b = uv_buf_init(strndup(buf, 512), strlen(buf) + 1);
  uv_write_t *w;
  CHECK((w = static_cast<uv_write_t *>(malloc(sizeof(uv_write_t)))) != nullptr);
  w->data = buf;
  uv_write(w, (uv_stream_t *)this->handle, &b, 1, writeCallback);
}
void tipirc::cmdNICK(const char *nickname) {
  char *buf;
  CHECK((buf = static_cast<char *>(calloc(512, sizeof(char)))) != nullptr);
  snprintf(buf, 512,
           "NICK"
           "%s"
           "\r\n",
           nickname);
  uv_buf_t b = uv_buf_init(strndup(buf, 512), strlen(buf) + 1);
  uv_write_t *w;
  CHECK((w = static_cast<uv_write_t *>(malloc(sizeof(uv_write_t)))) != nullptr);
  w->data = buf;
  uv_write(w, (uv_stream_t *)this->handle, &b, 1, writeCallback);
}
void tipirc::cmdUSER(const char *user, const char *mode, const char *realname) {
  char *buf;
  CHECK((buf = static_cast<char *>(calloc(512, sizeof(char)))) != nullptr);
  snprintf(buf, 512,
           "USER"
           "%s %s * %s"
           "\r\n",
           user, mode, realname);
  uv_buf_t b = uv_buf_init(strndup(buf, 512), strlen(buf) + 1);
  uv_write_t *w;
  CHECK((w = static_cast<uv_write_t *>(malloc(sizeof(uv_write_t)))) != nullptr);
  w->data = buf;
  uv_write(w, (uv_stream_t *)this->handle, &b, 1, writeCallback);
}
void tipirc::cmdOPER(const char *name, const char *password) {
  char *buf;
  CHECK((buf = static_cast<char *>(calloc(512, sizeof(char)))) != nullptr);
  snprintf(buf, 512,
           "OPER"
           "%s %s"
           "\r\n",
           name, password);
  uv_buf_t b = uv_buf_init(strndup(buf, 512), strlen(buf) + 1);
  uv_write_t *w;
  CHECK((w = static_cast<uv_write_t *>(malloc(sizeof(uv_write_t)))) != nullptr);
  w->data = buf;
  uv_write(w, (uv_stream_t *)this->handle, &b, 1, writeCallback);
}
void tipirc::cmdSERVICE(const char *nickname, const char *distribution,
                        const char *type, const char *info) {
  char *buf;
  CHECK((buf = static_cast<char *>(calloc(512, sizeof(char)))) != nullptr);
  snprintf(buf, 512,
           "SERVICE"
           "%s * %s %s * %s"
           "\r\n",
           nickname, distribution, type, info);
  uv_buf_t b = uv_buf_init(strndup(buf, 512), strlen(buf) + 1);
  uv_write_t *w;
  CHECK((w = static_cast<uv_write_t *>(malloc(sizeof(uv_write_t)))) != nullptr);
  w->data = buf;
  uv_write(w, (uv_stream_t *)this->handle, &b, 1, writeCallback);
}
void tipirc::cmdQUIT(const char *message) {
  char *buf;
  CHECK((buf = static_cast<char *>(calloc(512, sizeof(char)))) != nullptr);
  snprintf(buf, 512,
           "QUIT"
           "%s"
           "\r\n",
           message);
  uv_buf_t b = uv_buf_init(strndup(buf, 512), strlen(buf) + 1);
  uv_write_t *w;
  CHECK((w = static_cast<uv_write_t *>(malloc(sizeof(uv_write_t)))) != nullptr);
  w->data = buf;
  uv_write(w, (uv_stream_t *)this->handle, &b, 1, writeCallback);
}
void tipirc::cmdSQUIT(const char *server, const char *comment) {
  char *buf;
  CHECK((buf = static_cast<char *>(calloc(512, sizeof(char)))) != nullptr);
  snprintf(buf, 512,
           "SQUIT"
           "%s %s"
           "\r\n",
           server, comment);
  uv_buf_t b = uv_buf_init(strndup(buf, 512), strlen(buf) + 1);
  uv_write_t *w;
  CHECK((w = static_cast<uv_write_t *>(malloc(sizeof(uv_write_t)))) != nullptr);
  w->data = buf;
  uv_write(w, (uv_stream_t *)this->handle, &b, 1, writeCallback);
}
void tipirc::cmdTOPIC(const char *channel, const char *topic) {
  char *buf;
  CHECK((buf = static_cast<char *>(calloc(512, sizeof(char)))) != nullptr);
  snprintf(buf, 512,
           "TOPIC"
           "%s %s"
           "\r\n",
           channel, topic);
  uv_buf_t b = uv_buf_init(strndup(buf, 512), strlen(buf) + 1);
  uv_write_t *w;
  CHECK((w = static_cast<uv_write_t *>(malloc(sizeof(uv_write_t)))) != nullptr);
  w->data = buf;
  uv_write(w, (uv_stream_t *)this->handle, &b, 1, writeCallback);
}
void tipirc::cmdINVITE(const char *nickname, const char *channel) {
  char *buf;
  CHECK((buf = static_cast<char *>(calloc(512, sizeof(char)))) != nullptr);
  snprintf(buf, 512,
           "INVITE"
           "%s %s"
           "\r\n",
           nickname, channel);
  uv_buf_t b = uv_buf_init(strndup(buf, 512), strlen(buf) + 1);
  uv_write_t *w;
  CHECK((w = static_cast<uv_write_t *>(malloc(sizeof(uv_write_t)))) != nullptr);
  w->data = buf;
  uv_write(w, (uv_stream_t *)this->handle, &b, 1, writeCallback);
}
void tipirc::cmdPRIVMSG(const char *target, const char *message) {
  char *buf;
  CHECK((buf = static_cast<char *>(calloc(512, sizeof(char)))) != nullptr);
  snprintf(buf, 512,
           "PRIVMSG"
           "%s %s"
           "\r\n",
           target, message);
  uv_buf_t b = uv_buf_init(strndup(buf, 512), strlen(buf) + 1);
  uv_write_t *w;
  CHECK((w = static_cast<uv_write_t *>(malloc(sizeof(uv_write_t)))) != nullptr);
  w->data = buf;
  uv_write(w, (uv_stream_t *)this->handle, &b, 1, writeCallback);
}
void tipirc::cmdNOTICE(const char *target, const char *text) {
  char *buf;
  CHECK((buf = static_cast<char *>(calloc(512, sizeof(char)))) != nullptr);
  snprintf(buf, 512,
           "NOTICE"
           "%s %s"
           "\r\n",
           target, text);
  uv_buf_t b = uv_buf_init(strndup(buf, 512), strlen(buf) + 1);
  uv_write_t *w;
  CHECK((w = static_cast<uv_write_t *>(malloc(sizeof(uv_write_t)))) != nullptr);
  w->data = buf;
  uv_write(w, (uv_stream_t *)this->handle, &b, 1, writeCallback);
}
void tipirc::cmdMOTD(const char *target) {
  char *buf;
  CHECK((buf = static_cast<char *>(calloc(512, sizeof(char)))) != nullptr);
  snprintf(buf, 512,
           "MOTD"
           "%s"
           "\r\n",
           target);
  uv_buf_t b = uv_buf_init(strndup(buf, 512), strlen(buf) + 1);
  uv_write_t *w;
  CHECK((w = static_cast<uv_write_t *>(malloc(sizeof(uv_write_t)))) != nullptr);
  w->data = buf;
  uv_write(w, (uv_stream_t *)this->handle, &b, 1, writeCallback);
}
void tipirc::cmdLUSERS(const char *mask, const char *target) {
  char *buf;
  CHECK((buf = static_cast<char *>(calloc(512, sizeof(char)))) != nullptr);
  snprintf(buf, 512,
           "LUSERS"
           "%s %s"
           "\r\n",
           mask, target);
  uv_buf_t b = uv_buf_init(strndup(buf, 512), strlen(buf) + 1);
  uv_write_t *w;
  CHECK((w = static_cast<uv_write_t *>(malloc(sizeof(uv_write_t)))) != nullptr);
  w->data = buf;
  uv_write(w, (uv_stream_t *)this->handle, &b, 1, writeCallback);
}
void tipirc::cmdVERSION(const char *target) {
  char *buf;
  CHECK((buf = static_cast<char *>(calloc(512, sizeof(char)))) != nullptr);
  snprintf(buf, 512,
           "VERSION"
           "%s"
           "\r\n",
           target);
  uv_buf_t b = uv_buf_init(strndup(buf, 512), strlen(buf) + 1);
  uv_write_t *w;
  CHECK((w = static_cast<uv_write_t *>(malloc(sizeof(uv_write_t)))) != nullptr);
  w->data = buf;
  uv_write(w, (uv_stream_t *)this->handle, &b, 1, writeCallback);
}
void tipirc::cmdSTATS(const char *query, const char *target) {
  char *buf;
  CHECK((buf = static_cast<char *>(calloc(512, sizeof(char)))) != nullptr);
  snprintf(buf, 512,
           "STATS"
           "%s %s"
           "\r\n",
           query, target);
  uv_buf_t b = uv_buf_init(strndup(buf, 512), strlen(buf) + 1);
  uv_write_t *w;
  CHECK((w = static_cast<uv_write_t *>(malloc(sizeof(uv_write_t)))) != nullptr);
  w->data = buf;
  uv_write(w, (uv_stream_t *)this->handle, &b, 1, writeCallback);
}
void tipirc::cmdLINKS(const char *remote_server, const char *server_mask) {
  char *buf;
  CHECK((buf = static_cast<char *>(calloc(512, sizeof(char)))) != nullptr);
  snprintf(buf, 512,
           "LINKS"
           "%s %s"
           "\r\n",
           remote_server, server_mask);
  uv_buf_t b = uv_buf_init(strndup(buf, 512), strlen(buf) + 1);
  uv_write_t *w;
  CHECK((w = static_cast<uv_write_t *>(malloc(sizeof(uv_write_t)))) != nullptr);
  w->data = buf;
  uv_write(w, (uv_stream_t *)this->handle, &b, 1, writeCallback);
}
void tipirc::cmdTIME(const char *target) {
  char *buf;
  CHECK((buf = static_cast<char *>(calloc(512, sizeof(char)))) != nullptr);
  snprintf(buf, 512,
           "TIME"
           "%s"
           "\r\n",
           target);
  uv_buf_t b = uv_buf_init(strndup(buf, 512), strlen(buf) + 1);
  uv_write_t *w;
  CHECK((w = static_cast<uv_write_t *>(malloc(sizeof(uv_write_t)))) != nullptr);
  w->data = buf;
  uv_write(w, (uv_stream_t *)this->handle, &b, 1, writeCallback);
}
void tipirc::cmdCONNECT(const char *target_server, int port,
                        const char *remote_server) {
  char *buf;
  CHECK((buf = static_cast<char *>(calloc(512, sizeof(char)))) != nullptr);
  snprintf(buf, 512,
           "CONNECT"
           "%s %d %s"
           "\r\n",
           target_server, port, remote_server);
  uv_buf_t b = uv_buf_init(strndup(buf, 512), strlen(buf) + 1);
  uv_write_t *w;
  CHECK((w = static_cast<uv_write_t *>(malloc(sizeof(uv_write_t)))) != nullptr);
  w->data = buf;
  uv_write(w, (uv_stream_t *)this->handle, &b, 1, writeCallback);
}
void tipirc::cmdTRACE(const char *target) {
  char *buf;
  CHECK((buf = static_cast<char *>(calloc(512, sizeof(char)))) != nullptr);
  snprintf(buf, 512,
           "TRACE"
           "%s"
           "\r\n",
           target);
  uv_buf_t b = uv_buf_init(strndup(buf, 512), strlen(buf) + 1);
  uv_write_t *w;
  CHECK((w = static_cast<uv_write_t *>(malloc(sizeof(uv_write_t)))) != nullptr);
  w->data = buf;
  uv_write(w, (uv_stream_t *)this->handle, &b, 1, writeCallback);
}
void tipirc::cmdADMIN(const char *target) {
  char *buf;
  CHECK((buf = static_cast<char *>(calloc(512, sizeof(char)))) != nullptr);
  snprintf(buf, 512,
           "ADMIN"
           "%s"
           "\r\n",
           target);
  uv_buf_t b = uv_buf_init(strndup(buf, 512), strlen(buf) + 1);
  uv_write_t *w;
  CHECK((w = static_cast<uv_write_t *>(malloc(sizeof(uv_write_t)))) != nullptr);
  w->data = buf;
  uv_write(w, (uv_stream_t *)this->handle, &b, 1, writeCallback);
}
void tipirc::cmdINFO(const char *target) {
  char *buf;
  CHECK((buf = static_cast<char *>(calloc(512, sizeof(char)))) != nullptr);
  snprintf(buf, 512,
           "INFO"
           "%s"
           "\r\n",
           target);
  uv_buf_t b = uv_buf_init(strndup(buf, 512), strlen(buf) + 1);
  uv_write_t *w;
  CHECK((w = static_cast<uv_write_t *>(malloc(sizeof(uv_write_t)))) != nullptr);
  w->data = buf;
  uv_write(w, (uv_stream_t *)this->handle, &b, 1, writeCallback);
}
void tipirc::cmdSERVLIST(const char *mask, const char *type) {
  char *buf;
  CHECK((buf = static_cast<char *>(calloc(512, sizeof(char)))) != nullptr);
  snprintf(buf, 512,
           "SERVLIST"
           "%s %s"
           "\r\n",
           mask, type);
  uv_buf_t b = uv_buf_init(strndup(buf, 512), strlen(buf) + 1);
  uv_write_t *w;
  CHECK((w = static_cast<uv_write_t *>(malloc(sizeof(uv_write_t)))) != nullptr);
  w->data = buf;
  uv_write(w, (uv_stream_t *)this->handle, &b, 1, writeCallback);
}
void tipirc::cmdSQUERY(const char *servicename, const char *text) {
  char *buf;
  CHECK((buf = static_cast<char *>(calloc(512, sizeof(char)))) != nullptr);
  snprintf(buf, 512,
           "SQUERY"
           "%s %s"
           "\r\n",
           servicename, text);
  uv_buf_t b = uv_buf_init(strndup(buf, 512), strlen(buf) + 1);
  uv_write_t *w;
  CHECK((w = static_cast<uv_write_t *>(malloc(sizeof(uv_write_t)))) != nullptr);
  w->data = buf;
  uv_write(w, (uv_stream_t *)this->handle, &b, 1, writeCallback);
}
void tipirc::cmdWHO(const char *mask) {
  char *buf;
  CHECK((buf = static_cast<char *>(calloc(512, sizeof(char)))) != nullptr);
  snprintf(buf, 512,
           "WHO"
           "%s"
           "\r\n",
           mask);
  uv_buf_t b = uv_buf_init(strndup(buf, 512), strlen(buf) + 1);
  uv_write_t *w;
  CHECK((w = static_cast<uv_write_t *>(malloc(sizeof(uv_write_t)))) != nullptr);
  w->data = buf;
  uv_write(w, (uv_stream_t *)this->handle, &b, 1, writeCallback);
}
void tipirc::cmdKILL(const char *nickname, const char *comment) {
  char *buf;
  CHECK((buf = static_cast<char *>(calloc(512, sizeof(char)))) != nullptr);
  snprintf(buf, 512,
           "KILL"
           "%s %s"
           "\r\n",
           nickname, comment);
  uv_buf_t b = uv_buf_init(strndup(buf, 512), strlen(buf) + 1);
  uv_write_t *w;
  CHECK((w = static_cast<uv_write_t *>(malloc(sizeof(uv_write_t)))) != nullptr);
  w->data = buf;
  uv_write(w, (uv_stream_t *)this->handle, &b, 1, writeCallback);
}
void tipirc::cmdPING(const char *server1, const char *server2) {
  char *buf;
  CHECK((buf = static_cast<char *>(calloc(512, sizeof(char)))) != nullptr);
  snprintf(buf, 512,
           "PING"
           "%s %s"
           "\r\n",
           server1, server2);
  uv_buf_t b = uv_buf_init(strndup(buf, 512), strlen(buf) + 1);
  uv_write_t *w;
  CHECK((w = static_cast<uv_write_t *>(malloc(sizeof(uv_write_t)))) != nullptr);
  w->data = buf;
  uv_write(w, (uv_stream_t *)this->handle, &b, 1, writeCallback);
}
void tipirc::cmdPONG(const char *server1, const char *server2) {
  char *buf;
  CHECK((buf = static_cast<char *>(calloc(512, sizeof(char)))) != nullptr);
  snprintf(buf, 512,
           "PONG"
           "%s %s"
           "\r\n",
           server1, server2);
  uv_buf_t b = uv_buf_init(strndup(buf, 512), strlen(buf) + 1);
  uv_write_t *w;
  CHECK((w = static_cast<uv_write_t *>(malloc(sizeof(uv_write_t)))) != nullptr);
  w->data = buf;
  uv_write(w, (uv_stream_t *)this->handle, &b, 1, writeCallback);
}
void tipirc::cmdERROR(const char *error_message) {
  char *buf;
  CHECK((buf = static_cast<char *>(calloc(512, sizeof(char)))) != nullptr);
  snprintf(buf, 512,
           "ERROR"
           "%s"
           "\r\n",
           error_message);
  uv_buf_t b = uv_buf_init(strndup(buf, 512), strlen(buf) + 1);
  uv_write_t *w;
  CHECK((w = static_cast<uv_write_t *>(malloc(sizeof(uv_write_t)))) != nullptr);
  w->data = buf;
  uv_write(w, (uv_stream_t *)this->handle, &b, 1, writeCallback);
}
void tipirc::cmdAWAY(const char *text) {
  char *buf;
  CHECK((buf = static_cast<char *>(calloc(512, sizeof(char)))) != nullptr);
  snprintf(buf, 512,
           "AWAY"
           "%s"
           "\r\n",
           text);
  uv_buf_t b = uv_buf_init(strndup(buf, 512), strlen(buf) + 1);
  uv_write_t *w;
  CHECK((w = static_cast<uv_write_t *>(malloc(sizeof(uv_write_t)))) != nullptr);
  w->data = buf;
  uv_write(w, (uv_stream_t *)this->handle, &b, 1, writeCallback);
}
void tipirc::cmdSUMMON(const char *user, const char *target,
                       const char *channel) {
  char *buf;
  CHECK((buf = static_cast<char *>(calloc(512, sizeof(char)))) != nullptr);
  snprintf(buf, 512,
           "SUMMON"
           "%s %s %s"
           "\r\n",
           user, target, channel);
  uv_buf_t b = uv_buf_init(strndup(buf, 512), strlen(buf) + 1);
  uv_write_t *w;
  CHECK((w = static_cast<uv_write_t *>(malloc(sizeof(uv_write_t)))) != nullptr);
  w->data = buf;
  uv_write(w, (uv_stream_t *)this->handle, &b, 1, writeCallback);
}
void tipirc::cmdUSERS(const char *target) {
  char *buf;
  CHECK((buf = static_cast<char *>(calloc(512, sizeof(char)))) != nullptr);
  snprintf(buf, 512,
           "USERS"
           "%s"
           "\r\n",
           target);
  uv_buf_t b = uv_buf_init(strndup(buf, 512), strlen(buf) + 1);
  uv_write_t *w;
  CHECK((w = static_cast<uv_write_t *>(malloc(sizeof(uv_write_t)))) != nullptr);
  w->data = buf;
  uv_write(w, (uv_stream_t *)this->handle, &b, 1, writeCallback);
}
void tipirc::cmdWALLOPS(const char *text) {
  char *buf;
  CHECK((buf = static_cast<char *>(calloc(512, sizeof(char)))) != nullptr);
  snprintf(buf, 512,
           "WALLOPS"
           "%s"
           "\r\n",
           text);
  uv_buf_t b = uv_buf_init(strndup(buf, 512), strlen(buf) + 1);
  uv_write_t *w;
  CHECK((w = static_cast<uv_write_t *>(malloc(sizeof(uv_write_t)))) != nullptr);
  w->data = buf;
  uv_write(w, (uv_stream_t *)this->handle, &b, 1, writeCallback);
}

void tipirc::cmdMODE(const char *channel, const char **modes,
                     const char **modeparams) {
  char *buf;
  CHECK((buf = static_cast<char *>(calloc(512, sizeof(char)))) != nullptr);
  snprintf(buf, 512,
           "MODE"
           ""
           "\r\n",
           NULL);
  uv_buf_t b = uv_buf_init(strndup(buf, 512), strlen(buf) + 1);
  uv_write_t *w;
  CHECK((w = static_cast<uv_write_t *>(malloc(sizeof(uv_write_t)))) != nullptr);
  w->data = buf;
  uv_write(w, (uv_stream_t *)this->handle, &b, 1, writeCallback);
}
void tipirc::cmdJOIN(const cmdList2 channels[]) {
  char *buf;
  CHECK((buf = static_cast<char *>(calloc(512, sizeof(char)))) != nullptr);
  snprintf(buf, 512,
           "JOIN"
           ""
           "\r\n",
           NULL);
  uv_buf_t b = uv_buf_init(strndup(buf, 512), strlen(buf) + 1);
  uv_write_t *w;
  CHECK((w = static_cast<uv_write_t *>(malloc(sizeof(uv_write_t)))) != nullptr);
  w->data = buf;
  uv_write(w, (uv_stream_t *)this->handle, &b, 1, writeCallback);
}
void tipirc::cmdPART(const cmdList2 channels[], const char *message) {
  char *buf;
  CHECK((buf = static_cast<char *>(calloc(512, sizeof(char)))) != nullptr);
  snprintf(buf, 512,
           "PART"
           ""
           "\r\n",
           NULL);
  uv_buf_t b = uv_buf_init(strndup(buf, 512), strlen(buf) + 1);
  uv_write_t *w;
  CHECK((w = static_cast<uv_write_t *>(malloc(sizeof(uv_write_t)))) != nullptr);
  w->data = buf;
  uv_write(w, (uv_stream_t *)this->handle, &b, 1, writeCallback);
}
void tipirc::cmdNAMES(const cmdList2 channels[], const char *target) {
  char *buf;
  CHECK((buf = static_cast<char *>(calloc(512, sizeof(char)))) != nullptr);
  snprintf(buf, 512,
           "NAMES"
           ""
           "\r\n",
           NULL);
  uv_buf_t b = uv_buf_init(strndup(buf, 512), strlen(buf) + 1);
  uv_write_t *w;
  CHECK((w = static_cast<uv_write_t *>(malloc(sizeof(uv_write_t)))) != nullptr);
  w->data = buf;
  uv_write(w, (uv_stream_t *)this->handle, &b, 1, writeCallback);
}
void tipirc::cmdLIST(const cmdList2 channels[], const char *target) {
  char *buf;
  CHECK((buf = static_cast<char *>(calloc(512, sizeof(char)))) != nullptr);
  snprintf(buf, 512,
           "LIST"
           ""
           "\r\n",
           NULL);
  uv_buf_t b = uv_buf_init(strndup(buf, 512), strlen(buf) + 1);
  uv_write_t *w;
  CHECK((w = static_cast<uv_write_t *>(malloc(sizeof(uv_write_t)))) != nullptr);
  w->data = buf;
  uv_write(w, (uv_stream_t *)this->handle, &b, 1, writeCallback);
}
void tipirc::cmdKICK(const cmdList2 channels[], const cmdList2 users[],
                     char *comment) {
  char *buf;
  CHECK((buf = static_cast<char *>(calloc(512, sizeof(char)))) != nullptr);
  snprintf(buf, 512,
           "KICK"
           ""
           "\r\n",
           NULL);
  uv_buf_t b = uv_buf_init(strndup(buf, 512), strlen(buf) + 1);
  uv_write_t *w;
  CHECK((w = static_cast<uv_write_t *>(malloc(sizeof(uv_write_t)))) != nullptr);
  w->data = buf;
  uv_write(w, (uv_stream_t *)this->handle, &b, 1, writeCallback);
}
void tipirc::cmdWHOIS(const char *target, const cmdList2 masks[]) {
  char *buf;
  CHECK((buf = static_cast<char *>(calloc(512, sizeof(char)))) != nullptr);
  snprintf(buf, 512,
           "WHOIS"
           ""
           "\r\n",
           NULL);
  uv_buf_t b = uv_buf_init(strndup(buf, 512), strlen(buf) + 1);
  uv_write_t *w;
  CHECK((w = static_cast<uv_write_t *>(malloc(sizeof(uv_write_t)))) != nullptr);
  w->data = buf;
  uv_write(w, (uv_stream_t *)this->handle, &b, 1, writeCallback);
}
void tipirc::cmdWHOWAS(const cmdList2 nicknames[], int count,
                       const char *target) {
  char *buf;
  CHECK((buf = static_cast<char *>(calloc(512, sizeof(char)))) != nullptr);
  snprintf(buf, 512,
           "WHOWAS"
           ""
           "\r\n",
           NULL);
  uv_buf_t b = uv_buf_init(strndup(buf, 512), strlen(buf) + 1);
  uv_write_t *w;
  CHECK((w = static_cast<uv_write_t *>(malloc(sizeof(uv_write_t)))) != nullptr);
  w->data = buf;
  uv_write(w, (uv_stream_t *)this->handle, &b, 1, writeCallback);
}
void tipirc::cmdREHASH() {
  char *buf;
  CHECK((buf = static_cast<char *>(calloc(512, sizeof(char)))) != nullptr);
  snprintf(buf, 512,
           "REHASH"
           ""
           "\r\n",
           NULL);
  uv_buf_t b = uv_buf_init(strndup(buf, 512), strlen(buf) + 1);
  uv_write_t *w;
  CHECK((w = static_cast<uv_write_t *>(malloc(sizeof(uv_write_t)))) != nullptr);
  w->data = buf;
  uv_write(w, (uv_stream_t *)this->handle, &b, 1, writeCallback);
}
void tipirc::cmdDIE() {
  char *buf;
  CHECK((buf = static_cast<char *>(calloc(512, sizeof(char)))) != nullptr);
  snprintf(buf, 512,
           "DIE"
           ""
           "\r\n",
           NULL);
  uv_buf_t b = uv_buf_init(strndup(buf, 512), strlen(buf) + 1);
  uv_write_t *w;
  CHECK((w = static_cast<uv_write_t *>(malloc(sizeof(uv_write_t)))) != nullptr);
  w->data = buf;
  uv_write(w, (uv_stream_t *)this->handle, &b, 1, writeCallback);
}
void tipirc::cmdRESTART() {
  char *buf;
  CHECK((buf = static_cast<char *>(calloc(512, sizeof(char)))) != nullptr);
  snprintf(buf, 512,
           "RESTART"
           ""
           "\r\n",
           NULL);
  uv_buf_t b = uv_buf_init(strndup(buf, 512), strlen(buf) + 1);
  uv_write_t *w;
  CHECK((w = static_cast<uv_write_t *>(malloc(sizeof(uv_write_t)))) != nullptr);
  w->data = buf;
  uv_write(w, (uv_stream_t *)this->handle, &b, 1, writeCallback);
}
void tipirc::cmdUSERHOST(const cmdList2 nicknames[]) {
  char *buf;
  CHECK((buf = static_cast<char *>(calloc(512, sizeof(char)))) != nullptr);
  snprintf(buf, 512,
           "USERHOST"
           ""
           "\r\n",
           NULL);
  uv_buf_t b = uv_buf_init(strndup(buf, 512), strlen(buf) + 1);
  uv_write_t *w;
  CHECK((w = static_cast<uv_write_t *>(malloc(sizeof(uv_write_t)))) != nullptr);
  w->data = buf;
  uv_write(w, (uv_stream_t *)this->handle, &b, 1, writeCallback);
}
void tipirc::cmdISON(const cmdList2 nicknames[]) {
  char *buf;
  CHECK((buf = static_cast<char *>(calloc(512, sizeof(char)))) != nullptr);
  snprintf(buf, 512,
           "ISON"
           ""
           "\r\n",
           NULL);
  uv_buf_t b = uv_buf_init(strndup(buf, 512), strlen(buf) + 1);
  uv_write_t *w;
  CHECK((w = static_cast<uv_write_t *>(malloc(sizeof(uv_write_t)))) != nullptr);
  w->data = buf;
  uv_write(w, (uv_stream_t *)this->handle, &b, 1, writeCallback);
}

EXPORT int tipinit(int argc, char **argv) {
  google::InitGoogleLogging(argv[0]);
  gflags::SetUsageMessage("Rubbish Tip IRC library");
  gflags::ParseCommandLineFlags(&argc, &argv, true);
  return 0;
}
