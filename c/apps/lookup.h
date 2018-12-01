#ifndef LOOKUP_H_
#define LOOKUP_H_

/**
 * An user data type that can be used to build a lookup table.
 *
 * Lookup tables are arrays of (id, name) pairs.
 */
typedef struct {
        int id;
        const char *name;
} lookup_entry_t;

/**
 * Looks up the given id in the lookup table.
 *
 * @param table The lookup table to search in.
 * @param id What to search for.
 * @return The item found or NULL if not found.
 */
lookup_entry_t *
lookup_entry_by_id(lookup_entry_t table[], int id);

#endif  /* LOOKUP_H_ */
