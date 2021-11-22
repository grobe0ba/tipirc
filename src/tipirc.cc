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

#include <fmt/core.h>
#include <gflags/gflags.h>
#include <glog/logging.h>
#include <uv.h>

#include <boost/preprocessor.hpp>
#include <string>
#include <vector>

#if INTERFACE
#include <uv.h>

#include <boost/preprocessor.hpp>
#include <string>

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
#define ADDTYPE(r, d, x) (std::string x)
#define PROTO(com, fmt, args...) \
  void cmd##com(BOOST_PP_SEQ_ENUM(BOOST_PP_SEQ_FOR_EACH(ADDTYPE, ~, args)));
  CMDS(PROTO)
#undef PROTO
#undef ADDTYPE
  void cmdMODE(std::string target, std::vector<std::string> modes,
               std::vector<std::string> modeparams);
  void cmdJOIN(std::vector<std::string> channels);

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
  free(r);
}
#ifdef __cplusplus
}
#endif

#define NONE(...)
#define ADDTYPE(r, d, x) (std::string x)
#define HANDLEMSG(com, fmtstr, args...)                                       \
  void tipirc::cmd##com(                                                      \
      BOOST_PP_SEQ_ENUM(BOOST_PP_SEQ_FOR_EACH(ADDTYPE, ~, args))) {           \
    std::string sbuf = fmt::format(fmtstr, BOOST_PP_SEQ_ENUM(args));          \
    const char *buf = sbuf.c_str();                                                 \
    CHECK((buf = static_cast<char *>(calloc(WBUFSZ, sizeof(char)))) != NULL); \
    uv_buf_t b = uv_buf_init(strndup(buf, WBUFSZ), strlen(buf) + 1);          \
    uv_write_t *w;                                                            \
    CHECK((w = static_cast<uv_write_t *>(malloc(sizeof(uv_write_t)))) !=      \
          NULL);                                                              \
    uv_write(w, (uv_stream_t *)this->handle, &b, 1, writeCallback);           \
  }
CMDS(HANDLEMSG)
#undef HANDLEMSG
#undef CHECKNULL
#undef ADDTYPE
