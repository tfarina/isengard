# Database Design Notes

This document contains **design-stage database ideas**.
Nothing here is authoritative or guaranteed to be implemented.

---

## Email Address

Purpose:
- Store one or more email addresses per contact
- Allow a primary email

Proposed table: `EMAIL_ADDRESS`

Fields:
- emailID (INTEGER, PK)
- contactID (INTEGER, FK → contacts.id)
- isPrimary (INTEGER, 0/1)
- orderingIndex (INTEGER)
- label (TEXT)
- address (TEXT)
- createdDateTime (TEXT, ISO 8601, UTC)
- lastModifiedDateTime (TEXT, ISO 8601, UTC)

Open questions:
- Should `address` be UNIQUE per contact?
- Do we need soft deletes?
- Enforce one primary email at DB or app level?

---

## Phone Number

Purpose:
- Store multiple phone numbers per contact
- Allow a primary phone number
- International support

Proposed table: `PHONE_NUMBER`

Fields:
- phoneID (INTEGER, PK)
- contactID (INTEGER, FK → contacts.id)
- isPrimary (INTEGER, 0/1)
- orderingIndex (INTEGER)
- label (TEXT)
- areaCode (TEXT)
- countryCode (TEXT)
- extension (TEXT)
- fullNumber (TEXT)
- createdDateTime (TEXT, ISO 8601, UTC)
- lastModifiedDateTime (TEXT, ISO 8601, UTC)

Notes:
- `fullNumber` may be redundant but useful for search
- Formatting handled in UI, not DB
