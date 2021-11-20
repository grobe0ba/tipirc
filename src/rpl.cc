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

#include <string.h>
#include <unistd.h>

#include "rpl.hh"

#if INTERFACE

#define RPLS(X)           \
  X(WELCOME, 001)         \
  X(YOURHOST, 002)        \
  X(CREATED, 003)         \
  X(MYINFO, 004)          \
  X(BOUNCE, 005)          \
  X(USERHOST, 302)        \
  X(ISON, 303)            \
  X(AWAY, 301)            \
  X(UNAWAY, 305)          \
  X(NOWAWAY, 306)         \
  X(WHOISUSER, 311)       \
  X(WHOISSERVER, 312)     \
  X(WHOISOPERATOR, 313)   \
  X(WHOISIDLE, 317)       \
  X(ENDOFWHOIS, 318)      \
  X(WHOISCHANNELS, 319)   \
  X(WHOWASUSER, 314)      \
  X(ENDOFWHOWAS, 369)     \
  X(LISTSTART, 321)       \
  X(LIST, 322)            \
  X(LISTEND, 323)         \
  X(UNIQOPIS, 325)        \
  X(CHANNELMODEIS, 324)   \
  X(NOTOPIC, 331)         \
  X(TOPIC, 332)           \
  X(INVITING, 341)        \
  X(SUMMONING, 342)       \
  X(INVITELIST, 346)      \
  X(ENDOFINVITELIST, 347) \
  X(EXCEPTLIST, 348)      \
  X(ENDOFEXCEPTLIST, 349) \
  X(VERSION, 351)         \
  X(WHOREPLY, 352)        \
  X(ENDOFWHO, 315)        \
  X(NAMREPLY, 353)        \
  X(ENDOFNAMES, 366)      \
  X(LINKS, 364)           \
  X(ENDOFLINKS, 365)      \
  X(BANLIST, 367)         \
  X(ENDOFBANLIST, 368)    \
  X(INFO, 371)            \
  X(ENDOFINFO, 374)       \
  X(MOTDSTART, 375)       \
  X(MOTD, 372)            \
  X(ENDOFMOTD, 376)       \
  X(YOUREOPER, 381)       \
  X(REHASHING, 382)       \
  X(YOURESERVICE, 383)    \
  X(TIME, 391)            \
  X(USERSSTART, 392)      \
  X(USERS, 393)           \
  X(ENDOFUSERS, 394)      \
  X(NOUSERS, 395)         \
  X(TRACELINK, 200)       \
  X(TRACECONNECTING, 201) \
  X(TRACEHANDSHAKE, 202)  \
  X(TRACEUNKNOWN, 203)    \
  X(TRACEOPERATOR, 204)   \
  X(TRACEUSER, 205)       \
  X(TRACESERVER, 206)     \
  X(TRACESERVICE, 207)    \
  X(TRACENEWTYPE, 208)    \
  X(TRACECLASS, 209)      \
  X(TRACERECONNECT, 210)  \
  X(TRACELOG, 261)        \
  X(TRACEEND, 262)

#define ERPLS(X)            \
  X(NOSUCHNICK, 401)        \
  X(NOSUCHSERVER, 402)      \
  X(NOSUCHCHANNEL, 403)     \
  X(CANNOTSENDTOCHAN, 404)  \
  X(TOOMANYCHANNELS, 405)   \
  X(WASNOSUCHNICK, 406)     \
  X(TOOMANYTARGETS, 407)    \
  X(NOSUCHSERVICE, 408)     \
  X(NOORIGIN, 409)          \
  X(NORECIPIENT, 411)       \
  X(NOTEXTTOSEND, 412)      \
  X(NOTOPLEVEL, 413)        \
  X(WILDTOPLEVEL, 414)      \
  X(BADMASK, 415)           \
  X(UNKNOWNCOMMAND, 421)    \
  X(NOMOTD, 422)            \
  X(NOADMININFO, 423)       \
  X(FILEERROR, 424)         \
  X(NONICKNAMEGIVEN, 431)   \
  X(ERRONEUSNICKNAME, 432)  \
  X(NICKNAMEINUSE, 433)     \
  X(NICKCOLLISION, 436)     \
  X(UNAVAILRESOURCE, 437)   \
  X(USERNOTINCHANNEL, 441)  \
  X(NOTONCHANNEL, 442)      \
  X(USERONCHANNEL, 443)     \
  X(NOLOGIN, 444)           \
  X(SUMMONDISABLED, 445)    \
  X(USERSDISABLED, 446)     \
  X(NOTREGISTERED, 451)     \
  X(NEEDMOREPARAMS, 461)    \
  X(ALREADYREGISTERED, 462) \
  X(NOPERMFORHOST, 463)     \
  X(PASSWORDMISMATCH, 464)  \
  X(YOUREBANNEDCREEP, 465)  \
  X(YOUWILLBEBANNED, 466)   \
  X(KEYSET, 467)            \
  X(CHANNELISFULL, 471)     \
  X(UNKNOWNMODE, 472)       \
  X(INVITEONLYCHAN, 473)    \
  X(BANNEDFROMCHAN, 474)    \
  X(BADCHANNELKEY, 475)     \
  X(BADCHANMASK, 476)       \
  X(NOCHANMODES, 477)       \
  X(BANLISTFULL, 478)       \
  X(NOPRIVILEGES, 481)      \
  X(CHANOPRIVSNEEDED, 482)  \
  X(CANTKILLSERVER, 483)    \
  X(RESTRICTED, 484)        \
  X(UNIQOPRIVSNEEDED, 485)  \
  X(NOOPERHOST, 491)        \
  X(UMODEUNKNOWNFLAG, 501)  \
  X(USERSDONTMATCH, 502)

typedef struct {
  const char *name;
  const char *val;
  const int ival;
} sRPL;

typedef enum {
  RPL_BADRPL = -1,
#define ENUM(rpl, val) RPL_##rpl,
  RPLS(ENUM)
#undef ENUM
#define ENUM(rpl, val) ERR_##rpl,
      ERPLS(ENUM)
#undef ENUM
          RPL_MAX
} RPL;

#endif

const sRPL sRPLlist[] = {
#define sRPL(rpl, xval) (sRPL){.name = #rpl, .val = #xval, .ival = xval},
    RPLS(sRPL) ERPLS(sRPL)
#undef sRPL
};

const sRPL RPL2sRPL(RPL repl) {
  if (repl > RPL_BADRPL && repl < RPL_MAX) {
    return sRPLlist[repl];
  }
  return (sRPL){NULL, NULL, RPL_BADRPL};
}

const char *RPL2string(RPL repl) {
  if (repl > RPL_BADRPL && repl < RPL_MAX) {
#define R2s(rpl, val) \
  if (repl == RPL_##rpl) return sRPLlist[RPL_##rpl].name;
    RPLS(R2s)
#undef R2s
#define R2s(rpl, val) \
  if (repl == ERR_##rpl) return sRPLlist[ERR_##rpl].name;
    ERPLS(R2s)
#undef R2s
  }
  return NULL;
}

const int string2RPL(char *repl) {
#define s2R(rpl, val) \
  if (strncmp(repl, sRPLlist[RPL_##rpl].name, 16) == 0) return RPL_##rpl;
  RPLS(s2R)
#undef s2R
#define s2R(rpl, val) \
  if (strncmp(repl, sRPLlist[ERR_##rpl].name, 16) == 0) return ERR_##rpl;
  ERPLS(s2R)
#undef s2R
  return RPL_BADRPL;
}

const RPL valstring2RPL(char *repl) {
#define s2R(rpl, xval) \
  if (strncmp(repl, sRPLlist[RPL_##rpl].val, 3) == 0) return RPL_##rpl;
  RPLS(s2R)
#undef s2R
#define s2R(rpl, xval) \
  if (strncmp(repl, sRPLlist[ERR_##rpl].val, 3) == 0) return ERR_##rpl;
  ERPLS(s2R)
#undef s2R
  return RPL_BADRPL;
}
