#ifndef CPUINFO_H_
#define CPUINFO_H_

/**
 * Returns the number of CPU cores available.
 */
int cpu_get_count(void);

/**
 * Returns true if the CPU has SSE2 features.
 */
int cpu_has_sse2(void);

#endif  /* CPUINFO_H_ */
