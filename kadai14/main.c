#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "whatday.h"

const language* gLanguages[] = { &eLanguageEnglish, &eLanguageJapanese };
const int gLanguageAmount = 2;

int whatday(int dayOfYear, int* outputMonth, int* outputDay)
{
    if (dayOfYear < 1 || dayOfYear > 365)
        return WHATDAY_NG;
    
    static const int days_in_months[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    
    int month = 0;
    int day = dayOfYear;
    while (day > days_in_months[month])
    {
        day -= days_in_months[month];
        ++month;
    }

    *outputMonth = month;
    *outputDay = day;

    return WHATDAY_OK;
}

void convert_and_validate(const language* language, int dayOfYear, int* month, int* day)
{
    /* Responsibilities not clear. */
    if (whatday(dayOfYear, month, day) == WHATDAY_NG)
    {
        fprintf(stderr, "%s", language->err_invalid);
        exit(EXIT_FAILURE);
    }
}

void display_date(const language* language, int month, int day)
{
    printf(language->result_month, language->months[month]);
    printf(language->result_day, day);
}

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        fprintf(stderr, "usage: %s <language>\n", argv[0]);
        return 1;
    }
    char* inputLanguage = argv[1];

    const language* currentLanguage = NULL;
    for (int i = 0; i < gLanguageAmount; i++)
    {
        if (strcasecmp(gLanguages[i]->lang_name, inputLanguage) != 0)
            continue;

        currentLanguage = gLanguages[i];
        break;
    }

    // Defaults to english:
    if (!currentLanguage)
        currentLanguage = &eLanguageEnglish;

    {
        printf(currentLanguage->prompt_day, "");
        int inputDay;
        scanf("%d", &inputDay);
        
        int month = 0, day = 0;
        convert_and_validate(currentLanguage, inputDay, &month, &day);
        display_date(currentLanguage, month, day);
    }

    return EXIT_SUCCESS;
}