#include "Date.hpp"

// static const 변수 초기화
const int Date::MAX_YEARS = 100;                                                 
const int Date::MAX_MONTHS = 12;                                                           
const int Date::MAX_DAYS = 31;                                            
const int Date::DAYS[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}; 
const int Date::DAYS_LEAF_YEAR[12] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
const string Date::WEEKDAYS[7] = {"일요일", "월요일", "화요일", "수요일", "목요일", "금요일", "토요일"};

// 생성자(현재 시간 저장, 연도 단위 날짜 할달, 환경 설정 파일 가져오기)
Date::Date() {
    // 현재 시간 가져오기
    time_t t = time(nullptr);            // 현재 시간을 time_t 타입으로 얻기
    tm* now = localtime(&t);             // 현재 시간을 tm 구조체 포인터로 변환

    // 현재 년, 월, 일을 변수에 저장
    current_year = now->tm_year + 1900;  // tm_year는 1900년부터 시작하므로 1900을 더해 실제 연도 계산
    current_month = now->tm_mon + 1;     // tm_mon은 0부터 시작하므로 1을 더해 실제 월 계산
    current_day = now->tm_mday;          // 오늘의 일(day)
}

// 윤년 체크(true: 윤년/ flase: 윤년x)
bool Date::CheckLeapYear(int year) {
    if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)
        return true;
    return false;
}

// 날짜 유효한지 체크(true: 날짜 유효/ flase: 날짜 유효x)
bool Date::CheckRange(int year, int month, int day) {
    if(initial_year <= year && year <= initial_year + 99) {     // 연도 체크
        if(1 <= month  && month <= MAX_MONTHS) {                // 월 체크
            if (CheckLeapYear(year)){                           // 윤년 판단(일 체크)
                if(1 <= day && day <= DAYS_LEAF_YEAR[month-1])     
                    return true;
            }
            else {
                if(1 <= day && day <= DAYS[month-1])
                    return true;
            }
        }
    }
    return false;
}

// 해당 날짜의 요일 반환(Zeller의 공식)(0=일요일, 1=월요일, ..., 6=토요일)
int Date::GetDayOfWeek(int year, int month, int day) {
    // 1월과 2월을 13월, 14월로 처리
    if (month < 3) {
        month += 12;
        year--;
    }
    int K = year % 100;         // 연도의 마지막 두 자리
    int J = year / 100;         // 연도의 첫 두 자리
    
    // Zeller의 공식
    int h = (day + (13 * (month + 1)) / 5 + K + K / 4 + J / 4 + 5 * J) % 7;
    h = (h + 6) % 7;            // h가 음수일 경우 양수로 변환

    return h;                   // 요일 인덱스값 반환
}