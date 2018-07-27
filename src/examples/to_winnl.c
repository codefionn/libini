/*! @file to_winnl.c
 *  @brief Read ini file and output new ini file with DOS newlines
 */
#include <INI/ini.h>

int main(int argc, char * argv[])
{
  if (argc != 3)
    {
      fprintf(stderr, "Invalid usage!\n");
      fprintf(stderr, "Usage: %s input.ini output.ini\n", argv[0]);
      return EXIT_FAILURE;
    }
  // Create handle
  INI handle;
  INI_New(&handle);
  // Read from file
  if(!INI_ReadFilePath(&handle, argv[1]))
  {
    goto error;
  }
  INI_SetFlag(&handle, INI_FLAG_WINDOWS, true);
  // Write to file
  if (!INI_WriteFilePath(&handle, argv[2]))
    {
      goto error;
    }
  // Free allocated memory
  INI_Free(&handle);

  return EXIT_SUCCESS;
error:
  INI_Free(&handle);
  return EXIT_FAILURE;
}
