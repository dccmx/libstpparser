#include "stpparser.h"
#include <string.h>
#include <ctype.h>

enum States {
  StateNone = 0,
  StateInit,
  StateReadLen,
  StateReadLenEOL,
  StateReadArg,
  StateReadArgEOL,
  StateReadMsgCL,
  StateReadMsgRF,
};

void stpparser_init(stpparser *parser) {
  void *data = parser->data; /* preserve application data */
  memset(parser, 0, sizeof(*parser));
  parser->data = data;
  parser->state = StateInit;
}

#define RET return p - data

size_t stpparser_execute(stpparser *parser, const stpparser_settings *settings, const char *data, size_t len) {
  const char *p = data, *at = data;
  enum States last_state, uncommit_state = StateNone;

  while (p < data + len) {
    switch (parser->state) {
      case StateInit:
        if (settings->on_message_begin) settings->on_message_begin(parser);
        uncommit_state = StateNone;
        parser->state = StateReadLen;
        parser->arglen = 0;
        at = p;
        last_state = StateInit;
        break;

      case StateReadLen:
        if (isdigit(*p)) {
          if (last_state != StateReadLen && settings->on_argument_len_begin && settings->on_argument_len_begin(parser)) RET;
          parser->arglen = parser->arglen * 10 + (*p - '0');
          p++;
          uncommit_state = StateReadLen;
        } else if (*p != '\r') { // error
          RET;
        } else {
          if (settings->on_argument_len_data && settings->on_argument_len_data(parser, at, p - at)) RET;
          if (settings->on_argument_len_complete && settings->on_argument_len_complete(parser)) RET;
          if (settings->on_argument_len && settings->on_argument_len(parser, parser->arglen)) RET;
          uncommit_state = StateNone;
          parser->state = StateReadLenEOL;
          p++;
        }
        last_state = StateInit;
        break;

      case StateReadLenEOL:
        if (*p != '\n') { // error
          RET;
        } else {
          parser->state = StateReadArg;
          p++;
          at = p;
        }
        last_state = StateReadLenEOL;
        break;

      case StateReadArg:
        if (last_state != StateReadArg && settings->on_argument_begin && settings->on_argument_begin(parser)) RET;
        if (parser->arglen > 0) {
          parser->arglen--;
          p++;
          uncommit_state = StateReadArg;
          if (parser->arglen == 0) {
            if (settings->on_argument_data && settings->on_argument_data(parser, at, p - at)) RET;
            if (settings->on_argument_complete && settings->on_argument_complete(parser)) RET;
            uncommit_state = StateNone;
          }
        } else if (*p != '\r') { // error
          RET;
        } else {
          parser->state = StateReadArgEOL;
          p++;
          at = p;
        }
        last_state = StateReadArg;
        break;

      case StateReadArgEOL:
        if (*p != '\n') { // error
          RET;
        } else {
          parser->state = StateReadMsgCL;
          p++;
          at = p;
        }
        last_state = StateReadArgEOL;
        break;

      case StateReadMsgCL:
        if (*p != '\r') {
          parser->state = StateReadLen;
        } else {
          if (settings->on_message_complete && settings->on_message_complete(parser)) RET;
          parser->state = StateReadMsgRF;
          p++;
        }
        last_state = StateReadMsgCL;
        break;

      case StateReadMsgRF:
        if (*p != '\n') { // error
          RET;
        } else {
          parser->state = StateInit;
          p++;
        }
        last_state = StateReadMsgRF;
        break;
    }
  }

  switch (uncommit_state) {
    case StateReadLen:
      if (settings->on_argument_len_data && settings->on_argument_len_data(parser, at, p - at)) RET;
      break;
    case StateReadArg:
      if (settings->on_argument_data && settings->on_argument_data(parser, at, p - at)) RET;
      break;
    default:
      break;
  }

  RET;
}
