#ifdef __cplusplus
extern "C" {
#endif

#include "INI/ini.h"

bool _INI_strcasecmp(const char * s0, const char * s1)
{
  size_t i;
  for (i = 0; s0[i] && s1[i]; ++i)
    {
      if (isalpha(s0[i]) && isalpha(s1[i]))
        {
          if (tolower(s0[i]) != tolower(s1[i]))
            return false;
        }
      else if (s0[i] != s1[i])
        return false;
    }
  // If equal, i at s0 and s1 should be equal to '\0'
  return !s0[i] && !s1[i];
}

#ifdef __cplusplus
}
#endif
