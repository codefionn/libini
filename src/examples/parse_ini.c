#include <INI/ini.h>

int main(int argc, char * argv[])
{
  if (argc != 2)
    {
      fprintf(stderr, "Invalid usage!\n");
      fprintf(stderr, "Usage: %s file.ini\n", argv[0]);
      return EXIT_FAILURE;
    }
  // Create handle
  INI handle;
  INI_New(&handle);
  // Read from file
  if(!INI_ReadFilePath(&handle, argv[1]))
  {
    // Free allocated memory
    INI_Free(&handle);
    return EXIT_FAILURE;
  }

  // Free allocated memory
  INI_Free(&handle);

  return EXIT_SUCCESS;
}
