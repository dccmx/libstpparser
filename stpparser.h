#ifndef STP_PARSER_H_
#define STP_PARSER_H_
#ifdef __cplusplus
extern "C" {
#endif

#include <sys/types.h>
#include <stdint.h>

typedef struct stpparser stpparser;
typedef struct stpparser_settings stpparser_settings;

typedef int (*data_callback) (stpparser* parser, const char *at, size_t length);
typedef int (*size_callback) (stpparser* parser, size_t length);
typedef int (*callback) (stpparser* parser);

struct stpparser {
  char state;
  size_t arglen;
  void *data;
};

struct stpparser_settings {
  callback        on_message_begin;

  callback        on_argument_len_begin;
  data_callback   on_argument_len_data;
  callback        on_argument_len_complete;
  size_callback   on_argument_len;

  callback        on_argument_begin;
  data_callback   on_argument_data;
  callback        on_argument_complete;

  callback        on_message_complete;
};

void stpparser_init(stpparser *parser);

size_t stpparser_execute(stpparser *parser, const stpparser_settings *settings, const char *data, size_t len);

#ifdef __cplusplus
}
#endif
#endif // STP_PARSER_H_
