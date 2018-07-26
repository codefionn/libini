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

/*! @file INI/ini.h
 *  @brief One and only header file for INI
 */

/*! @mainpage INI library
 *  
 *  INI library is a small utility for interacting
 *  with INI files.
 *
 *  @author Fionn Langhans
 *  @version 0.0
 */
#ifndef INI_H
#define INI_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <ctype.h>

#ifndef INIAPI
# define INIAPI extern
#endif

#define _INI_ALLOC_TYPE_EXTERN  0
#define _INI_ALLOC_TYPE_API     1

/* ::INI_pair */

/*! @brief Structure for storing a key and
 *         the assigned value
 */
typedef struct INI_pair {
  /* @brief For memory magement (API/EXTERN). */
  uint8_t alloc_type;
  /* @brief Key */
  char * key;
  /* @brief Value assigned to key */
  char * value;
} INI_pair;

/*! @brief Create a new pair.
 *
 *  @param pair  Can be null. If not, this parameter will be set
 *               to the new generated pair and the API won't manage
 *               the directly allocated memory of the pair. If NULL,
 *               it is managed by the API. In both cases INI_pair_Free
 *               has to be called to free storage.
 *  @param key   When null, set it later on. Will be copied.
 *  @param value When null. set it later on. Will be copied.
 */
INIAPI
INI_pair* INI_pair_New(INI_pair* pair,
                       const char * key, const char * value);

/*! @brief Deallocates storage used by pointers in pair.
 *         Also deletes memory of pair itself, if it has been
 *         allocated by the API (You passed a NULL pointer to
 *         the INI_pair_New function).
 */
INIAPI
void INI_pair_Free(INI_pair* pair);

/*! @brief Set the key of a pair.
 *
 *  @param key Must not be NULL. Will be copied.
 */
INIAPI
void INI_pair_SetKey(INI_pair* pair, const char * key);

/*! @brief Set the value of a pair.
 *
 *  @param vaule Must not be NULL. Will be copied.
 */
void INI_pair_SetValue(INI_pair* pair, const char * value);

/* ::INI_section */

/*! @brief Capable of storing whole sections.
 */
typedef struct INI_section {
  /*! @brief Memory management (API/EXTERN). */
  uint8_t alloc_type;
  /*! @brief Name of the section. */
  char * name;
  /*! @brief Attribute for the pairs (key,value) of the configuration.
   *         (+ Indexing)
   */
  INI_pair* * pairs[255];
} INI_section;

/*! @brief Create a new configuration.
 *
 *  @param conf Can be null. If this parameter is not null, the newly created
 *              configuration is stored in this one (but also returned). Also
 *              means directly allocated storage of conf won't be managed by
 *              the API. If NULL, memory will be managed by the API. In both
 *              cases INI_section_Free has to be called to free memory.
 *  @param name Name of the configuration (When creating the global one: NULL)
 *
 *  @return Returns the new configuration. NULL if error occured.
 */
INIAPI
INI_section* INI_section_New(INI_section* conf,
                             const char * name);

/*! @brief Deallocates pointers of the INI configuration.
 *         Also deletes memory of conf (INI_section) itself if it
 *         has been allocated directly by the API (you passed NULL
 *         to the INI_section_New function). If not you have to free
 *         directly allocated memory yourself.
 */
INIAPI
void INI_section_Free(INI_section* conf);

/*! @return Returns the pair of key. If the key doesn't exist,
 *          NULL is returned.
 */
INI_pair* INI_section_Get(INI_section* conf, const char * key);

/*! @return Returns the c-string (not copied!) value of key. If the key doesn't
 *          exist, NULL is returned.
 */
INIAPI
const char * INI_section_GetString(INI_section* conf, const char * key);

/*! @return Returns a boolean value of key. If the key doesn't exist,
 *          defaultValue is returned.
 *  @param exists Can be null. Will be set to true, if the value exists.
 */
INIAPI
bool INI_section_GetBool(INI_section* conf,
                         bool defaultValue, const char * key,
                         bool* exists);

/*! @return Returns a integer value of key. If the key doesn't exist,
 *          defaultValue is returned.
 *  @param exists Can be null. Will be set to true, if the value exists.
 */
INIAPI
int INI_section_GetInt(INI_section* conf,
                       int defaultValue, const char * key,
                       bool* exists);

/*! @return Returns a float value of key. If the key doesn't exist,
 *          defaultValue is returned.
 *  @param exists Can be null. Will be set to true, if the value exists.
 */
INIAPI
float INI_section_GetFloat(INI_section* conf,
                           float defaultValue, const char * key,
                           bool* exists);

/*! @brief Sets the value of key to 'value'.
 *
 *  If 'key' was already set, the value of key is set. If 'key'
 *  doesn't exist, a new one will be created and assigned to 'value'.
 */
INIAPI
void INI_section_SetString(INI_section* conf,
                           const char * key, const char * value);

/*! @brief Adds the key and value to the pairs (section).
 *  
 *  @param conf
 *  @param key
 *  @param value
 *
 *  @return Returns false, if key already exists.
 */
INIAPI
bool INI_section_AddString(INI_section* conf,
                           const char * key, const char * value);

/* ::INI */

/*! @brief Be verbose.                      */
#define INI_FLAG_VERBOSE       0x0001
/*! @brief Print errors to stderr.          */
#define INI_FLAG_ERROR         0x0002
/*! @brief Interpreter trimes spaces        */
#define INI_FLAG_TRIM_SPACES   0x0004
/*! @brief Use windows newlines for writing.
 *         Standard are Linux file endings
 *         '\n'.                            */
#define INI_FLAG_WINDOWS       0x0008

/*! @brief Flags that are active at default. */
#define INI_FLAGS_DEFAULT INI_FLAG_ERROR | INI_FLAG_TRIM_SPACES

/*! @brief The handler for INI the format (a configuration)
 */
typedef struct INI {
  /*! @brief How the memory of the INI reference should be handled (API,EXTERN)
   */
  uint8_t alloc_type;
  /*! @brief All sections. Using indexing. */
  INI_section* * sections[256];
  /*! @brief How the INI should behave. */
  uint8_t flags;
} INI;

/*! @return Returns a new INI handler.
 *
 *  @param If handler is NULL the memory, which the handler requires directly,
 *         will be managed by the API. If not NULL, the memory directly required
 *         by handler won't be managed by the API. In both cases you have to
 *         call INI_Free to free storage.
 */
INIAPI
INI* INI_New(INI* handler);

/*! @brief Deallocates pointers of the INI handler.
 *         Also deletes memory of the handler itself, if allocated by 
 *         the API (you passed NULL to the INI_New function). If not,
 *         deallocate the directly allocated memory yourself.
 */
INIAPI
void INI_Free(INI* handler);

/*! @brief Check if flag is active.
 *
 *  @return Returns true if flag in handler is activated.
 */
INIAPI
bool INI_GetFlag(INI* handler, int flag);

/*! @brief Activates/Deactivates a feature/flag.
 *         You could also activate/deactivate several flags at once.
 *  
 *  @param handler,
 *  @param flag
 *  @param status true=active,false=deactive
 */
INIAPI
void INI_SetFlag(INI* handler, int flog, bool status);

/*! @return Returns a section called 'section'. If the section doesn't exist
 *          NULL is returned.
 */
INIAPI
INI_section* INI_GetSection(INI* handler, const char * section);

/*! @return Returns the pair of key. If the key doesn't 
 *          exist, NULL is returned.
 *  @param conf
 *  @param section In which section to search (NULL for the global one).
 *  @param key     The key to use.
 */
INIAPI
INI_pair* INI_Get(INI* handler,
                  const char * section, const char * key);

/*! @return Returns the c-string value (not copied!) of key. If the key doesn't 
 *          exist, NULL is returned.
 *  @param conf
 *  @param section In which section to search (NULL for the global one).
 *  @param key     The key to use.
 */
INIAPI
const char * INI_GetString(INI* handler,
                     const char * section, const char * key);

/*! @return Returns the boolean value of key. If the key doesn't
 *          exist, defaultValue is returned.
 *  @param conf
 *  @param defaultValue
 *  @param section      In which section to search (NULL for the global one).
 *  @param key          The key to use.
 *  @param exists Can be null. Will be set to true, if the value exists.
 */
INIAPI
bool INI_GetBool(INI* handler, bool defaultValue,
                 const char * section, const char * key,
                 bool* exists);

/*! @return Returns the integer value of key. If the key doesn't
 *          exist, defaultValue is returned.
 *  @param conf
 *  @param defaultValue
 *  @param section      In which section to search (NULL for the global one).
 *  @param key          The key to use.
 *  @param exists Can be null. Will be set to true, if the value exists.
 */
INIAPI
int INI_GetInt(INI* handler, int defaultValue,
               const char * section, const char * key,
               bool* exists);

/*! @return Returns the float value of key. If the key doesn't
 *          exist, defaultValue is returned.
 *  @param conf
 *  @param defaultValue
 *  @param section      In which section to search (NULL for the global one).
 *  @param key          The key to use.
 *  @param exists Can be null. Will be set to true, if the value exists.
 */
INIAPI
float INI_GetFloat(INI* handler, float defaultValue,
               const char * section, const char * key,
               bool* exists);

/*! @brief Adds section to INI handler.
 *  
 *  @return Returns false, if section already exists.
 */
INIAPI
bool INI_AddSection(INI* handler, INI_section* conf);

/*! @brief Sets a key to value in section. If key doesn't
 *         exist, then it will be created. The section
 *         won't be created.
 *
 *  @return Returns false, if section doesn't exist.
 */
INIAPI
bool INI_SetString(INI* handler,
                   const char * section,
                   const char * key, const char * value);

/*! @brief Adds key to value in section.
 *  
 *  @return Returns false, if section doesn't exist or
 *          if key does exist.
 */
INIAPI
bool INI_AddString(INI* handler,
                   const char * section,
                   const char * key, const char * value);

/* Interpreter functions */

#define INI_READ_ERROR_FILE   400
#define INI_READ_ERROR_FORMAT 401
#define INI_READ_ERROR_MEM    402
#define INI_READ_ERROR_MAX    402

/*! @brief A interface for reading from userdata
 *  
 *  @return Normally returns 'char's. If EOF then EOF.
 *          Returns INI_READ_ERROR
 */
typedef int (*INI_PFN_READ) (void* userdata);

/*! @brief Reads INI formatted data from userdata with readfn
 *         till EOF.
 *
 *  @param handle
 *  @param readfn
 *  @param userdata
 *
 *  @return Returns false, if reading failed.
 */
INIAPI
bool INI_Read(INI* handle, INI_PFN_READ readfn, void* userdata);

/*! @brief Reads INI formatted data from 'data' and writes results
 *         into 'handle'
 */
INIAPI
bool INI_ReadString(INI* handle, const char * data);

/*! @brief Reads INI formatted data from 'file' and writes result
 *         into handle
 */
INIAPI
bool INI_ReadFile(INI* handle, FILE* file);

/*! @brief Reads INI formatted ata from the file 'path' and writes
 *         results into handle
 */
INIAPI
bool INI_ReadFilePath(INI* handle, const char * path);

/*! @brief A interface for writting to userdata
 *
 *  @param txt txt is equal to NULL, if all data has been written.
 *             strlen(txt) is equal to 0, if error occured.
 *
 *  @return Returns false, if writing to userdata failed.
 */
typedef bool (*INI_PFN_WRITE) (const char * txt, void* userdata);

/*! @brief Write INI formatted data from handle to userdata using writefn.
 *  
 *  @return Returns false if failed.
 */
INIAPI
bool INI_Write(INI* handle, INI_PFN_WRITE writefn, void* userdata);

/*! @brief Writes INI formatted data from handle to result.
 *
 *  @param handle
 *  @param result *result has to be NULL(result itself not).
 *
 *  @return Returns false if failed.
 */
INIAPI
bool INI_WriteString(INI* handle, char ** result);

/*! @brief Write INI formated data from handle to file.
 *  
 *  @return Returns false if failed.
 */
INIAPI
bool INI_WriteFile(INI* handle, FILE* file);

/*! @brief Write handle to file 'path'.
 *  
 *  @param handle
 *  @param path
 */
INIAPI
bool INI_WriteFilePath(INI* handle, const char * path);

#ifdef __cplusplus
}
#endif

#endif /* INI_H */
