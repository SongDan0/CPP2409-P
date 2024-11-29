#pragma once
#include "DataManagement.hpp"

class Planner {
private:
    DataManagement* bd;                             // DataManagement 포인터
    Schedule*** date;                               // 날짜별 Schedule 객체를 담을 3차원 배열의 포인터
    int current_year, current_month, current_day;   // 현재 작업 중인 날짜 저장하는 변수
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