#ifndef STP_PARSER_H_
#define STP_PARSER_H_
#ifdef __cplusplus
extern "C" {
#endif

#include <sys/types.h>
#include <stdint.h>

#define STOK 0
#define STBIGLEN 1
#define STBADLEN 2
#define STNOTR 3
#define STNOTN 4

typedef struct stpparser stpparser;
typedef struct stpparser_settings stpparser_settings;

typedef int (*data_callback) (stpparser* parser, const char *at, int length);
typedef int (*size_callback) (stpparser* parser, int length);
typedef int (*callback) (stpparser* parser);

struct stpparser {
  char state;
  char last_state;
  int arglen;
  void *data;
  int errnum;
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

int stpparser_execute(stpparser *parser, const stpparser_settings *settings, const char *data, int len);

const char *stpparser_strerror(int errnum);

#ifdef __cplusplus
}
#endif
#endif // STP_PARSER_H_
