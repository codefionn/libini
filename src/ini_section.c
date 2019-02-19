/* 
   This is free and unencumbered software released into the public domain.
   
   Anyone is free to copy, modify, publish, use, compile, sell, or
   distribute this software, either in source code form or as a compiled
   binary, for any purpose, commercial or non-commercial, and by any
   means.
   
   In jurisdictions that recognize copyright laws, the author or authors
   of this software dedicate any and all copyright interest in the
   software to the public domain. We make this dedication for the benefit
   of the public at large and to the detriment of our heirs and
   successors. We intend this dedication to be an overt act of
   relinquishment in perpetuity of all present and future rights to this
   software under copyright law.
   
   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
   IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
   OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
   ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
   OTHER DEALINGS IN THE SOFTWARE.
   
   For more information, please refer to <http://unlicense.org/> */

#ifdef __cplusplus
extern "C" {
#endif

#include "INI/ini.h"

INI_section* INI_section_New(INI_section* conf,
                             const char * name)
{
  // Allocated conf, if NULL
  if (!conf)
    {
      conf = (INI_section*) malloc(sizeof(INI_section));
      conf->alloc_type = _INI_ALLOC_TYPE_API;
    }
  else
    conf->alloc_type = _INI_ALLOC_TYPE_EXTERN;  
  // Iterate through all exiting pair arrays and set them to NULL
  for (size_t i = 0; i <= 255; ++i)
    {
      conf->pairs[i] = NULL;
    }
  // Copy name (to conf->name) if not NULL.
  if (!name)
    conf->name = (char*) NULL;
  else
    {
      conf->name = (char*) malloc((strlen(name) + 1) * sizeof(char));
      strcpy(conf->name, name);
      conf->name[strlen(name)] = '\0';
    }

  return conf;
}

void INI_section_Free(INI_section* conf)
{
  if (conf->name)
    free(conf->name);
  // Free all allocated pairs if exists
  for (size_t i = 0; i <= 255; ++i)
    {
      INI_pair* * pairs = conf->pairs[i];
      if (!pairs)
        continue;
      // Iterate through pairs till NULL
      for (size_t ii = 0; pairs[ii]; ++ii)
        {
          // Dereference array (current iterator position)
          INI_pair_Free(pairs[ii]);
        }
      // Free array if exists
      free((void*) conf->pairs[i]);
    }

  if (conf->alloc_type) free ((void*) conf);
}

INI_pair* INI_section_Get(INI_section* conf, const char * key,
                          bool ignore_case)
{
	size_t index = (size_t) _INI_strhash(key, ignore_case);
  INI_pair ** pairs = conf->pairs[index];
  if (!pairs)
    return NULL;

  for (size_t i = 0; pairs[i]; ++i)
    {
      if (!ignore_case)
        {
          if (strcmp(pairs[i]->key, key) == 0)
            {
              // Right pair found. Return pair.
              return pairs[i];
            }
        }
      else
        {
          if (_INI_strcasecmp(pairs[i]->key, key))
            {
              // Right pair found. Return pair.
              return pairs[i];
            }
        }
    }
  // Nothing found: return NULL.
  return NULL;
}

const char * INI_section_GetString(INI_section* conf, const char * key,
                                   bool ignore_case)
{
  INI_pair* pair = INI_section_Get(conf, key, ignore_case);
  if (!pair)
    return NULL;

  return pair->value;
}

bool INI_section_GetBool(INI_section* conf,
                         bool defaultValue, const char * key,
                         bool* exists, bool ignore_case)
{
  const char * value = INI_section_GetString(conf, key, ignore_case);
  if (!value)
    {
      // Set exists to false if not null.
      if (exists)
        *exists = false;
      return defaultValue;
    }

  if (strcmp(value, "1") == 0 || strcmp(value, "True") == 0) 
    return true;
  // Set exists to true, if not null.
  if (exists)
    *exists = true;
  return false;
}

int INI_section_GetInt(INI_section* conf,
                           int defaultValue, const char * key,
                           bool* exists, bool ignore_case)
{
  const char * value = INI_section_GetString(conf, key, ignore_case);
  if (!value)
    {
      // Set exists to false if not null.
      if (exists)
        *exists = false;
      return defaultValue;
    }
  // Set exists to true if not null.
  if (exists)
    *exists = true;
  
  return atoi(value);
}

float INI_section_GetFloat(INI_section* conf,
                           float defaultValue, const char * key,
                           bool* exists, bool ignore_case)
{
  const char * value = INI_section_GetString(conf, key, ignore_case);
  if (!value)
    {
      // Set exists to false if not null.
      if (exists)
        *exists = false;
      return defaultValue;
    }
  // Set exists to true if not null.
  if (exists)
    *exists = true;
  
  return strtof(value, NULL);
}

void INI_section_SetString(INI_section* conf,
                           const char * key, const char * value,
                           bool ignore_case)
{
  INI_pair* pair = INI_section_Get(conf, key, ignore_case);
  if (!pair)
    {
      INI_section_AddString(conf, key, value, ignore_case);
      return;
    }

  INI_pair_SetValue(pair, value);
}

bool INI_section_AddString(INI_section* conf,
                           const char * key, const char * value,
                           bool ignore_case)
{
  // Check if already exists. If exists, return false.
  if (INI_section_Get(conf, key, ignore_case))
    return false;
  // Triple-pointer because we may need to allocate the pointer
  // (created for more convinience)
	size_t index = (size_t) _INI_strhash(key, ignore_case);
  INI_pair *** pairs = &conf->pairs[index];
  if (!(*pairs))
    {
      *pairs = (INI_pair**) malloc(sizeof(void*) * 2);
      (*pairs)[0] = INI_pair_New(NULL, key, value);
      (*pairs)[1] = NULL; // NULL terminated array
      return true;
    }
  // Determine length of pairs array
  size_t length = 0;
  for (; (*pairs)[length]; ++length);
  // Reallocate the pairs array
  *pairs = (INI_pair**) realloc(*pairs, sizeof(void*) * (length+2));
  // Assign terminating null pointer
  (*pairs)[length+1] = NULL;
  // Add new pair object
  (*pairs)[length] = INI_pair_New(NULL, key, value);

  return true;
}

#ifdef __cplusplus
}
#endif
