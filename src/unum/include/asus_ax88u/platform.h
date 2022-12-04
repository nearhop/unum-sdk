// (c) 2022 nearhop.com
// Platform specific include file.
// The main purpose is to pull in all the platform specific headers.

#ifndef _PALTFORM_H
#define _PALTFORM_H

// Use openssl for random number generation
// under util/dns.c
#define DNS_RANDOM RAND_bytes

#endif // _PALTFORM_H

