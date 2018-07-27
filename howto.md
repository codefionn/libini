# HowTo

### Add a key to a non-existing section

The problem: When adding/setting a key the section you referred to won't
be created. You have to create it before.

```C
INI_AddSection(handle, INI_section_New(NULL, "test"))
// Now you can add pairs to the test section
INI_AddString(handle, "test", "value", "True")
```

### Write with DOS (Windows) new lines

```C
INI_SetFlag(handle, INI_FLAG_WINDOWS, true);
```

### Enable Spaces at the beginning end end of sections, keys and values

```C
INI_SetFlag(handle, INI_FLAG_TRIM_SPACES, false);
```

### Disable error printing

```C
INI_SetFlag(handle, INI_FLAG_ERROR, false);
```
