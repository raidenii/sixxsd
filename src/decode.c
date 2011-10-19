/***********************************************************
 SixXSd - The Daemon of SixXS
 by Jeroen Massar <jeroen@sixxs.net>
 (C) Copyright SixXS 2000-2011 All Rights Reserved
************************************************************
 $Author: $
 $Id: $
 $Date: $

 Packet decoder
***********************************************************/

#include "sixxsd.h"

const char module_decode[] = "decode";
#define module module_decode

BOOL l3_ipv6_parse(const uint8_t *packet, const uint32_t len, uint8_t *_ipe_type, struct ip6_ext **_ipe, uint32_t *_plen)
{
	struct ip6_hdr		*iph = (struct ip6_hdr *)packet;
	struct ip6_ext		*ipe;
	uint8_t			ipe_type;
	uint32_t		plen, l;

	if (len < sizeof(*iph))
	{
		mdolog(LOG_WARNING, "IPv6: Short IPv6 packet received of len %u\n", len);
		return false;
	}

	if ((iph->ip6_ctlun.ip6_un2_vfc>>4) != 6)
	{
		mdolog(LOG_WARNING, "IPv6: Corrupt IP version %u packet found\n", (iph->ip6_ctlun.ip6_un2_vfc>>4));
		return false;
	}

	/* Save the type of the next header */
	ipe_type = iph->ip6_nxt;
	/* Step to the next header */
	ipe = (struct ip6_ext *)(((char *)iph) + sizeof(*iph));
	plen = ntohs(iph->ip6_plen);

	/*
	 * Skip the hopbyhop options that we know.
	 * The problem is that for unknown
	 * hopbyhop options we don't know
	 * if there is a ipe->ip6e_nxt field
	 * in the following header
	 */
	while (	ipe_type == IPPROTO_HOPOPTS ||
		ipe_type == IPPROTO_ROUTING ||
		ipe_type == IPPROTO_DSTOPTS ||
		ipe_type == IPPROTO_AH)
	{
		/* Save the type of the next header */
		ipe_type = ipe->ip6e_nxt;

		/* Step to the next header */
		l = ((ipe->ip6e_len*8)+8);
		if (l > plen)
		{
			mdolog(LOG_WARNING, "CORRUPT: Extension (%u) Longer than left-over payload (%u)\n", l, plen);
		}

		plen -= l;
		ipe  = (struct ip6_ext *)(((char *)ipe) + l);

		/* Check for corrupt packets */
		if ((char *)ipe > ((char *)(iph) + len))
		{
			mdolog(LOG_WARNING, "CORRUPT: Header chain beyond packet data\n");
			/* Send ICMPv6 Parameter Problem - Header */
			iface_send_icmpv6_unreach(SIXXSD_TUNNEL_UPLINK, SIXXSD_TUNNEL_NONE, packet, len, ICMP6_PARAMPROB_HEADER);
			return false;
		}

		if (ipe_type == IPPROTO_ROUTING)
		{
			struct ip6_rthdr *rt = (struct ip6_rthdr *)ipe;
			if (rt->ip6r_type == 0)
			{
				mdolog(LOG_WARNING, "RH0 packet encountered\n");
				/* Send ICMP Admin disabled */
				iface_send_icmpv6_unreach(SIXXSD_TUNNEL_UPLINK, SIXXSD_TUNNEL_NONE, packet, len, ICMP6_PARAMPROB_HEADER);
				return false;
			}
		}

	}

	/* All okay */
	if (_ipe_type)	*_ipe_type = ipe_type;
	if (_ipe)	*_ipe = ipe;
	if (_plen)	*_plen = plen;

	return true;
}
