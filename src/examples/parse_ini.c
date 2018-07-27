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
  // Iterate through INI
  INI_iter its;
  INI_iter_FromHandle(&its, &handle);
  INI_section* section;
  while ((section = INI_iter_NextSection(&its)))
    {
      // name of the global section is NULL
      // thats why we need an 'if' here
      // println [section]
      if (section->name)
        printf("[%s]\n", section->name);
      // iterate through the section
      INI_iter itp;
      INI_iter_FromSection(&itp, section);
      INI_pair* pair;
      while((pair = INI_iter_NextPair(&itp)))
        {
          // println key=value
          printf("%s=%s\n", pair->key, pair->value);
        }
    }
  // Free allocated memory
  INI_Free(&handle);

  return EXIT_SUCCESS;
}
