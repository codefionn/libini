# INI

A library for reading INI files. Has configure options using flags.
Windows & Linux newlines supported.

## Build

```sh
mkdir build
cd build
cmake -G "Unix Makefiles" .. ; make -j9
```

## Testing

Test the project (Assumes you already build the project in the build dir).

```sh
cd build/tests
ctest
```

## Documentation

Create the docs:

```sh
doxygen .doxyconf
```

Open docs (Linux):
```sh
xdg-open docs/html/index.html
```

## Examples

Some examples to help you get started.
For short howtos go to /howto.md

### Write stored data to file

```C
INI* handle = INI_New(NULL);

// Add keys & values to global section
INI_AddString(handle, NULL, "is_global", "True");

// Add keys & values to section 'mysection'
INI_AddSection(handle, INI_section_New(NULL, "mysection"));
INI_AddString(handle, "mysection", "awesome", "9000");
INI_AddString(handle, "mysection", "id", "1");


if (!INI_WriteFilePath("file.ini"))
  {
    // Error handling ...
  }

INI_Free(handle);
```

### Read INI file and write to stdout

```C
INI* handle = INI_New(NULL);

if (!INI_ReadFilePath(handle, "file.ini"))
  {
    // Error handling ...
  }

INI_WriteFile(handle, stdout);

INI_Free(handle); // Delete handle
```

### Read INI file and print value of key to stdout

```C
INI handle;
INI_New(&handle);

if (!INI_ReadFilePath(&handle, "file.ini"))
  {
    // Error handling ...
  }

const char * value;
if (!(value = INI_GetString(&handle, NULL, "do_tests")))
  {
    // Error handling ...
  }

puts(value);

INI_Free(&handle);
```

### Write INI data to string and read it again

```C
INI handle;
INI_New(&handle);

// Manipulate data of handle (Add strings and so on)

char * str_result; // We need to free this variable again
INI_WriteString(&handle, &str_result);

// Reload handle
INI_Free(&handle);
INI_New(&handle);

// Load data from string
INI_ReadString(&handle, str_result);
free(str_result);

INI_Free(&handle);
```

### Iterate through sections and pairs

```C
INI handle;
INI_New(&handle);

INI_ReadFilePath(&handle, "file.ini");

INI_iter it;
INI_iter_FromHandle(NULL, &handle);

INI_section* section;
while ((section = INI_iter_NextSection(&it)))
  {
    if (section->name)
      printf("[%s]\n", section->name);

    INI_iter itp;
    INI_iter_FromSection(&itp, section);
    INI_pair* pair;
    while ((pair = INI_iter_NextPair(&itp)))
      {
        printf("%s=%s\n", pair->key, pair->value);
      }
  }

INI_Free(&handle);
```
