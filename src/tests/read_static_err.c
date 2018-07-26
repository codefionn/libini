#include <INI/ini.h>

#ifdef NDEBUG
# undef NDEBUG
#endif
#include <assert.h>

int main(void)
{
  INI handle;
  INI_New(&handle);
  INI_SetFlag(&handle, INI_FLAG_ERROR, false);

  // Testing thing that should fail
  assert(!INI_ReadString(&handle, "[Category0][Category1]"));
  assert(!INI_ReadString(&handle, "[Category0] [Category1]"));
  assert(!INI_ReadString(&handle, "[does-exist]\n[does-exist]"));
  // Test things that should work
  assert(INI_ReadString(&handle, "   hello=value"));
  assert(INI_ReadString(&handle, "[Category]\n[Category3]"));
  // Free
  INI_Free(&handle);

  return EXIT_SUCCESS;
}
