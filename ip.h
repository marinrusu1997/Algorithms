#ifndef _IP_UTILS_HEADER_
#define _IP_UTILS_HEADER_

#include <stdio.h>
#include <inttypes.h>
#include <limits.h>
#include <math.h>
#include <assert.h>

#define IPV4_MAX_STR_LEN	19
#define NIBBLE_LEN			4

/*
		Little-Endian
		   higher memory
			  ----->
		+----+----+----+----+
	MSB	|0x04|0x03|0x02|0x01| LSB
		+----+----+----+----+
		A
		|
	   &x
*/
/*
		Big-Endian
			higher memory
			  ----->
		+----+----+----+----+
	MSB	|0x01|0x02|0x03|0x04| LSB
		+----+----+----+----+
		A
		|
	   &x
*/
#define REVERSE_1BYTE(value)	(((value & 0xF0) >> 4) | ((value & 0x0F) << 4))

#define REVERSE_2BYTES(value)	(((value & 0x00FF) << 8) | ((value & 0xFF00) >> 8))

#define REVERSE_4BYTES(value)	(((value & 0x000000FF) << 24) | \
								((value & 0x0000FF00) << 8) | \
								((value & 0x00FF0000) >> 8) | \
								((value & 0xFF000000) >> 24))

/*
	@param ip	IPv4 Address in network byte format
	@param mask IPv4 Mask in network byte format
*/
#define NETWORK_ADDRESS(ip, mask) ip & mask

/*
	@param naddr	IPv4 Network address in network byte format
	@param mask		IPv4 Mask in network byte format
*/
#define BROADCAST_ADDRESS(naddr, mask) ~(naddr ^ mask)

enum IP_CLASS { A, B, C };

uint32_t host_part(uint32_t mask, enum IP_CLASS class)
{
	switch (class)
	{
	case A:
		return mask & 0xffffff00;
	case B:
		return mask & 0xffff0000;
	case C:
		return mask & 0xff000000;
	default:
		return 0;
	}
}

static uint8_t number_to_bits[16] = { 0, 1, 1, 2, 1, 2, 2, 3,
									1, 2, 2, 3, 2, 3, 3, 4 };
/*
	@param n	Number to count set bits
	@ret		Number of bits set
	@impl		Look-up Table
*/
uint8_t count_bits_set_lookup(uint32_t n)
{
	return n == 0 ? number_to_bits[0] : number_to_bits[n & 0xf] +
		count_bits_set_lookup(n >> NIBBLE_LEN);
}

/*
	@param x	Number to count set bits
	@ret		Number of bits set
	@impl		Kernigan Substract Method
*/
uint8_t count_bits_set_substract(uint32_t x)
{
	uint8_t c = 0;
	for (; x; c++) // acumulate in c
		x &= x - 1; // clear the least significant bit set
	return c;
}

/*
	@param mask	IPv4 Mask in network byte order
	@param clas	IPv4 Address class
	@ret		Number of bits unset in host part
*/
uint8_t bits_unset_host_part(uint32_t mask, enum IP_CLASS clas)
{
	switch (clas)
	{
	case A:
		return 24 - count_bits_set_lookup(host_part(mask, clas));
	case B:
		return 16 - count_bits_set_lookup(host_part(mask, clas));
	case C:
		return 8 - count_bits_set_lookup(host_part(mask, clas));
	default:
		return 0;
	}
}

/*
	@param mask		IPv4 Mask in network byte format
	@param class	IPv4 Address class
*/
#define NUMBER_OF_SUBNETS(mask, clas) pow(2, count_bits_set_substract(host_part((mask), (clas))))

/*
	@param mask		IPv4 Mask in network byte format
	@param class	IPv4 Address class
*/
#define NUMBER_OF_VALID_HOSTS_PER_SUBNET(mask,clas) pow(2, bits_unset_host_part((mask),(clas))) - 2

/*
	@param	addr IPv4 Address in format x.x.x.x where x is an octet
	@ret	IPv4 Address as a integer in network byte format(Big Endian)
*/
uint32_t ipv4_p2n(const uint8_t* addr)
{
	assert(addr != NULL);
	uint32_t ip;
	for (uint8_t *octet = strtok(addr, "."), index = NULL; octet; octet = strtok(NULL, "."))
		((uint8_t*)&ip)[index++] = atoi(octet);
	return ip;
}


/*
	@param buf	Buffer which has at least 16 bytes length
	@param ip	IPv4 Address as a integer in network byte format
	@ret		0 on succes, -1 on failure
*/
int8_t ipv4_n2p(char *buf, const uint32_t ip)
{
	assert(buf != NULL);
	return sprintf(buf, "%d.%d.%d.%d",
		((uint8_t*)&ip)[0], ((uint8_t*)&ip)[1], ((uint8_t*)&ip)[2], ((uint8_t*)&ip)[3])
		<= IPV4_MAX_STR_LEN ? 0 : -1;
}

/*
	@param CIDR	Number of bits turned on based on CIDR notation
	@ret		Mask in network byte format
*/
uint32_t mask_for(const char CIDR)
{
	uint32_t mask = 0;
	for (char i = 0; i < CIDR; i++)
		mask |= 0x80000000 >> i;
	return REVERSE_4BYTES(mask);
}

#endif // _IP_UTILS_HEADER_
