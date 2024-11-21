#include <iostream>
#include <chrono>
#include <fstream>
#include "Schedule.hpp"


class Date {
public:
    static const int MAX_YEARS;                 // 최대 연도수
    static const int MAX_MONTHS;                // 최대 개월수
    static const int MAX_DAYS;                  // 최대 일수
    static const int kDays[12];                 // 각 월의 최대 일수
    static const int kDaysLeafYear[12];         // 각 월의 최대 일수
    static const string kWeekdays[7];           // 요일 배열
protected:
    int initial_year = 0;                           // 0번 인덱스의 연도(시작 연도)
    int max_year_index = -1;                        // 프로그램 시작 시 할당할 연도의 인덱스의 최댓값을 저장하는 변수
    int year_index = -1;                            // 현재 할당된 연도의 인덱스의 최댓값을 저장할 변수
    int current_year, current_month, current_day;   // 현재 작업 중인 날짜 저장하는 변수
    Schedule*** date;                               // 날짜별 Schedule 객체를 담을 3차원 배열의 포인터   

    //생성자(현재 시간 저장, 연도 단위 날짜 할달, 환경 설정 파일 가져오기)
    Date(string config, string filename);

    //달력 범위 1년 증가(다음 연도 할당)
    void AddYear();

    //윤년 체크(true: 윤년/ flase: 윤년x)
    bool CheckLeapYear(int year);

    //날짜 유효한지 체크(true: 날짜 유효/ flase: 날짜 유효x)
    bool CheckRange(int year, int month, int day);

    //해당 날짜의 요일 반환(Zeller의 공식)(0=일요일, 1=월요일, ..., 6=토요일)
    int GetDayOfWeek(int year, int month, int day);

    //환경 설정 파일 저장
    void SaveConfig(string config);

    //환경 설정 파일 불러오기
    void LoadConfig(string config);

    //데이터 파일 저장
    void SaveToFile(string filename);

    //데이터 파일 불러오기
    void LoadFromFile(string filename);
};