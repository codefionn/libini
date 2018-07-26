#ifdef __cplusplus
extern "C" {
#endif

#include <INI/ini.h>
#include <string.h>

#ifdef NDEBUG
# undef NDEBUG
#endif
#include <assert.h>

int main(int argc, char * argv[])
{
  // handle allocated here. So it isn't managed by the API.
  // because 'handle' falls out of scope at the end of the
  // main method it will be automatically deallocated. We
  // don't have to worry about that
  INI handle;
  INI_New(&handle);

  // Check if global section exists
  assert(INI_GetSection(&handle, NULL));

  // Test for invalid section
  assert(!INI_GetSection(&handle, "invalid_section"));

  // Add a new section
  INI_section* test;
  assert(INI_AddSection(&handle, test = INI_section_New(NULL, "test")));
  // Check if new section exists
  assert(INI_GetSection(&handle, "test"));
  // Add element to section
  INI_AddString(&handle, "test", "value", "Hello, World!");
  const char * value;
  assert(value = INI_GetString(&handle, "test", "value"));
  assert(strcmp(value, "Hello, World!") == 0);
  // Try to add "test.value" again (should fail)
  assert(!INI_AddString(&handle, "test", "value", "Hello, World!"));

  // Add value to invalid section
  assert(!INI_AddString(&handle, "invalid_section", "value", "Hello, World!"));

  // Add another value
  assert(INI_AddString(&handle, "test", "test", "Second test value"));
  assert(INI_Get(&handle, "test", "test"));

  // Add another section starting with 't'
  assert(INI_AddSection(&handle, INI_section_New(NULL, "testing")));
  assert(INI_GetSection(&handle, "testing"));
  assert(INI_AddString(&handle, "testing", "value0", "Hello, World!"));
  assert(INI_GetString(&handle, "testing", "value0"));

  INI_Free(&handle);
  return EXIT_SUCCESS;
}

#ifdef __cplusplus
}
#endif
