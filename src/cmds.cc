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

#define CMDS(X)                                                       \
  X(PASS, "%s", (password))                                           \
  X(NICK, "%s", (nickname))                                           \
  X(USER, "%s %s * %s", (user)(mode)(realname))                       \
  X(OPER, "%s %s", (name)(password))                                  \
  X(SERVICE, "%s * %s %s * %s", (nickname)(distribution)(type)(info)) \
  X(QUIT, "%s", (message))                                            \
  X(SQUIT, "%s %s", (server)(comment))                                \
  X(TOPIC, "%s %s", (channel)(topic))                                 \
  X(INVITE, "%s %s", (nickname)(channel))                             \
  X(PRIVMSG, "%s %s", (target)(message))                              \
  X(NOTICE, "%s %s", (target)(text))                                  \
  X(MOTD, "%s", (target))                                             \
  X(LUSERS, "%s %s", (mask)(target))                                  \
  X(VERSION, "%s", (target))                                          \
  X(STATS, "%s %s", (query)(target))                                  \
  X(LINKS, "%s %s", (remote_server)(server_mask))                     \
  X(TIME, "%s", (target))                                             \
  X(CONNECT, "%s %s %s", (target_server)(port)(remote_server))        \
  X(TRACE, "%s", (target))                                            \
  X(ADMIN, "%s", (target))                                            \
  X(INFO, "%s", (target))                                             \
  X(SERVLIST, "%s %s", (mask)(type))                                  \
  X(SQUERY, "%s %s", (servicename)(text))                             \
  X(WHO, "%s", (mask))                                                \
  X(KILL, "%s %s", (nickname)(comment))                               \
  X(PING, "%s %s", (server1)(server2))                                \
  X(PONG, "%s %s", (server1)(server2))                                \
  X(ERROR, "%s", (error_message))                                     \
  X(AWAY, "%s", (text))                                               \
  X(SUMMON, "%s %s %s", (user)(target)(channel))                      \
  X(USERS, "%s", (target))                                            \
  X(WALLOPS, "%s", (text))

#define LCMDS(X)                                                               \
  X(MODE, "", const char *target, const char **modes, const char **modeparams) \
  X(JOIN, ~, const cmdList2 channels[])                                        \
  X(PART, ~, const cmdList2 channels[], const char *message)                   \
  X(NAMES, ~, const cmdList2 channels[], target)                               \
  X(LIST, ~, const cmdList2 channels[], target)                                \
  X(KICK, ~, const cmdList2 channels[], const cmdList2 users[],                \
    const char *comment)                                                       \
  X(REHASH, ~, ~)                                                              \
  X(DIE, ~, ~)                                                                 \
  X(RESTART, ~, ~)                                                             \
  X(WHOIS, ~, const char *target, const cmdList2 masks[])                      \
  X(WHOWAS, ~, const cmdList2 nicknames[], int count, const char *target)      \
  X(USERHOST, ~, const cmdList2 nicknames[])                                   \
  X(ISON, ~, const cmdList2 nicknames[])

typedef struct {
  const char *name;
  const char *value;
} cmdList2;

typedef enum {
  CMD_BADCMD = -1,
#define ENUM(cmd, fmt, args...) CMD_##cmd,
  CMDS(ENUM) LCMDS(ENUM)
#undef ENUM
      CMD_MAX
} CMD;

#endif

#define TXT(cmd, fmt, args...) #cmd,
const char *cmdStrings[] = {CMDS(TXT) LCMDS(TXT)};
#undef TXT

EXPORT const char *CMD2string(CMD cmd) {
  if (cmd > CMD_BADCMD && cmd < CMD_MAX) {
#define C2s(c, fmt, a...) \
  if (cmd == CMD_##c) return cmdStrings[CMD_##c];
    CMDS(C2s)
    LCMDS(C2s)
#undef C2s
  }
  return NULL;
}

EXPORT const CMD string2CMD(const char *cmd) {
#define s2C(c, fmt, a...) \
  if (strncmp(cmd, cmdStrings[CMD_##c], 8) == 0) return CMD_##c;
  CMDS(s2C)
  LCMDS(s2C)
#undef s2C
  return CMD_BADCMD;
}

// Use this to generate empty function block copypasta
// #define PROTO(com, args...) \
//   void tipirc::cmd##com(args) {}
// CMDS(PROTO)
