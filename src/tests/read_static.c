#include <INI/ini.h>

#ifdef NDEBUG
# undef NDEBUG
#endif
#include <assert.h>

static bool test_key(INI* handle, char * section, char * key, char * value)
{
  return INI_Get(handle, section, key)
    && strcmp(INI_GetString(handle, section, key), value) == 0;
}

static void runCheck(INI* handle, int n)
{
  printf("Checknum: %d\n", n);
  // global
  assert(test_key(handle, NULL, "global", "True"));

  // name
  assert(test_key(handle, NULL, "name", "test read static"));

  // Program.status
  assert(test_key(handle, "Program", "status", "on"));

  // Program.output
  assert(test_key(handle, "Program", "output", "Hello World"));
}

/* Checks with spaces */
static void runCheckSpaces(INI* handle, int n)
{
  printf("Checknum: %d\n", n);
  // global
  assert(test_key(handle, NULL, "  global", "True"));

  // name
  assert(test_key(handle, NULL, " name ", "   test read static"));

  // Program.status
  assert(test_key(handle, "  Program ", " status", "on"));

  // Program.output
  assert(test_key(handle, "  Program ", "output", "Hello World  "));
}

int main(void)
{
  INI handle;
  INI_New(&handle); // Manual memory management of the direct reference

  const char * txt = "  global=True\n name =   test read static\n[  Program ]\n status=on\nname=test_read_static\noutput=Hello World  ";
  INI_SetFlag(&handle, INI_FLAG_WINDOWS, true);
  assert(INI_ReadString(&handle, txt));
  INI_SetFlag(&handle, INI_FLAG_WINDOWS, false);

  INI_WriteFile(&handle, stdout);
  
  runCheck(&handle, 0);

  char * generatedtxt;
  assert(INI_WriteString(&handle, &generatedtxt));
  // Reload handle
  INI_Free(&handle);
  INI_New(&handle);
  // Load from text
  assert(INI_ReadString(&handle, txt));
  runCheck(&handle, 1);
  // Exit
  // Free allocated text
  free(generatedtxt);

  // Reload handle; Check with spaces
  INI_Free(&handle);
  INI_New(&handle);
  INI_SetFlag(&handle, INI_FLAG_TRIM_SPACES, false);
  assert(INI_ReadString(&handle, txt));
  assert(INI_WriteFile(&handle, stdout));
  runCheckSpaces(&handle, 2);

  return EXIT_SUCCESS;
}
