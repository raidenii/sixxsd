/***********************************************************
 SixXSd - The Daemon of SixXS
 by Jeroen Massar <jeroen@sixxs.net>
 (C) Copyright SixXS 2000-2011 All Rights Reserved
************************************************************
 $Author: $
 $Id: $
 $Date: $
***********************************************************/

#ifndef STATS_H
#define STATS_H "SixXSd Statistics"

#include "sixxsd.h"

#define stats_ipv4 0
#define stats_ipv6 1

#define stats_in 0
#define stats_out 1

/* Statistics */
struct sixxsd_traffic
{
	uint64_t			last;				/* Last time packet in this direction was seen */
	uint64_t			packets;			/* Number of packets seen */
	uint64_t			octets;				/* Number of octets seen */
};

struct sixxsd_latency
{
	uint16_t			seq, _padding_;			/* Sequence number */
	uint16_t			num_sent, num_recv;		/* Number sent & received */
	uint64_t			min, max, tot;			/* Minimum, Max and Total latency */
};

struct sixxsd_stats
{
	struct sixxsd_traffic		traffic[2][2];			/* Traffic: IPv4/IPv6 + in/out */
	struct sixxsd_latency		latency[2];			/* Latency: IPv4/IPv6 */
};

#define reset_latency(l)		\
	{				\
		(l)->min = -1;		\
		(l)->max = 0;		\
		(l)->tot = 0;		\
		(l)->num_sent = 0;	\
		(l)->num_recv = 0;	\
	}

#endif /* STATS_H */

