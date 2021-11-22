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

#include <fmt/core.h>

#include <string>
#include <vector>

#if INTERFACE

#define CMDS(X)                                                       \
  X(PASS, "{}", (password))                                           \
  X(NICK, "{}", (nickname))                                           \
  X(USER, "{} {} * {}", (user)(mode)(realname))                       \
  X(OPER, "{} {}", (name)(password))                                  \
  X(SERVICE, "{} * {} {} * {}", (nickname)(distribution)(type)(info)) \
  X(QUIT, "{}", (message))                                            \
  X(SQUIT, "{} {}", (server)(comment))                                \
  X(TOPIC, "{} {}", (channel)(topic))                                 \
  X(INVITE, "{} {}", (nickname)(channel))                             \
  X(PRIVMSG, "{} {}", (target)(message))                              \
  X(NOTICE, "{} {}", (target)(text))                                  \
  X(MOTD, "{}", (target))                                             \
  X(LUSERS, "{} {}", (mask)(target))                                  \
  X(VERSION, "{}", (target))                                          \
  X(STATS, "{} {}", (query)(target))                                  \
  X(LINKS, "{} {}", (remote_server)(server_mask))                     \
  X(TIME, "{}", (target))                                             \
  X(CONNECT, "{} {} {}", (target_server)(port)(remote_server))        \
  X(TRACE, "{}", (target))                                            \
  X(ADMIN, "{}", (target))                                            \
  X(INFO, "{}", (target))                                             \
  X(SERVLIST, "{} {}", (mask)(type))                                  \
  X(SQUERY, "{} {}", (servicename)(text))                             \
  X(WHO, "{}", (mask))                                                \
  X(KILL, "{} {}", (nickname)(comment))                               \
  X(PING, "{} {}", (server1)(server2))                                \
  X(PONG, "{} {}", (server1)(server2))                                \
  X(ERROR, "{}", (error_message))                                     \
  X(AWAY, "{}", (text))                                               \
  X(SUMMON, "{} {} {}", (user)(target)(channel))                      \
  X(USERS, "{}", (target))                                            \
  X(WALLOPS, "{}", (text))

#define LCMDS(X)                                                     \
  X(MODE, "", std::string target, std::vector<std::string> modes,    \
    std::vector<std::string> modeparams)                             \
  X(JOIN, ~, std::vector<std::string> channels)                      \
  X(PART, ~, std::vector<std::string> channels, std::string message) \
  X(NAMES, ~, std::vector<std::string> channels, std::string target) \
  X(LIST, ~, std::vector<std::string> channels, std::string target)  \
  X(KICK, ~, std::vector<std::string> channels,                      \
    std::vector<std::string> users, std::string comment)             \
  X(REHASH, ~, ~)                                                    \
  X(DIE, ~, ~)                                                       \
  X(RESTART, ~, ~)                                                   \
  X(WHOIS, ~, std::string target, std::vector<std::string> masks)    \
  X(WHOWAS, ~, std::vector<std::string> nicknames, int count,        \
    std::string target)                                              \
  X(USERHOST, ~, std::vector<std::string> nicknames)                 \
  X(ISON, ~, std::vector<std::string> nicknames)

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
