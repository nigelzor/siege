/**
 * Variable evaluation
 *
 * Copyright (C) 2003-2014 by
 * Jeffrey Fulmer - <jeff@joedog.org>, et al. 
 * This file is part of siege
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *--
 *
 */
#ifdef  HAVE_CONFIG_H
# include <config.h>
#endif/*HAVE_CONFIG_H*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <eval.h>
#include <hash.h>
#include <joedog/joedog.h>

char *
evaluate(HASH hash_table, char *buf)
{
  int  x   = 0;
  const char *ptr;
  char *lookup;
  char *replacement;
  char *result;
  const char *scan;
  const char *start;
  
  start = scan = strchr(buf, '$');
  if (scan == NULL) {
	  return buf;
  }
  scan++;
 
  if(scan[0] == '{' || scan[0] == '(')
    scan++;
 
  ptr = scan;
  
  while(*scan && *scan != '}' && *scan != ')' && *scan != '/'){
    scan++;
    x++;
  }
 
  if(scan[0] == '}' || scan[0] == ')')
    scan++;
 
  lookup = substring(ptr, 0, x);
  replacement = hash_get(hash_table, lookup);
  if (replacement == NULL) {
	  replacement = getenv(lookup);
  }
  if (replacement == NULL) {
    replacement = ""; /* user botched his config file */
  }
 
  result = xcalloc((start - buf) + strlen(replacement) + strlen(scan) + 1, 1);
  strncpy(result, buf, start - buf);
  strcat(result, replacement);
  strcat(result, scan);
  
  xfree(buf);
  xfree(lookup);
  return result;
}
 
