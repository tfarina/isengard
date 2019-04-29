#ifndef BITOPS_H_
#define BITOPS_H_

/**
 * Macro functions for bit manipulation.
 */

#define BIT_MASK(bit) (1 << (bit))

/**
 * These macros work on positions defined by constants.
 *
 * Examples:
 *
 * 1- To set bit 2:
 *
 *    BIT_SET(a, 0x04) 
 */
#define BIT_SET(a, mask)   ((a) |= (mask))
#define BIT_CLR(a, mask)   ((a) &= ~(mask))

/**
 * Returns true if any of bits specified in 'mask' is set.
 */
#define BIT_ISSET(a, mask) ((a) & (mask))

/* https://www.pixelbeat.org/libs/bitops.h */
/* https://github.com/irrdnet/irrd/blob/03de032c2d1769a6ffb72513a03cec77d9759fc2/src/include/defs.h#L27 */
/* https://github.com/moxuse/ofxSgp/blob/74ccfa7c164226d17e287beb63152a3d84ea4e7d/libs/sgp4sdp4/sgp4sdp4.c */
/* https://github.com/acassen/keepalived/blob/7630e72ba38be27b060a9db4b01a6045ee794c66/lib/bitops.h#L37 */
/* https://people.cs.umu.se/isak/snippets/bitops.c */

#endif  /* BITOPS_H_ */
