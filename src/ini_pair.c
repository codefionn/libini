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

INI_pair* INI_pair_New(INI_pair* pair,
                          const char * key, const char * value)
{
  if (!pair)
    {
      pair = (INI_pair*) malloc(sizeof(INI_pair));
      pair->alloc_type = _INI_ALLOC_TYPE_API;
    }
  else
    pair->alloc_type = _INI_ALLOC_TYPE_EXTERN;

  if (!key)
    pair->key = NULL;
  else
    {
      pair->key = (char*) malloc((strlen(key) + 1) * sizeof(char));
      strcpy(pair->key, key);
      pair->key[strlen(key)] = '\0';
    }

  if (!value)
    pair->value = NULL;
  else
    {
      pair->value = (char*) malloc((strlen(value) + 1) * sizeof(char));
      strcpy(pair->value, value);
      pair->value[strlen(value)] = '\0';
    }
   
  return pair;
}

void INI_pair_Free(INI_pair* pair)
{
  if (pair->key)
    free(pair->key);
  if (pair->value)
    free(pair->value);
  if (pair->alloc_type) free(pair);
}

void INI_pair_SetKey(INI_pair* pair, const char * key)
{
  if (pair->key)
    free(pair->key);
  
  pair->key = (char*) malloc((strlen(key) + 1) * sizeof(char));
  strcpy(pair->key, key);
  pair->key[strlen(key)] = '\0';
}

void INI_pair_SetValue(INI_pair* pair, const char * value)
{
  if (pair->value)
    free(pair->value);
  
  pair->value = (char*) malloc((strlen(value) + 1) * sizeof(char));
  strcpy(pair->value, value);
  pair->value[strlen(value)] = '\0';
}

#ifdef __cplusplus
}
#endif
