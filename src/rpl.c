#include "rpl.h"

#include <string.h>
#include <unistd.h>

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

#if INTERFACE
typedef struct {
  const char *name;
  const char *val;
  const int ival;
} sRPL;
typedef enum XeRPL eRPL;
#endif

#define ENUM(rpl, val) RPL_##rpl,
typedef enum XeRPL { RPL_BADRPL = -1, RPLS(ENUM) RPL_MAX } eRPL;
#undef ENUM

#define sRPL(rpl, xval) (sRPL){.name = #rpl, .val = #xval, .ival = xval},
const sRPL sRPLlist[] = {RPLS(sRPL)};

const sRPL eRPL2sRPL(eRPL repl) {
  if (repl > RPL_BADRPL && repl < RPL_MAX) {
    return sRPLlist[repl];
  }
  return (sRPL){NULL, NULL, RPL_BADRPL};
}

const char *RPL2string(eRPL repl) {
  if (repl > RPL_BADRPL && repl < RPL_MAX) {
#define R2s(rpl, val) \
  if (repl == RPL_##rpl) return sRPLlist[RPL_##rpl].name;
    RPLS(R2s)
#undef R2s
  }
  return NULL;
}

const int string2RPL(char *repl) {
#define s2R(rpl, val) \
  if (strncmp(repl, sRPLlist[RPL_##rpl].name, 16) == 0) return RPL_##rpl;
  RPLS(s2R)
#undef s2R
  return RPL_BADRPL;
}

const eRPL valstring2RPL(char *repl) {
#define s2R(rpl, xval) \
  if (strncmp(repl, sRPLlist[RPL_##rpl].val, 3) == 0) return RPL_##rpl;
  RPLS(s2R)
#undef s2R
  return RPL_BADRPL;
}
