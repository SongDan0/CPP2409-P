#pragma once
#include "DataManagement.hpp"

class AccountBook {
private:
    DataManagement* bd;                             // DataManagement 포인터
    Transaction*** date;                            // 날짜별 Schedule 객체를 담을 3차원 배열의 포인터
    int current_year, current_month, current_day;   // 현재 작업 중인 날짜 저장하는 변수
public:
    // 생성자(Date 생성자 호출)
    AccountBook();

    // 가계부 내역 추가
    void AddTransaction(string detail, int price);

    // 가계부 내역 삭제
    void DelTransaction(string detail, int price);

    // 합계 반환
    string GetTotal(int year, int month, int day);

    // 가계부 표시
    void PrintAccountBook();

    // 내역 출력
    void PrintTransaction();

    // 메뉴
    void Menu();
};