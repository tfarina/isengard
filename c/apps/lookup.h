#ifndef LOOKUP_H_
#define LOOKUP_H_

/**
 * An user data type that can be used to build a lookup table.
 */
typedef struct {
        int id;
        const char *name;
} lookup_entry_t;

/**
 * Looks up the given id in the lookup table.
 *
 * @param table Lookup table.
 * @param id ID to look up.
 */
lookup_entry_t *
lookup_entry_by_id(lookup_entry_t table[], int id);

#endif  /* LOOKUP_H_ */
