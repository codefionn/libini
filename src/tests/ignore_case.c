#ifdef __cplusplus
extern "C" {
#endif

#include <INI/ini.h>
#include <assert.h>

int main(void)
{
  // Check compare function
  assert(_INI_strcasecmp("hello", "hello"));
  assert(_INI_strcasecmp("hello", "Hello"));
  assert(_INI_strcasecmp("hello", "hELlo"));
  assert(_INI_strcasecmp("hello", "heLLO"));
  assert(_INI_strcasecmp("Test", "test"));

  INI handle;
  INI_New(&handle);
  assert(INI_GetFlag(&handle, INI_FLAG_IGNORE_CASE));
  // Add some stuff
  assert(INI_AddSection(&handle, INI_section_New(NULL, "Test")));
  // Checkout the section
  assert(INI_GetSection(&handle, "Test"));
  assert(INI_GetSection(&handle, "test"));
  assert(INI_AddString(&handle, "Test", "Hello", "manpage"));
  assert(!INI_AddString(&handle, "Test", "hello", "..."));
  assert(INI_AddString(&handle, "test", "status", "on"));
  // Check if stuff exists
  assert(INI_Get(&handle, "Test", "Hello"));
  assert(INI_Get(&handle, "Test", "hello"));
  assert(INI_Get(&handle, "Test", "hEllO"));
  assert(INI_Get(&handle, "test", "heLLo"));

  assert(INI_Get(&handle, "Test", "status"));
  assert(INI_Get(&handle, "Test", "Status"));

  INI_Free(&handle);

  return EXIT_SUCCESS;
}

#ifdef __cplusplus
}
#endif
