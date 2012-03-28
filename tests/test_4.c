{
  .name = "two parts",
  .parts = 
  {
    {
      .data = "4\r\n1234\r\n1\r\n1",
      .parse_len = sizeof("4\r\n1234\r\n1\r\n1") - 1,
    },
    {
      .data = "\r\n\r\n5\r\nhello\r\n1\r\n1\r\n\r\n",
      .parse_len = sizeof("\r\n\r\n5\r\nhello\r\n1\r\n1\r\n\r\n") - 1,
    },
    { .data = NULL, }
  },
  .argc = 4,
  .argv = 
  {
    "1234",
    "1",
    "hello",
    "1",
  },
  .arglen = 
  {
    4,
    1,
    5,
    1,
  },
  .errnum = STOK,
  .cnt_message_begin = 2,

  .cnt_argument_len_begin = 4,
  .cnt_argument_len_data = 4,
  .cnt_argument_len_complete = 4,
  .cnt_argument_len = 4,

  .cnt_argument_begin = 4,
  .cnt_argument_data = 4,
  .cnt_argument_complete = 4,

  .cnt_message_complete = 2,
},

