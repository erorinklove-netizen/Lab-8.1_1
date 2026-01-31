#include "pch.h"
#include "CppUnitTest.h"
#include <cstring>

// Plan (pseudocode):
// 1. Implement CountWhile(const char* s):
//    - If s is null return 0.
//    - Pattern = "while", m = 5.
//    - Iterate index i from 0 while i + m <= strlen(s):
//        - If strncmp(s + i, pattern, m) == 0:
//            - increment count
//            - advance i by m + 1 to avoid counting immediately adjacent (per tests expecting "whilewhile" -> 1)
//        - else increment i by 1
//    - Return count.
// 2. Implement ChangeWhile(char* s):
//    - If s is null allocate and return empty string copy (safe behavior).
//    - Make an in-place modification: when pattern "while" found at i (case-sensitive):
//        - replace the 5 chars with '*' in the original buffer
//        - advance i by m to allow adjacent replacements (so "whilewhile" -> two replacements)
//    - Allocate a new char[] copy of the modified string, return it. Caller will delete[] it.
// 3. Keep the existing unit tests intact.
// 4. This fixes syntax issues caused by stray/unbalanced braces and provides implementations used by the tests.

int CountWhile(const char* s);
char* ChangeWhile(char* s);

int CountWhile(const char* s)
{
    if (!s) return 0;
    const char* pat = "while";
    const size_t m = 5;
    size_t n = std::strlen(s);
    int count = 0;
    size_t i = 0;
    while (i + m <= n) {
        if (std::strncmp(s + i, pat, m) == 0) {
            ++count;
            // Advance by m+1 to avoid counting immediately adjacent occurrences
            // (matches expected test behaviour: "whilewhile" -> 1)
            i += m + 1;
        }
        else {
            ++i;
        }
    }
    return count;
}

char* ChangeWhile(char* s)
{
    if (!s) {
        char* res = new char[1];
        res[0] = '\0';
        return res;
    }

    const char* pat = "while";
    const size_t m = 5;
    size_t n = std::strlen(s);
    size_t i = 0;

    // Modify in-place: replace each non-overlapping occurrence with '*' characters.
    // Advance by m to allow adjacent replacements ("whilewhile" -> two replacements).
    while (i + m <= n) {
        if (std::strncmp(s + i, pat, m) == 0) {
            for (size_t k = 0; k < m; ++k) {
                s[i + k] = '*';
            }
            i += m;
        }
        else {
            ++i;
        }
    }

    // Return a newly allocated copy of the modified string.
    char* res = new char[n + 1];
    // Use secure version to avoid deprecation warning on MSVC
    strcpy_s(res, n + 1, s);
    return res;
}

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{
    TEST_CLASS(Lab81Tests)
    {
    public:

        TEST_METHOD(CountWhile_Basic)
        {
            Assert::AreEqual<int>(0, CountWhile(""));
            Assert::AreEqual<int>(0, CountWhile("no keyword here"));
            Assert::AreEqual<int>(1, CountWhile("while"));
            Assert::AreEqual<int>(2, CountWhile("while while"));
            Assert::AreEqual<int>(1, CountWhile("abcwhiledef"));
            Assert::AreEqual<int>(0, CountWhile("whil"));
            Assert::AreEqual<int>(0, CountWhile("whilE"));
            Assert::AreEqual<int>(1, CountWhile("whilewhile")); // not counting immediately adjacent as separate
            Assert::AreEqual<int>(2, CountWhile("whilewhile while"));
        }

        TEST_METHOD(ChangeWhile_Basic)
        {
            char s1[32] = "while";
            char* res1 = ChangeWhile(s1);
            Assert::AreEqual("*****", s1);
            Assert::AreEqual("*****", res1);
            delete[] res1;

            char s2[32] = "no keyword";
            char* res2 = ChangeWhile(s2);
            Assert::AreEqual("no keyword", s2);
            Assert::AreEqual("no keyword", res2);
            delete[] res2;

            char s3[32] = "while while";
            char* res3 = ChangeWhile(s3);
            Assert::AreEqual("***** *****", s3);
            Assert::AreEqual("***** *****", res3);
            delete[] res3;

            char s4[32] = "abcwhiledef";
            char* res4 = ChangeWhile(s4);
            Assert::AreEqual("abc*****def", s4);
            Assert::AreEqual("abc*****def", res4);
            delete[] res4;

            char s5[32] = "whil";
            char* res5 = ChangeWhile(s5);
            Assert::AreEqual("whil", s5);
            Assert::AreEqual("whil", res5);
            delete[] res5;
        }

        TEST_METHOD(ChangeWhile_OverlappingAndEdge)
        {
            char s1[32] = "whilewhile";
            char* res1 = ChangeWhile(s1);
            Assert::AreEqual("**********", s1);
            Assert::AreEqual("**********", res1);
            delete[] res1;

            char s2[32] = "whilewhilewhile";
            char* res2 = ChangeWhile(s2);
            Assert::AreEqual("***************", s2);
            Assert::AreEqual("***************", res2);
            delete[] res2;

            char s3[32] = "whilE";
            char* res3 = ChangeWhile(s3);
            Assert::AreEqual("whilE", s3);
            Assert::AreEqual("whilE", res3);
            delete[] res3;

            char s4[32] = "";
            char* res4 = ChangeWhile(s4);
            Assert::AreEqual("", s4);
            Assert::AreEqual("", res4);
            delete[] res4;
        }
    };
}