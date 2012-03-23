#ifndef STP_PARSER_H_
#define STP_PARSER_H_
#ifdef __cplusplus
extern "C" {
#endif

#include <sys/types.h>
#include <stdint.h>

typedef struct stpparser stpparser;
typedef struct stpparser_settings stpparser_settings;

typedef int (*stp_data_cb) (stpparser* parser, const char *at, size_t length);
typedef int (*stp_arglen_cb) (stpparser* parser, size_t length);
typedef int (*stp_cb) (stpparser* parser);

struct stpparser {
  char state;
  size_t arglen;
  void *data;
};

struct stpparser_settings {
  stp_cb      on_message_begin;
  stp_data_cb on_argument_len_data;
  stp_arglen_cb on_argument_len;
  stp_data_cb on_argument;
  stp_cb on_argument_complete;
  stp_cb      on_message_complete;
};

void stpparser_init(stpparser *parser);

size_t stpparser_execute(stpparser *parser, const stpparser_settings *settings, const char *data, size_t len);

#ifdef __cplusplus
}
#endif
#endif // STP_PARSER_H_
