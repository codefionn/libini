#include <INI/ini.h>
#include <assert.h>

#ifdef NDEBUG
# undef NDEBUG
#endif

int main(void)
{
  INI handle;
  assert(INI_New(&handle));
  
  assert(INI_AddSection(&handle, INI_section_New(NULL, "test")));

  // Add elements
  assert(INI_AddString(&handle, "test", "value", "0"));
  assert(INI_AddString(&handle, "test", "status", "off"));
  assert(INI_AddString(&handle, "test", "status_program", "on"));

  // Check output
  const char * expected = "[test]\nvalue=0\nstatus=off\nstatus_program=on\n\n";
  char * result;
  assert(INI_WriteString(&handle, &result));
  printf("Expected:\n%sResult:\n%s", expected, result);
  assert(strcmp(result, expected) == 0);
  free(result);

  INI_Free(&handle);
  
  return EXIT_SUCCESS;
}
