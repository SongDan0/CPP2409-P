#include <iostream>
#include <chrono>
#include <fstream>
#include "data_type.hpp"

class Calendar {
private:
    int initial_year = 0;                                                                                           // 시작 연도
    int maxYearIndex = -1;                                                                                          // 현재 채워진 연도 인덱스의 최대값
    const int MAX_YEARS = 100;                                                                                      // 최대 연도수
    const int MAX_MONTHS = 12;                                                                                      // 최대 개월수
    const int MAX_DAYS = 31;                                                                                        // 최대 일수
    const int Days_m[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};                                        // 각 월의 최대 일수
    const int Days_M[12] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};                                        // 각 월의 최대 일수
    const string weekdays[7] = {"일요일", "월요일", "화요일", "수요일", "목요일", "금요일", "토요일"};                  // 요일 배열
    Schedule*** calendar;                                                                                           // 년, 월, 일의 일정을 담을 배열의 포인터
public:
    //생성자(달력 해당 연도만 할당)
    Calendar();

    //달력 범위 1년 증가(다음 연도 할당)
    void addYear();

    //윤년 체크
    bool checkLeapYear(int year);

    //날짜 유효한지 체크(true: 날짜 유효/ flase: 날짜 유효x)
    bool check_range(int year, int month, int day);

    //캘린더 일정 추가
    void addSchedule(int year_s, int month_s, int day_s, int year_e, int month_e, int day_e, string detail);

    //일정 삭제
    void delSchedule(int year_s, int month_s, int day_s, int year_e, int month_e, int day_e, string detail);

    //해당 날짜의 요일 반환(Zeller의 공식)(0=일요일, 1=월요일, ..., 6=토요일)
    int getDayOfWeek(int year, int month, int day);

    //캘린더 표시
    void printCalendar(int year, int month, int day = 1);

    //일정 출력
    void printSchedule(int year, int month, int day_s, int day_e, const int* Days, int space_position = 0);

    //데이터 파일 저장
    void saveToFile(const string& filename);

    //데이터 파일 불러오기
    void loadFromFile(const string& filename);
    
    //환경 설정 파일 저장
    void saveConfig();

    //환경 설정 파일 불러오기
    void loadConfig();
    
    //메뉴
    void menu() ;

};