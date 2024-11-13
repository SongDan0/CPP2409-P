#include "Planner.hpp"

class Calendar: public Date{
private:
    Planner* plan;
public:
    //생성자(Date 생성자 호출)
    Calendar();

    // 캘린더 일정 추가
    void addSchedule(int year_s, int month_s, int day_s, int year_e, int month_e, int day_e, string detail);

    // 캘린더 일정 삭제
    void delSchedule(int year_s, int month_s, int day_s, int year_e, int month_e, int day_e, string detail);

    // 캘린더 표시
    void printCalendar(int year, int month, int day = 1);

    // 일정 출력
    void printSchedule(int year, int month, int day_s, int day_e, const int* Days, int space_position = 0);

    //메뉴
    void menu() ;

};