#ifndef CPU_H_
#define CPU_H_

#if defined(__i686__) || defined(__i586__) || defined(__i486__) || defined(__i386__) || defined(__i386) || defined(_M_IX86) || defined(_X86_) || defined(__THW_INTEL__)
#define CPU_ARCH_X86
#endif

/**
 * Returns the number of CPUs currently online in the system.
 */
int cpu_get_count(void);

/**
 * Returns true if the CPU has SSE2 features.
 */
int cpu_has_sse2(void);

#endif  /* CPU_H_ */
