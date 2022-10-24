#ifndef BITOPS_H_
#define BITOPS_H_

/**
 * These macros work on positions defined by constants.
 *
 * Examples:
 *
 * 1- To set bit 2:
 *
 *    BIT_SET(flags, 0x04)
 */

/**
 * Helper macro for bit manipulation.
 */
#define BIT_MASK(bit) (1U << (bit))

/**
 * Set bit on flags.
 *
 * @param flags the value where bits should be set
 */
#define BIT_SET(flags, mask)   ((flags) |= (mask))

/**
 * Clear bit on flags.
 *
 * @param flags the value where bits should be clear.
 */
#define BIT_CLR(flags, mask)   ((flags) &= ~(mask))

/**
 * Check if bits are set
 *
 * @param flags the value where bits should be checked
 *
 * @return Returns true if any of bits specified in 'mask' is set.
 */
#define BIT_ISSET(flags, mask) ((flags) & (mask))

/**
 * Flip bits
 *
 * Go from '0' to '1' and vice-versa.
 *
 * @param flags the value where bits should flipped.
 */
#define BIT_FLIP(flags, mask) ((flags) ^= (mask))

/* https://www.pixelbeat.org/libs/bitops.h */
/* https://github.com/irrdnet/irrd/blob/03de032c2d1769a6ffb72513a03cec77d9759fc2/src/include/defs.h#L27 */
/* https://github.com/moxuse/ofxSgp/blob/74ccfa7c164226d17e287beb63152a3d84ea4e7d/libs/sgp4sdp4/sgp4sdp4.c */
/* https://github.com/acassen/keepalived/blob/7630e72ba38be27b060a9db4b01a6045ee794c66/lib/bitops.h#L37 */
/* https://people.cs.umu.se/isak/snippets/bitops.c */
/* https://github.com/royratcliffe/ta-lib/blob/272b60afa70625b84ab647fd1e0a13909735280b/branches/ta-lib/ta-lib/c/src/ta_common/imatix/sfl/prelude.h#L429 */
/* https://github.com/signalwire/libks/blob/master/src/include/libks/ks.h#L42 */
/* https://github.com/ventanium/ventanium/blob/master/src/vtm/core/flag.h */
/* https://www.coranac.com/documents/working-with-bits-and-bitfields/ */
/* http://git.savannah.gnu.org/cgit/m4.git/tree/m4/m4private.h#n42 */
/* https://github.com/zerotao/libzt/blob/develop/src/libzt/zt_macros.h#L153*/
/* https://github.com/neutrinolabs/xrdp/blob/master/common/xrdp_constants.h*/

int flags = 0;

void set_flag(int flag) {
  flags |= flag;
}

void clear_flag(int flag) {
  flags &= ~flag;
}

int is_flag_set(int flag) {
  return (flags & flag);
}

int is_flag_clear(int flag) {
  return (~flags & flag);
}

#endif  /* BITOPS_H_ */
