{
  .name = "0 len arg",
  .parts = 
  {
    {
      .data = "0\r\n\r\n\r\n",
      .parse_len = sizeof("0\r\n\r\n\r\n") - 1,
    },
    { .data = NULL, }
  },
  .argc = 1,
  .argv = 
  {
    "",
  },
  .arglen = 
  {
    0,
  },
  .errnum = STOK,
  .cnt_message_begin = 1,

  .cnt_argument_len_begin = 1,
  .cnt_argument_len_data = 1,
  .cnt_argument_len_complete = 1,
  .cnt_argument_len = 1,

  .cnt_argument_begin = 1,
  .cnt_argument_data = 1,
  .cnt_argument_complete = 1,

  .cnt_message_complete = 1,
},
