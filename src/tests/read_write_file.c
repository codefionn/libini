#include <INI/ini.h>
#ifdef NDEBUG
# undef NDEBUG
#endif
#include <assert.h>

inline static bool checkValue(INI* handle,
                              const char * section,
                              const char * key,
                              const char * value)
{
  return INI_Get(handle, section, key)
    && strcmp(INI_GetString(handle, section, key), value) == 0;
}

inline static void check(INI* handle, int n)
{
  printf("Checknum: %d\n", n);

  assert(checkValue(handle, NULL, "is_always_on_top", "False"));
  assert(checkValue(handle, "Hello", "val", "True"));
  assert(checkValue(handle, "Hello", "namie", "Voyage"));
  assert(checkValue(handle, "Hello", "value_type", "String"));
  assert(checkValue(handle, "Hello", "value_exists", "1"));
}

int main(void)
{
  INI* handle = INI_New(NULL);
  INI_SetFlag(handle, INI_FLAG_WINDOWS, true);

  assert(INI_AddSection(handle, INI_section_New(NULL, "Hello")));
  assert(INI_AddString(handle, "Hello", "val", "True"));
  assert(INI_AddString(handle, "Hello", "namie", "Voyage"));
  assert(INI_AddString(handle, "Hello", "value_type", "String"));
  assert(INI_AddString(handle, "Hello", "value_exists", "1"));
  assert(INI_AddString(handle, NULL, "is_always_on_top", "False"));

  assert(INI_WriteFilePath(handle, "test.ini"));
  check(handle, 0);

  INI_Free(handle);
  handle = INI_New(NULL);
  assert(INI_ReadFilePath(handle, "test.ini"));
  check(handle, 1);

  INI_Free(handle);
  return EXIT_SUCCESS;
}
