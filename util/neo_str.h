/* 
 * Copyright (C) 2000  Neotonic
 *
 * All Rights Reserved.
 */

#ifndef __NEO_STR_H_
#define __NEO_STR_H_ 1

__BEGIN_DECLS

/* This modifies the string its called with by replacing all the white
 * space on the end with \0, and returns a pointer to the first
 * non-white space character in the string 
 */
char *neos_strip (char *s);

typedef struct _string
{
  char *buf;
  int len;
  int max;
} STRING;

void string_init (STRING *str);
NEOERR *string_append (STRING *str, char *buf);
NEOERR *string_appendn (STRING *str, char *buf, int l);
NEOERR *string_append_char (STRING *str, char c);
void string_clear (STRING *str);

__END_DECLS

#endif /* __NEO_STR_H_ */
