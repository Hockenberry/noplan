#ifndef __MBOX_H
#define __MBOX_H

#pragma once


#include <stdbool.h>


#define MBOX_REQUEST    0

enum mbox_channels : unsigned int{
  MBOX_CH_POWER   = 0,
  MBOX_CH_FB      = 1,
  MBOX_CH_VUART   = 2,
  MBOX_CH_VCHIQ   = 3,
  MBOX_CH_LEDS    = 4,
  MBOX_CH_BTNS    = 5,
  MBOX_CH_TOUCH   = 6,
  MBOX_CH_COUNT   = 7,
  MBOX_CH_PROP    = 8,
};

/* tags */
#define MBOX_TAG_GETSERIAL      0x10004
#define MBOX_TAG_LAST           0


#define MBOX_REQUEST            0

bool mbox_call(unsigned char ch);
bool mbox_buffer_alloc(unsigned int len, void **result);

#endif
