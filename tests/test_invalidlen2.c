{
  .name = "invalid length 2",
  .parts = 
  {
    {
      .data = "5\r\nhello\r\n\n",
      .parse_len = sizeof("5\r\nhello\r\n\n") - 1,
    },
    { .data = NULL, }
  },
  .argc = 1,
  .argv = 
  {
    "hello",
  },
  .arglen = 
  {
    5,
  },
  .errnum = STBADLEN,
  .cnt_message_begin = 1,

  .cnt_argument_len_begin = 1,
  .cnt_argument_len_data = 1,
  .cnt_argument_len_complete = 1,
  .cnt_argument_len = 1,

  .cnt_argument_begin = 1,
  .cnt_argument_data = 1,
  .cnt_argument_complete = 1,

  .cnt_message_complete = 0,
},
