#include "whatday.h"

const language eLanguageEnglish =
{
    .lang_name = "English",
    .usage = "Usage: program\nThen enter 'Language' and 'Day of Year'.\n",
    .prompt_day = "Choose a day of the year (1-365): ",
    .result_month = "The corresponding month is: %s\n",
    .result_day = "The corresponding day is: %d\n",
    .err_invalid = "Error: Please enter a number between 1 and 365.\n",
    .months = {
        "January", "February", "March", "April", "May", "June",
        "July", "August", "September", "October", "November", "December"
    }
};