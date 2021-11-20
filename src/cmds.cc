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

#include "cmds.hh"

#include <string.h>
#include <unistd.h>

#if INTERFACE

#define CMDS(X)                                                                \
  X(PASS, const char *password)                                                \
  X(NICK, const char *nickname)                                                \
  X(USER, const char *user, const char *mode, const char *realname)            \
  X(OPER, const char *name, const char *password)                              \
  X(SERVICE, const char *nickname, const char *distribution, const char *type, \
    const char *info)                                                          \
  X(QUIT, const char *message)                                                 \
  X(SQUIT, const char *server, const char *comment)                            \
  X(JOIN, const cmdList2 channels[])                                           \
  X(PART, const cmdList2 channels[], const char *message)                      \
  X(MODE, const char *channel, const char **modes, const char **modeparams)    \
  X(TOPIC, const char *channel, const char *topic)                             \
  X(NAMES, const cmdList2 channels[], const char *target)                      \
  X(LIST, const cmdList2 channels[], const char *target)                       \
  X(INVITE, const char *nickname, const char *channel)                         \
  X(KICK, const cmdList2 channels[], const cmdList2 users[], char *comment)    \
  X(PRIVMSG, const char *target, const char *message)                          \
  X(NOTICE, const char *target, const char *text)                              \
  X(MOTD, const char *target)                                                  \
  X(LUSERS, const char *mask, const char *target)                              \
  X(VERSION, const char *target)                                               \
  X(STATS, const char *query, const char *target)                              \
  X(LINKS, const char *remote_server, const char *server_mask)                 \
  X(TIME, const char *target)                                                  \
  X(CONNECT, const char *target_server, int port, const char *remote_server)   \
  X(TRACE, const char *target)                                                 \
  X(ADMIN, const char *target)                                                 \
  X(INFO, const char *target)                                                  \
  X(SERVLIST, const char *mask, const char *type)                              \
  X(SQUERY, const char *servicename, const char *text)                         \
  X(WHO, const char *mask)                                                     \
  X(WHOIS, const char *target, const cmdList2 masks[])                         \
  X(WHOWAS, const cmdList2 nicknames[], int count, const char *target)         \
  X(KILL, const char *nickname, const char *comment)                           \
  X(PING, const char *server1, const char *server2)                            \
  X(PONG, const char *server1, const char *server2)                            \
  X(ERROR, const char *error_message)                                          \
  X(AWAY, const char *text)                                                    \
  X(REHASH)                                                                    \
  X(DIE)                                                                       \
  X(RESTART)                                                                   \
  X(SUMMON, const char *user, const char *target, const char *channel)         \
  X(USERS, const char *target)                                                 \
  X(WALLOPS, const char *text)                                                 \
  X(USERHOST, const cmdList2 nicknames[])                                      \
  X(ISON, const cmdList2 nicknames[])

typedef struct {
  const char *name;
  const char *value;
} cmdList2;

typedef enum {
  CMD_BADCMD = -1,
#define ENUM(cmd, args...) CMD_##cmd,
  CMDS(ENUM)
#undef ENUM
      CMD_MAX
} CMD;

#endif

#define TXT(cmd, args...) #cmd,
const char *cmdStrings[] = {CMDS(TXT)};
#undef TXT

EXPORT const char *CMD2string(CMD cmd) {
  if (cmd > CMD_BADCMD && cmd < CMD_MAX) {
#define C2s(c, a...) \
  if (cmd == CMD_##c) return cmdStrings[CMD_##c];
    CMDS(C2s)
#undef C2s
  }
  return NULL;
}

EXPORT const CMD string2CMD(const char *cmd) {
#define s2C(c, a...) \
  if (strncmp(cmd, cmdStrings[CMD_##c], 8) == 0) return CMD_##c;
  CMDS(s2C)
#undef s2C
  return CMD_BADCMD;
}

// Use this to generate empty function block copypasta
// #define PROTO(com, args...) \
//   void tipirc::cmd##com(args) {}
// CMDS(PROTO)
