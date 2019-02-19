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

uint8_t _INI_strhash(const char * s, bool ignore_case) {
	uint8_t result = 0;
	for (size_t i = 0; s[i] != (uint8_t) '\0'; ++i) {
		if (ignore_case)
			result += (uint8_t) tolower(s[i]);
		else
			result += (uint8_t) s[i];
	}

	return result;
}

#ifdef __cplusplus
}
#endif
