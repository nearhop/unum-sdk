// (c) 2022 nearhop.com
// unum platform specific include file for wireless

#ifndef _UNUM_WIRELESS_H
#define _UNUM_WIRELESS_H

// Max number of radios we can have for the platform (used for platforms
// where wireless code could be shared between hardare with potentially
// different number of radios, for now iwinfo and wl)
#define WIRELESS_MAX_RADIOS 2

#include "wlioctl.h"
#include "wlutils.h"

typedef unsigned short int chanspec_t;

// Command to get station counters
#define BCM_WL_PATH "/usr/sbin/wl"

// Max VAPs per radio we can report (used in wireless_common.h and
// in some platform specific code)
#define WIRELESS_MAX_VAPS 8

// Number of radio cores (for now only used to get TX power)
#define WIRELESS_RADIO_CORES 4

// Define listing the platform radio ifnames (for wl the first VAP ifnames)
#define PLATFRM_RADIO_IFS {     \
  "eth6", /* 2.4 GHz radio */ \
  "eth7"  /* 5 GHz radio   */ \
}
#define PLATFRM_VAP_IFS { \
  { "eth6", "wl0.1", "wl0.2", "wl0.3" }, \
  { "eth7", "wl1.1", "wl1.2", "wl1.3" }, \
}

// Define listing the platform VAP ifnames for each radio in a 2d array
// initializer form (radios should be in the same order as above)

// Common header shared across all platforms
#include "../wireless_common.h"

// Max time we might potentially block the thread while doing wireless
#include "../wireless_wl.h"


// Max time we might potentially block the thread while doing wireless
// fiunctions calls (used to set watchdog).
#define WIRELESS_MAX_DELAY (60 * 5)

// Per radio counters buffer size
#define WT_COUNTERS_SIZE 3096

#endif // _UNUM_WIRELESS_H
