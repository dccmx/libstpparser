#include "stpparser.h"
#include <string.h>
#include <ctype.h>

enum States {
  StateInit = 0,
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
  const char *p = data, *at;

  while (p < data + len) {
    switch (parser->state) {
      case StateInit:
        if (settings->on_message_begin) settings->on_message_begin(parser);
        parser->state = StateReadLen;
        parser->arglen = 0;
        at = p;
        break;

      case StateReadLen:
        if (isdigit(*p)) {
          parser->arglen = parser->arglen * 10 + (*p - '0');
          p++;
        } else if (*p != '\r') { // error
          RET;
        } else {
          parser->state = StateReadLenEOL;
          p++;
        }
        break;

      case StateReadLenEOL:
        if (*p != '\n') { // error
          RET;
        } else {
          parser->state = StateReadArg;
          if (settings->on_argument_len_data && settings->on_argument_len_data(parser, at, p - at - 2)) RET;
          if (settings->on_argument_len && settings->on_argument_len(parser, parser->arglen)) RET;
          p++;
          at = p;
        }
        break;

      case StateReadArg:
        if (parser->arglen > 0) {
          parser->arglen--;
          p++;
        } else if (*p != '\r') { // error
          RET;
        } else {
          if (settings->on_argument && settings->on_argument(parser, at, p - at)) RET;
          if (settings->on_argument_complete && settings->on_argument_complete(parser)) RET;
          parser->state = StateReadArgEOL;
          p++;
          at = p;
        }
        break;

      case StateReadArgEOL:
        if (*p != '\n') { // error
          RET;
        } else {
          parser->state = StateReadMsgCL;
          p++;
          at = p;
        }
        break;

      case StateReadMsgCL:
        if (*p != '\r') {
          parser->state = StateReadLen;
        } else {
          parser->state = StateReadMsgRF;
          p++;
        }
        break;

      case StateReadMsgRF:
        if (*p != '\n') { // error
          RET;
        } else {
          if (settings->on_message_complete && settings->on_message_complete(parser)) RET;
          parser->state = StateInit;
          p++;
        }
        break;
    }
  }

  RET;
}
