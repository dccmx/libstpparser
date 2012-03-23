#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stpparser.h"

#define MAX_NARG 5
#define MAX_ARG 100

struct message {
  const char *name; // for debugging purposes
  const char *raw;
  int num_args;
  char args[MAX_NARG][MAX_ARG];
  const size_t argslen[MAX_NARG];
};

const struct message messages[] = {
  {
    .name = "one arg",
    .raw = "5\r\nhello\r\n\r\n",
    .num_args = 1,
    .args = 
    {
      "hello",
    },
    .argslen = 
    {
      5,
    }
  },
  {
    .name = "two arg",
    .raw = "5\r\nhello\r\n1\r\n1\r\n\r\n",
    .num_args = 2,
    .args = 
    {
      "hello",
      "1",
    },
    .argslen = 
    {
      5,
      1,
    }
  },
  {
    .name = "two req",
    .raw = "4\r\n1234\r\n1\r\n1\r\n\r\n5\r\nhello\r\n1\r\n1\r\n\r\n",
    .num_args = 2,
    .args = 
    {
      "hello",
      "1",
    },
    .argslen = 
    {
      5,
      1,
    }
  },
  {
    .name = NULL,
  },
};

struct argument {
  int num;
  char args[MAX_NARG][MAX_ARG];
  size_t argslen[MAX_NARG];
  int pos;
};


int message_begin(stpparser* parser) {
  struct argument *arg = (struct argument*)parser->data;
  arg->num = 0;
  arg->pos = 0;
  return 0;
}

int arglen_cb(stpparser* parser, size_t length) {
  struct argument *arg = (struct argument*)parser->data;
  arg->argslen[arg->num] = length;
  return 0;
}

int argument(stpparser* parser, const char *at, size_t length) {
  struct argument *arg = (struct argument*)parser->data;
  memcpy(arg->args[arg->num] + arg->pos, at, length);
  arg->pos += length;
  return 0;
}

int argument_complete(stpparser* parser) {
  struct argument *arg = (struct argument*)parser->data;
  arg->num++;
  arg->pos = 0;
  return 0;
}

int verify(struct argument *arg, const struct message *msg) {
  int i;
  if (arg->num != msg->num_args) {
    printf("\nexpected %d args but got %d\n", msg->num_args, arg->num);
    return 1;
  }
  for (i = 0; i < arg->num; i++) {
    if (arg->argslen[i] != msg->argslen[i]) {
      printf("\nexpected argument #%d length %zu but got %zu\n", i + 1, msg->argslen[i], arg->argslen[i]);
      return 1;
    }
    if (strcmp(arg->args[i], msg->args[i])) {
      printf("\nexpected argument \"%s\" but got \"%s\"\n", msg->args[i], arg->args[i]);
      return 1;
    }
  }
  return 0;
}

int test(const struct message *msg) {
  stpparser parser;
  struct argument arg;
  size_t len = strlen(msg->raw);

  parser.data = &arg;
  stpparser_settings settings;
  settings.on_message_begin = message_begin;
  settings.on_argument_len = arglen_cb;
  settings.on_argument = argument;
  settings.on_argument_complete = argument_complete;

  stpparser_init(&parser);

  printf("TEST (%s)\t\t", msg->name);
  
  size_t plen = stpparser_execute(&parser, &settings, msg->raw, len);
  if ( plen != len) {
    printf("invalid at %zu:%s[FAILED]\n\n", plen, msg->raw + plen);
    return 0;
  }

  if (verify(&arg, msg)) {
    printf("[FAILED]\n\n");
    return 0;
  }

  printf("[OK]\n\n");
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
