#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stpparser.h"

#define MAX_NARG 5
#define MAX_PART 5
#define MAX_ARG 100

struct part {
  const char *data;
  size_t parse_len;
};

struct message {
  const char *name; // for debugging purposes
  struct part parts[MAX_PART];
  int argc;
  char argv[MAX_NARG][MAX_ARG];
  const size_t arglen[MAX_NARG];

  int errnum;

  int cnt_message_begin;

  int cnt_argument_len_begin;
  int cnt_argument_len_data;
  int cnt_argument_len_complete;
  int cnt_argument_len;

  int cnt_argument_begin;
  int cnt_argument_data;
  int cnt_argument_complete;

  int cnt_message_complete;
};

const struct message messages[] = {
#include "test_0.c"
#include "test_1.c"
#include "test_2.c"
#include "test_3.c"
#include "test_4.c"
#include "test_invalidlen.c"
#include "test_invalidlen2.c"
#include "test_emptylen.c"
#include "test_biglen.c"
#include "test_notr1.c"
#include "test_notr2.c"
  { .name = NULL, },
};

struct result {
  int argc;
  char argv[MAX_NARG][MAX_ARG];
  size_t arglen[MAX_NARG];
  int errnum;

  int cnt_message_begin;

  int cnt_argument_len_begin;
  int cnt_argument_len_data;
  int cnt_argument_len_complete;
  int cnt_argument_len;

  int cnt_argument_begin;
  int cnt_argument_data;
  int cnt_argument_complete;

  int cnt_message_complete;

  int pos;
};

int message_begin(stpparser* parser) {
  struct result *res = (struct result*)parser->data;
  res->cnt_message_begin++;
  return 0;
}

int argument_len_begin(stpparser* parser) {
  struct result *res = (struct result*)parser->data;
  res->cnt_argument_len_begin++;
  return 0;
}

int argument_len_data(stpparser* parser, const char *at, int length) {
  struct result *res = (struct result*)parser->data;
  at = at;
  length = length;
  res->cnt_argument_len_data++;
  return 0;
}

int argument_len_complete(stpparser* parser) {
  struct result *res = (struct result*)parser->data;
  res->cnt_argument_len_complete++;
  return 0;
}

int argument_len(stpparser* parser, int length) {
  struct result *res = (struct result*)parser->data;
  res->cnt_argument_len++;
  res->arglen[res->argc] = length;
  return 0;
}

int argument_begin(stpparser* parser) {
  struct result *res = (struct result*)parser->data;
  res->cnt_argument_begin++;
  return 0;
}

int argument_data(stpparser* parser, const char *at, int length) {
  struct result *res = (struct result*)parser->data;
  res->cnt_argument_data++;
  memcpy(res->argv[res->argc] + res->pos, at, length);
  res->pos += length;
  return 0;
}

int argument_complete(stpparser* parser) {
  struct result *res = (struct result*)parser->data;
  res->cnt_argument_complete++;
  res->argc++;
  res->pos = 0;
  return 0;
}

int message_complete(stpparser* parser) {
  struct result *res = (struct result*)parser->data;
  res->cnt_message_complete++;
  return 0;
}

int verify(struct result *res, const struct message *msg) {
  int i;
  // verify callback
  if (res->cnt_message_begin != msg->cnt_message_begin) {
    printf("\nexpected %d cnt_message_begin callback but got %d\n", msg->cnt_message_begin, res->cnt_message_begin);
    return 1;
  }
  if (res->cnt_argument_len_begin != msg->cnt_argument_len_begin) {
    printf("\nexpected %d cnt_argument_len_begin callback but got %d\n", msg->cnt_argument_len_begin, res->cnt_argument_len_begin);
    return 1;
  }
  if (res->cnt_argument_len_data != msg->cnt_argument_len_data) {
    printf("\nexpected %d cnt_argument_len_data callback but got %d\n", msg->cnt_argument_len_data, res->cnt_argument_len_data);
    return 1;
  }
  if (res->cnt_argument_len_complete != msg->cnt_argument_len_complete) {
    printf("\nexpected %d cnt_argument_len_complete callback but got %d\n", msg->cnt_argument_len_complete, res->cnt_argument_len_complete);
    return 1;
  }
  if (res->cnt_argument_len != msg->cnt_argument_len) {
    printf("\nexpected %d cnt_argument_len callback but got %d\n", msg->cnt_argument_len, res->cnt_argument_len);
    return 1;
  }
  if (res->cnt_argument_begin != msg->cnt_argument_begin) {
    printf("\nexpected %d cnt_argument_begin callback but got %d\n", msg->cnt_argument_begin, res->cnt_argument_begin);
    return 1;
  }
  if (res->cnt_argument_data != msg->cnt_argument_data) {
    printf("\nexpected %d cnt_argument_data callback but got %d\n", msg->cnt_argument_data, res->cnt_argument_data);
    return 1;
  }
  if (res->cnt_argument_complete != msg->cnt_argument_complete) {
    printf("\nexpected %d cnt_argument_complete callback but got %d\n", msg->cnt_argument_complete, res->cnt_argument_complete);
    return 1;
  }
  if (res->cnt_message_complete != msg->cnt_message_complete) {
    printf("\nexpected %d cnt_message_complete callback but got %d\n", msg->cnt_message_complete, res->cnt_message_complete);
    return 1;
  }
  // verify argc
  if (res->argc != msg->argc) {
    printf("\nexpected %d args but got %d\n", msg->argc, res->argc);
    return 1;
  }

  // verify errnum
  if (res->errnum != msg->errnum) {
      printf("\nexpected errnum %d but got %d\n", msg->errnum, res->errnum);
      return 1;
  }

  // verify argv
  for (i = 0; i < res->argc; i++) {
    if (res->arglen[i] != msg->arglen[i]) {
      printf("\nexpected argument #%d length %zu but got %zu\n", i + 1, msg->arglen[i], res->arglen[i]);
      return 1;
    }
    if (strcmp(res->argv[i], msg->argv[i])) {
      printf("\nexpected argument \"%s\" but got \"%s\"\n", msg->argv[i], res->argv[i]);
      return 1;
    }
  }
  return 0;
}

int test(const struct message *msg) {
  stpparser parser;
  struct result res;
  int i;

  memset(&res, 0, sizeof(res));
  parser.data = &res;
  stpparser_settings settings;
  settings.on_message_begin = message_begin;

  settings.on_argument_len_begin = argument_len_begin;
  settings.on_argument_len_data = argument_len_data;
  settings.on_argument_len_complete = argument_len_complete;
  settings.on_argument_len = argument_len;

  settings.on_argument_begin = argument_begin;
  settings.on_argument_data = argument_data;
  settings.on_argument_complete = argument_complete;

  settings.on_message_complete = message_complete;

  stpparser_init(&parser);

  printf("TEST %-40s", msg->name);
  
  for (i = 0; msg->parts[i].data != NULL; i++) {
    size_t plen = stpparser_execute(&parser, &settings, msg->parts[i].data, strlen(msg->parts[i].data));
    if (plen != msg->parts[i].parse_len && msg->errnum == 0) {
      printf("invalid at pos %zu of msg \"%s\" [FAILED]\n\n", plen, msg->parts[i].data);
      return 0;
    }
    res.errnum = parser.errnum;
  }

  if (verify(&res, msg)) {
    printf("[FAILED]\n");
    return 0;
  }

  printf("[OK]\n");
  return 1;
}

int main() {
  int i, total_msg;
  for (total_msg = 0; messages[total_msg].name; total_msg++);

  for (i = 0; i < total_msg; i++) {
    test(&messages[i]);
  }
  return 0;
}
