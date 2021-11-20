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

#include <string>

#if INTERFACE
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
 private:
#define PROTO(msg) tipCallback On##msg;
  CALLBACKS(PROTO)
#undef PROTO
};

#endif

#define DEFAULT(msg)                                   \
  void defaultOn##msg(void *ctx, tipEvent *e) { \
    LOG(INFO) << "Received message " << #msg;          \
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

EXPORT int tipinit(int argc, char **argv) {
  google::InitGoogleLogging(argv[0]);
  gflags::SetUsageMessage("Rubbish Tip IRC library");
  gflags::ParseCommandLineFlags(&argc, &argv, true);
  return 0;
}
