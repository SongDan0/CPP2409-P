#include "Date.hpp"

class Planner: public Date{
public:
    // 생성자(Date 생성자 호출)
    Planner();

    // 플래너 일정 추가
    void AddSchedule(string detail);

    // 플래너 일정 삭제
    void DelSchedule(string detail);

    // 플래너 표시
    void PrintPlanner();

    // 일정 출력
    void PrintSchedule();

    // 메뉴
    void Menu() ;

};