#pragma once
#include <chrono>
#include <string>
using namespace std;

class Date {
public:
    static const int MAX_YEARS;                     // 최대 연도수
    static const int MAX_MONTHS;                    // 최대 개월수
    static const int MAX_DAYS;                      // 최대 일수
    static const int kDays[12];                     // 각 월의 최대 일수
    static const int kDaysLeafYear[12];             // 각 월의 최대 일수
    static const string kWeekdays[7];               // 요일 배열
    int current_year;                        // 현재 연도 변수
    int current_month;                       // 현재 월 변수
    int current_day;                         // 현재 일 변수
    int initial_year;           // 0번 인덱스의 연도(시작 연도)
    int max_year_index = 0;        // 프로그램 시작 시 할당할 연도의 인덱스의 최댓값을 저장하는 변수
    //생성자
    Date();

    //윤년 체크(true: 윤년/ flase: 윤년x)
    bool CheckLeapYear(int year);

    //날짜 유효한지 체크(true: 날짜 유효/ flase: 날짜 유효x)
    bool CheckRange(int year, int month, int day);

    //해당 날짜의 요일 반환(Zeller의 공식)(0=일요일, 1=월요일, ..., 6=토요일)
    int GetDayOfWeek(int year, int month, int day);
};