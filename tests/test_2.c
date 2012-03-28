{
  .name = "two arg",
  .parts = 
  {
    {
      .data = "5\r\nhello\r\n1\r\n1\r\n\r\n",
      .parse_len = sizeof("5\r\nhello\r\n1\r\n1\r\n\r\n") - 1,
    },
    { .data = NULL, }
  },
  .argc = 2,
  .argv = 
  {
    "hello",
    "1",
  },
  .arglen = 
  {
    5,
    1,
  },
  .errnum = STOK,
  .cnt_message_begin = 1,

  .cnt_argument_len_begin = 2,
  .cnt_argument_len_data = 2,
  .cnt_argument_len_complete = 2,
  .cnt_argument_len = 2,

  .cnt_argument_begin = 2,
  .cnt_argument_data = 2,
  .cnt_argument_complete = 2,

  .cnt_message_complete = 1,
},
