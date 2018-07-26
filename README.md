# INI

A library for reading INI files. Has configure options using flags.
Windows & Linux newlines supported.

## Example

Some examples to help you get started.

### Write stored data to file

```C
INI* handle = INI_New(NULL);

// Add keys & values to global section
INI_AddSection(handle, NULL, "is_global", "True");

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

## Read INI file and print value of key to stdout

```C
INI handle;
INI_New(&handle);

if (!INI_ReadFilePath(handle, "file.ini"))
  {
    // Error handling ...
  }

const char * value;
if (!(value = INI_GetString(handle, NULL, "do_tests")))
  {
    // Error handling ...
  }

puts(value);
```
