#include "whatday.h"

const language eLanguageJapanese =
{
    .lang_name = "Japanese",
    .usage = "使用方法: プログラムを実行後、'言語' と '年頭からの日数' を入力してください。\n",
    .prompt_day = "1年のうちの日数を選んでください (1-365): ",
    .result_month = "該当する月は: %s\n",
    .result_day = "該当する日は: %d\n",
    .err_invalid = "エラー: 1から365の間の数値を入力してください。\n",
    .months = {
        "1月", "2月", "3月", "4月", "5月", "6月",
        "7月", "8月", "9月", "10月", "11月", "12月"
    }
};