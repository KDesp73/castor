#include <stdio.h>
#include <stdlib.h>

int test_map_roundtrip(void);
int test_astar_finds_target(void);
int test_nav_cache_matches_export(void);

typedef int (*test_fn)(void);

typedef struct {
    const char* name;
    test_fn fn;
} TestCase;

static const TestCase tests[] = {
    {"map_roundtrip", test_map_roundtrip},
    {"astar_finds_target", test_astar_finds_target},
    {"nav_cache_matches_export", test_nav_cache_matches_export},
};

int main(void)
{
    int failed = 0;
    size_t count = sizeof(tests) / sizeof(tests[0]);

    for (size_t i = 0; i < count; i++) {
        printf("[TEST] %s ... ", tests[i].name);
        fflush(stdout);
        if (tests[i].fn() == 0) {
            printf("ok\n");
        } else {
            printf("FAILED\n");
            failed++;
        }
    }

    if (failed > 0) {
        fprintf(stderr, "%d test(s) failed\n", failed);
        return 1;
    }

    printf("All %zu test(s) passed.\n", count);
    return 0;
}
