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

INI* INI_New(INI* handler)
{
  if (!handler)
    {
      handler = (INI*) malloc(sizeof(INI));
      handler->alloc_type = _INI_ALLOC_TYPE_API;
    }
  else
    handler->alloc_type = _INI_ALLOC_TYPE_EXTERN;
  // No flags
  handler->flags = INI_FLAGS_DEFAULT;
  
  for (size_t i = 1; i <= 256; ++i)
    {
      handler->sections[i] = NULL;
    }
  // Allocate the global section
  (handler->sections[0]) = (INI_section* *) malloc(sizeof(void*) * 2);
  if (!handler->sections[0])
    {
#ifndef NDEBUG
      printf("%s:%d: Error: %s", __FILE__, __LINE__,
             "Out of memory");
#endif /* NDEBUG */
      return NULL;
    }
  (handler->sections[0])[0] = INI_section_New(NULL, NULL);
  (handler->sections[0])[1] = NULL; // NULL terminated array

  return handler;
}

void INI_Free(INI* handler)
{
  for (size_t i = 0; i <= 256; ++i)
    {
      // Free all allocated sections in array i
      INI_section* * sections = handler->sections[i];
      if (!sections)
        continue;
      // Iterate through pairs till NULL
      for (size_t ii = 0; sections[ii]; ++ ii)
        {
          INI_section_Free(sections[ii]);
        }

      free((void*) handler->sections[i]);
    }

  if (handler->alloc_type) free((void*) handler);
}

bool INI_GetFlag(INI* handler, int flag)
{
  return handler->flags & flag;
}

void INI_SetFlag(INI* handler, int flag, bool status)
{
  // Bitwise magic
  if (status)
    handler->flags |= flag;
  else
    handler->flags &= ~flag;
}

INI_section* INI_GetSection(INI* handler, const char * section)
{
  // section == NULL ? : That's the global section
  if (!section)
    return (handler->sections[0])[0];
  
  bool ignore_case = INI_GetFlag(handler, INI_FLAG_IGNORE_CASE);
  // + 1 because of GLOBAL == 0
  INI_section* * sections = handler->sections[((size_t)
    (ignore_case ? tolower(section[0]) : section[0])) + 1];
  if (!sections)
    return NULL;
  // Search for section
  for (size_t i = 0; sections[i]; ++i)
    {
      if (ignore_case)
        {
          if (_INI_strcasecmp(sections[i]->name, section))
            return sections[i];
        }
      else
        {
          if (strcmp(sections[i]->name, section) == 0)
            return sections[i];
        }
    }
  // Nothing found. Return NULL.
  return NULL;
}

INI_pair* INI_Get(INI* handler,
                        const char * section, const char * key)
{
  INI_section* sec = INI_GetSection(handler, section);
  if (!sec)
    return NULL;

  return INI_section_Get(sec, key, INI_GetFlag(handler, INI_FLAG_IGNORE_CASE));
}

const char * INI_GetString(INI* handler,
                     const char * section, const char * key)
{
  INI_section* sec = INI_GetSection(handler, section);
  if (!sec)
    return NULL;

  return INI_section_GetString(sec, key, INI_GetFlag(handler, INI_FLAG_IGNORE_CASE));
}

bool INI_GetBool(INI* handler, bool defaultValue,
                 const char * section, const char * key,
                 bool* exists)
{
  INI_section* sec = INI_GetSection(handler, section);
  if (!sec)
    {
      if (exists)
        *exists = false;
      return false;
    }

  return INI_section_GetBool(sec, defaultValue, key, exists,
    INI_GetFlag(handler, INI_FLAG_IGNORE_CASE));
}

int INI_GetInt(INI* handler, int defaultValue,
               const char * section, const char * key,
               bool* exists)
{
  INI_section* sec = INI_GetSection(handler, section);
  if (!sec)
    {
      if (exists)
        *exists = false;
      return false;
    }

  return INI_section_GetInt(sec, defaultValue, key, exists,
    INI_GetFlag(handler, INI_FLAG_IGNORE_CASE));
}

float INI_GetFloat(INI* handler, float defaultValue,
                   const char * section, const char * key,
                   bool* exists)
{
  INI_section* sec = INI_GetSection(handler, section);
  if (!sec)
    {
      if (exists)
        *exists = false;
      return false;
    }

  return INI_section_GetFloat(sec, defaultValue, key, exists,
    INI_GetFlag(handler, INI_FLAG_IGNORE_CASE));
}

bool INI_AddSection(INI* handler, INI_section* conf)
{
  // Check if section already exists
  if (INI_GetSection(handler, conf->name))
    {
      return false;
    }

  bool ignore_case = INI_GetFlag(handler, INI_FLAG_IGNORE_CASE);
  // Exists, create new one (create new array, or append to array)
  // One pointer more for convinience
  INI_section** * sections = &handler->sections[((size_t)
    (ignore_case ? tolower(conf->name[0]) : conf->name[0])) + 1];
  if (!(*sections))
    {
      *sections = (INI_section**) malloc(sizeof(void*) * 2);
      (*sections)[0] = conf;
      (*sections)[1] = NULL; // NULL-terminated array
    }
  else
    {
      // Determine length
      size_t length = 0;
      for (; (*sections)[length] ; ++length);
      // + 2 length because the array should be 1 unit longer than before.
      // The other + 1 is there because NULL was not counted
      *sections = (INI_section**) realloc(*sections,
                                          sizeof(void*) * (length + 2));
      (*sections)[length+1] = NULL;
      (*sections)[length] = conf;
    }

  return true;
}

bool INI_SetString(INI* handler,
                   const char * section,
                   const char * key, const char * value)
{
  INI_section* sec = INI_GetSection(handler, section);
  if (!sec)
    return false;

  INI_section_SetString(sec, key, value,
    INI_GetFlag(handler, INI_FLAG_IGNORE_CASE));
  return true;
}

bool INI_AddString(INI* handler,
                   const char * section,
                   const char * key, const char * value)
{
  INI_section* sec = INI_GetSection(handler, section);
  if (!sec)
    return false;

  return INI_section_AddString(sec, key, value,
    INI_GetFlag(handler, INI_FLAG_IGNORE_CASE)); 
}

#ifdef __cplusplus
}
#endif
