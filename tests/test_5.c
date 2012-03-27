{
  .name = "two parts 2",
  .parts = 
  {
    {
      .data = "1\r\n",
      .parse_len = sizeof("1\r\n") - 1,
    },
    {
      .data = "1\r\n",
      .parse_len = sizeof("1\r\n") - 1,
    },
    { .data = NULL, }
  },
  .argc = 1,
  .argv = 
  {
    "1",
  },
  .arglen = 
  {
    1,
  },
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


