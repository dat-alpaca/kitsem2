#pragma once

#define WHATDAY_OK 0
#define WHATDAY_NG 1

struct language
{
    const char* lang_name;
    const char* usage;
    const char* prompt_day;
    const char* result_month;
    const char* result_day;
    const char* err_invalid;
    const char* months[12];
} typedef language;

int whatday(int dayOfYear, int* outMonth, int* outDay);

extern const language eLanguageEnglish;
extern const language eLanguageJapanese;