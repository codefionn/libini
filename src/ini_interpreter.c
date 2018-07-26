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

/* Read */

#define _INI_INTERPRETER_SECTION 256
#define _INI_INTERPRETER_KEY     257
#define _INI_INTERPRETER_VALUE   258
#define _INI_INTERPRETER_COMMENT 259
#define _INI_INTERPRETER_NONE    260

#define _INI_AERR if (INI_GetFlag(handle, INI_FLAG_ERROR))
#define _INI_ERROR(X) _INI_AERR fprintf(stderr, "%s:%d: Error: %s\n", __FILE__, __LINE__, X)

inline static int _INI_ReadString(INI* handle,
                                  INI_PFN_READ readfn,
                                  void* userdata,
                                  int last_token,
                                  char ** value,
                                  char * should_not_come,
                                  int* c)
{
  size_t pos = 0u;
  size_t length = 1024u;
  *value = (char*) malloc(length * sizeof(char));
  if (!(*value))
    {
      _INI_ERROR("Out of memory.");
      return INI_READ_ERROR_MEM;
    }
  (*value)[pos] = '\0';

  char cs[2];
  cs[0] = (char) *c;
  cs[1] = '\0';
  while (strcspn(cs, should_not_come) && *c <= 255 && *c >= 0)
    {
      if (pos + 1 >= length)
        {
          length += 1024u;
          *value = (char*) realloc((void*) *value, length * sizeof(char));
          if (!(*value))
            {
              _INI_ERROR("Out of memory");
              return INI_READ_ERROR_MEM;
            }
        }

      (*value)[pos++] = (char) *c;
      (*value)[pos] = '\0';
      *c = readfn(userdata);
      cs[0] = (char) *c;
    }
  

  // trim C-string
  *value = (char*) realloc((void*) *value, (pos+1) * sizeof(char));
  return 0;
}

inline static int _INI_Read_Interpreter(INI* handle,
                                         INI_PFN_READ readfn,
                                         void* userdata,
                                         int last_token,
                                         char ** value)
{
  int c;
  while ((c = readfn(userdata)) != EOF
         && c <= 255
         && (((c == ' ' || c == '\t')
              && INI_GetFlag(handle, INI_FLAG_TRIM_SPACES))));

  if (c == EOF || c >= 256)
    return c;
  
  if (c == '\n' || c == '\r')
    return c;

  if (last_token == _INI_INTERPRETER_KEY)
    {
      // Read value
      int error_status = 0;
      if ((error_status = _INI_ReadString(handle, readfn, userdata,
                                          last_token, value,
                                          "\n\r", &c)) != 0)
        return error_status;

      if (c != '\n' && c != '\r' && c != EOF)
        {
          _INI_ERROR("Invalid character!");
          return INI_READ_ERROR_FORMAT;
        }

      // Check if several characters
      if (!strlen(*value))
        {
          // No character
          _INI_ERROR("Requires several characters");
          return INI_READ_ERROR_FORMAT;
        }
      return _INI_INTERPRETER_VALUE;
    }

  if (c == '[')
    {
      while ((c = readfn(userdata)) != EOF
         && c <= 255
         && (((c == ' ' || c == '\t')
              && INI_GetFlag(handle, INI_FLAG_TRIM_SPACES))));

      int error_status = 0;
      if ((error_status = _INI_ReadString(handle, readfn, userdata,
                                          last_token, value,
                                          "\n\r[]=", &c)) != 0)
        return error_status;

      // Check if several characters
      if (!strlen(*value))
        {
          // No character
          _INI_ERROR("Requires several characters");
          return INI_READ_ERROR_FORMAT;
        }

      // Closing brackets
      if (c != ']')
        {
          // Invalid character
          _INI_ERROR("Character ']' should have come");
          return INI_READ_ERROR_FORMAT;
        }

      // Read till end of line
      while ((c = readfn(userdata)) != EOF
         && c <= 255
         && (c == ' ' || c == '\t' || c == '\r'));
      
      if (c != '\r' && c != '\n' && c != EOF)
        {
          _INI_ERROR("A new line should have come or EOF"); 
          printf("Character: %d\n", (int) c);
          return INI_READ_ERROR_FORMAT;
        }

      // Return section token
      return _INI_INTERPRETER_SECTION;
    }
  
  if (c == ';')
    {
      while ((c = readfn(userdata)) != '\n'
             && c != '\r' && c != EOF && c <= 255);
      // Check for error
      if (c >= 400)
        {
          _INI_ERROR("Failed to read from buffer");
          return c;
        }

      return _INI_INTERPRETER_COMMENT;
    }


  if (c >= INI_READ_ERROR_FILE && c <= INI_READ_ERROR_MAX)
    {
      _INI_ERROR("Failed to read from buffer");
      return c;
    }

  if (c == EOF)
    return EOF;

  // Read Key
  int error_status = 0;
  if ((error_status = _INI_ReadString(handle, readfn, userdata,
                                      last_token, value,
                                      "\n\r=", &c)) != 0)
    return error_status;
  // Check if several characters
  if (!strlen(*value))
    {
      free((void*) *value);
      *value = NULL;
      // No character
      return _INI_INTERPRETER_NONE;
    }
  // Key has to end with '='
  if (c != '=')
    {
      _INI_ERROR("Waited for the character '='. But it did not come.");
      return INI_READ_ERROR_FORMAT;
    }


  return _INI_INTERPRETER_KEY;
}

inline static void _INI_str_trim(char ** value)
{
  size_t length = strlen(*value);
  if (!length)
    return;

  length -= 1;
  while (length && ((*value)[length] == ' ' || (*value)[length] == '\t'))
    length--;

  *value = (char*) realloc((void*) *value, (length+2) * sizeof(char));
  (*value)[length+1] = '\0';
}

bool INI_Read(INI* handle, INI_PFN_READ readfn, void* userdata)
{
  int last_token = _INI_INTERPRETER_NONE;
  int token;
  char * valval = NULL;
  char * value = NULL;
  INI_section * current_section = INI_GetSection(handle, NULL);
  bool has_error = false;
  while ((token = _INI_Read_Interpreter(handle, readfn, userdata,
                                        last_token, &value)) != EOF)
    {
      // Check for error
      if (token >= 400)
        {
          _INI_ERROR("Token interpreter failed");
          has_error = true;
          break;
        }

      if (token == _INI_INTERPRETER_NONE)
        continue;

      // trim value if not null
      if (value && INI_GetFlag(handle, INI_FLAG_TRIM_SPACES))
        _INI_str_trim(&value);

      if (token == _INI_INTERPRETER_SECTION)
        {
          current_section = INI_section_New(NULL, value);
          if (!INI_AddSection(handle, current_section))
            {
              _INI_ERROR("Section already exists.");
              _INI_AERR fprintf(stderr, "Section: %s\n", value);
              // Only mention a section once
              INI_section_Free(current_section);
              has_error = true;
              break;
            }
        }

      if (token == _INI_INTERPRETER_KEY)
        {
          valval = value;
          value = NULL;
        }

      if (last_token == _INI_INTERPRETER_KEY)
        {
          if (!valval)
            {
              _INI_ERROR("Can't assign value to non existing key.");
              has_error = true;
              break;
            }

          if (token == _INI_INTERPRETER_VALUE && value)
            {
              if (!INI_section_AddString(current_section, valval, value))
                {
                  _INI_ERROR("Key alread exists.");
                  _INI_AERR fprintf(stderr, "Key: %s\n", valval);
                  has_error = true;
                  break;
                }
            }
          else
            {
              if (!INI_section_AddString(current_section, valval, ""))
                {
                  _INI_ERROR("Key already exists.");
                  _INI_AERR fprintf(stderr, "Key: %s\n", valval);
                  has_error = true;
                  break;
                }
            }
          
          free(valval);
          valval = NULL;
        }

      if (token == '\n')
        token = _INI_INTERPRETER_NONE;

      // Dealloc value if not null
      if (value)
        {
          free(value);
          value = NULL;
        }
      last_token = token;
    }
  // Dealloc value if not null
  if (value)
    {
      free(value);
      value = NULL;
    }
  if (valval)
    {
      free(value);
      value = NULL;
    }
  // Return error state
  return !has_error;
}

struct _INI_readu_str
{
  char * data;
  size_t pos;
};

inline static int _INI_readfn_string(struct _INI_readu_str* userdata)
{
  if (userdata->data[userdata->pos] == '\0')
    {
      return EOF;
    }

  int result = (int) userdata->data[userdata->pos++];
  return result;
}

bool INI_ReadString(INI* handle, const char * data)
{
  struct _INI_readu_str userdata;
  userdata.data = (char*) data;
  userdata.pos = 0;

  if (!(INI_Read(handle, (INI_PFN_READ) _INI_readfn_string, (void*) &userdata)))
    return false;

  return true;
}

inline static int _INI_readfn_file(FILE* file)
{
  return fgetc(file);
}

bool INI_ReadFile(INI* handle, FILE* file)
{
    return INI_Read(handle, (INI_PFN_READ) _INI_readfn_file, (void*) file);
}

/* Write */

inline static char * _INI_GetNewLine(INI* handle)
{
  if (INI_GetFlag(handle, INI_FLAG_WINDOWS))
    return "\r\n";
  else
    return "\n";
}

inline static bool _INI_WriteSection(INI* handle,
                                     INI_section* section,
                                     INI_PFN_WRITE writefn,
                                     void* userdata)
{
  // Write section name, if not global
  if (section->name)
    {
      if (!(writefn("[", userdata)
          && writefn(section->name, userdata)
          && writefn("]", userdata)
          && writefn(_INI_GetNewLine(handle), userdata)))
        return false;
    }

  bool has_pairs = false;
  // Write section keys and values
  for (size_t i = 0; i < 255; ++i)
    {
      INI_pair* * pairs = section->pairs[i];
      // Check if pairs is NULL. If it is continue.
      if (!pairs)
        continue;
      // Iterate through pairs
      for (size_t ii = 0; pairs[ii]; ++ii)
        {
          has_pairs = true;
          // Write key=value\n
          if(!(writefn(pairs[ii]->key, userdata)
             && writefn("=", userdata)
             && writefn(pairs[ii]->value, userdata)
             && writefn(_INI_GetNewLine(handle), userdata)))
            return false;
        }
    }

  if (has_pairs)
    {
      // Write new line
      return writefn(_INI_GetNewLine(handle), userdata);
    }
  
  return true;
}

bool INI_Write(INI* handle, INI_PFN_WRITE writefn, void* userdata)
{
  for (size_t i = 0; i < 256; ++i)
    {
      INI_section* * sections = handle->sections[i];
      // If sections is NULL, continue.
      if (!sections)
        continue;
      // Iterate through sections
      for (size_t ii = 0; sections[ii]; ++ii)
        {
          if(!_INI_WriteSection(handle, sections[ii], writefn, userdata))
            {
              writefn("\0", userdata);
              return false;
            }
        }
    }

  writefn(NULL, userdata);
  return true;
}

struct _INI_writeu_str
{
  char * result;
  size_t used;
  size_t length;
};

inline static bool _INI_writefn_string(const char * txt,
                                       struct _INI_writeu_str* userdata)
{
  // Finished writing
  if (!txt)
    {
      // trim string & add terminating '\0'
      userdata->result = realloc(userdata->result,
                                 (1u + userdata->used) * sizeof(char));
      userdata->result[userdata->used] = '\0'; // NULL-terminated string
      return true;
    }
  // Error str
  if (txt[0] == '\0')
    return true;
  
  size_t txtlen = strlen(txt);
  
  // Enlarge string capacity if neccessary
  if (userdata->used + txtlen >= userdata->length)
    {
      userdata->length += 1024u;
      userdata->result = realloc(userdata->result,
                                 userdata->length * sizeof(char));
      if (!userdata->result)
        return false; // Out of memory
    }
  
  for (size_t i = 0; i < txtlen; ++i)
    {
      userdata->result[userdata->used++] = txt[i];
    }

  return true;
}

bool INI_WriteString(INI* handle, char ** result)
{
  struct _INI_writeu_str userdata;
  userdata.length = 1024u;
  userdata.used = 0u;
  userdata.result = (char*) malloc(userdata.length * sizeof(char));

  if (!INI_Write(handle,
                 (INI_PFN_WRITE) _INI_writefn_string,
                 (void*) &userdata))
    {
      free(userdata.result);
      *result = NULL;
      return false;
    }

  *result = userdata.result;
  return true;
}

inline static bool _INI_writefn_file(const char * txt,
                                     FILE* file)
{
  if (!txt)
    return true;
  if (!txt[0])
    return false;

  return fprintf(file, txt);
}

bool INI_WriteFile(INI* handle, FILE* file)
{
  return INI_Write(handle,
                   (INI_PFN_WRITE) _INI_writefn_file,
                   (void*) file);
}

/* File path functions */

bool INI_ReadFilePath(INI* handle, const char * path)
{
  FILE* f = fopen(path, "r");
  if (!f)
    {
      _INI_ERROR("Could not open file for read operation.");
      return false;
    }

  bool status = INI_ReadFile(handle, f);
  fclose(f);
  return status;
}

bool INI_WriteFilePath(INI* handle, const char * path)
{
  FILE* f = fopen(path, "w");
  if (!f)
    {
      _INI_ERROR("Could not open file for write operation.");
      return false;
    }

  bool status = INI_WriteFile(handle, f);
  fclose(f);
  return status;
}

#ifdef __cplusplus
}
#endif
