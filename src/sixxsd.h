/***********************************************************
 SixXSd - The Daemon of SixXS
 by Jeroen Massar <jeroen@sixxs.net>
 (C) Copyright SixXS 2000-2011 All Rights Reserved
************************************************************
 $Author: $
 $Id: $
 $Date: $
***********************************************************/

#ifndef SIXXSD_H
#define SIXXSD_H "The Daemon Of SixXS"

/* How much is a clock allowed to be off? */
#define MAX_CLOCK_OFF 120

/* How many tunnels does a single sixxsd support? */
#define SIXXSD_TUNNELS_MAX 8192

#include "platform.h"

/* List Code */
#include "list.h"

/* Read/Write Lock */
#include "rwl.h"

/* Used almost everywhere thus define it like this */
struct sixxsd_context;

#include "common_extra.h"

/* Context */
#include "context.h"

/* Hashes */
#include "hash_sha1.h"
#include "hash_md5.h"

/* Checksum */
#include "checksum.h"

/* Stats */
#include "stats.h"

/* Subnets */
#include "subnet.h"

/* Tunnels */
#include "tunnel.h"

/* Config */
#include "config.h"

/* Interfaces */
#include "iface.h"

/* Common code */
#include "common.h"

/* Thread Management */
#include "thread.h"

/* sixxsd.c */
VOID terminate(const char *who);

/* Protocols */
#include "ayiya.h"
#include "proto41.h"
#include "hb.h"

BOOL l3_ipv6_parse(const uint8_t *packet, const uint32_t len, uint8_t *_ipe_type, struct ip6_ext **_ipe, uint32_t *_plen);

#endif /* SIXXSD_H */

