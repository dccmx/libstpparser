{
  .name = "not \\r 1",
  .parts = 
  {
    {
      .data = "5\nhello\r\n\r\n",
      .parse_len = sizeof("5\nhello\r\n\r\n") - 1,
    },
    { .data = NULL, }
  },
  .argc = 0,
  .argv = 
  {
  },
  .arglen = 
  {
  },
  .errnum = STNOTR,
  .cnt_message_begin = 1,

  .cnt_argument_len_begin = 1,
  .cnt_argument_len_data = 0,
  .cnt_argument_len_complete = 0,
  .cnt_argument_len = 0,

  .cnt_argument_begin = 0,
  .cnt_argument_data = 0,
  .cnt_argument_complete = 0,

  .cnt_message_complete = 0,
},
