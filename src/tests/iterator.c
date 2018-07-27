#ifdef __cplusplus
extern "C" {
#endif

#include <INI/ini.h>

#ifdef NDEBUG
# undef NDEBUG
#endif
#include <assert.h>

inline static void countIter(INI* handle,
                             size_t* count_sections,
                             size_t* count_pairs)
{
  INI_iter it;
  INI_iter_FromHandle(&it, handle);

  INI_section* section;
  while ((section = INI_iter_NextSection(&it)))
    {
      ++(*count_sections);
      INI_iter it_pairs;
      INI_iter_FromSection(&it_pairs, section);
      INI_pair* pair;
      while ((pair = INI_iter_NextPair(&it_pairs)))
        ++(*count_pairs);
    }
}

int main(void)
{
  INI handle;
  INI_New(&handle);

  size_t count_sections = 0;
  size_t count_pairs = 0;
  countIter(&handle, &count_sections,& count_pairs);
  assert(count_sections == 1);
  assert(count_pairs == 0);
  count_sections = 0; count_pairs = 0;

  // Add sections and test
  assert(INI_AddSection(&handle, INI_section_New(NULL, "test")));
  assert(INI_AddSection(&handle, INI_section_New(NULL, "sec")));
  countIter(&handle, &count_sections, &count_pairs);
  assert(count_sections == 3);
  assert(count_pairs == 0);
  count_sections = 0; count_pairs = 0;

  // Add pairs and test
  assert(INI_AddString(&handle, NULL, "value", "True"));
  assert(INI_AddString(&handle, NULL, "value_type", "False"));
  assert(INI_AddString(&handle, NULL, "value_refs", "1"));
  assert(INI_AddString(&handle, "test", "test", "True"));
  
  assert(INI_AddSection(&handle, INI_section_New(NULL, "ttest")));
  assert(INI_AddSection(&handle, INI_section_New(NULL, "tttest")));
  assert(INI_AddString(&handle, "tttest", "ants", "100"));
  assert(INI_AddString(&handle, "tttest", "hello", "world"));

  countIter(&handle, &count_sections, &count_pairs);
  assert(count_sections == 5);
  assert(count_pairs == 6);
  count_sections = 0; count_pairs = 0;

  INI_Free(&handle);
  
  return EXIT_SUCCESS;
}

#ifdef __cplusplus
}
#endif
