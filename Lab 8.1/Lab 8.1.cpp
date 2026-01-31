#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstring>

using namespace std;

// Функція для підрахунку входжень "while"
int CountWhile(const char* s)
{
    size_t len = strlen(s);
    int count = 0;
    // Використовуємо i + 4 < len, щоб не вийти за межі при перевірці 5 символів
    for (size_t i = 0; i + 4 < len; )
    {
        if (strncmp(s + i, "while", 5) == 0)
        {
            count++;
            i += 5; // Пропускаємо знайдене слово
        }
        else
            i++;
    }
    return count;
}

// Функція для заміни "while" на "*****"
char* ChangeWhile(const char* s)
{
    size_t len = strlen(s);
    // Виділяємо достатньо пам'яті (у гіршому випадку "while" не збільшує довжину, 
    // але для безпеки беремо з запасом)
    char* res = new char[len + 1];

    size_t i = 0, j = 0;
    while (s[i] != '\0')
    {
        if (i + 4 < len && strncmp(s + i, "while", 5) == 0)
        {
            memcpy(res + j, "*****", 5);
            j += 5;
            i += 5;
        }
        else
        {
            res[j++] = s[i++];
        }
    }
    res[j] = '\0';
    return res;
}

int main()
{
    char str[101];
    cout << "Enter string: ";
    cin.getline(str, 100);

    // Виклик функцій
    cout << "Count of 'while': " << CountWhile(str) << endl;

    char* modifiedStr = ChangeWhile(str);
    cout << "Modified string: " << modifiedStr << endl;

    // ОБОВ'ЯЗКОВО звільняємо пам'ять, яку виділили в ChangeWhile
    delete[] modifiedStr;

    return 0;
}