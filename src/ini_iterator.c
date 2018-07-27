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
   
   For more information, please refer to <http://unlicense.org/>*/ 

#ifdef __cplusplus
extern "C" {
#endif

#include "INI/ini.h"

inline static void _INI_iter_New(INI_iter** it)
{
  if (!(*it))
    {
      *it = (INI_iter*) malloc(sizeof(INI_iter));
      (*it)->alloc_type = _INI_ALLOC_TYPE_API;
    }
  else
    (*it)->alloc_type = _INI_ALLOC_TYPE_EXTERN;

  (*it)->indexing_pos = 0u;
  (*it)->pos = 0u;
}

#define _INI_FP_TYPE_HANDLE  0x01
#define _INI_FP_TYPE_SECTION 0x02

INI_iter* INI_iter_FromHandle(INI_iter* it, INI* handle)
{
  _INI_iter_New(&it);
  it->fp = (void*) handle;
  it->fp_type = _INI_FP_TYPE_HANDLE;
  return it;
}

INI_iter* INI_iter_FromSection(INI_iter* it, INI_section* section)
{
  _INI_iter_New(&it);
  it->fp = (void*) section;
  it->fp_type = _INI_FP_TYPE_HANDLE;
  return it;
}

void INI_iter_Free(INI_iter* it)
{
  if (it->alloc_type) free(it); // Free if memory management done by the API 
}

INI_section* INI_iter_NextSection(INI_iter* it)
{
  if (it->indexing_pos == 257)
    return NULL;

  if (it->indexing_pos == 0)
    {
      // Global section
      it->indexing_pos++;
      return INI_GetSection((INI*) it->fp, NULL);
    }
  // Everything else
  // Search a index which contains sections
  while (it->indexing_pos < 257
         && !(((INI*) it->fp)->sections[it->indexing_pos]))
    ++(it->indexing_pos);
  // Chancel if out of bounds
  if (it->indexing_pos == 257)
    return NULL;
  // If selected section is NULL (meaning end of array)
  if (!((INI*) it->fp)->sections[it->indexing_pos][it->pos])
    {
      // Reset pos and increment indexing_pos
      it->pos = 0u;
      ++it->indexing_pos;
      return INI_iter_NextSection(it);
    }
  // Seems to exist; Return and increment pos
  return ((INI*) it->fp)->sections[it->indexing_pos][it->pos++];
}

INI_pair* INI_iter_NextPair(INI_iter* it)
{
  if (it->indexing_pos == 256)
    return NULL;
  // Search for non-empty indexing_pos
  while (it->indexing_pos < 256
         && !(((INI_section*) it->fp)->pairs[it->indexing_pos]))
    ++(it->indexing_pos);
  // Return NULL if out of bounds (nothing found)
  if (it->indexing_pos == 256)
    return NULL;

  if (!((INI_section*) it->fp)->pairs[it->indexing_pos][it->pos])
    {
      // Reset pos and increment indexing_pos
      it->pos = 0u;
      ++(it->indexing_pos);
      return INI_iter_NextPair(it);
    }
  // Seems to exist; Return and increment pos
  return ((INI_section*) it->fp)->pairs[it->indexing_pos][it->pos++];
}

#ifdef __cplusplus
}
#endif
