{
  .name = "length too big",
  .parts = 
  {
    {
      .data = "10000000000000000\r\n",
      .parse_len = sizeof("10000000000000000\r\n") - 1,
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
  .errnum = STBIGLEN,
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
