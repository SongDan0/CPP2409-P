#include "Date.hpp"

class AccountBook {
public:
    // 생성자(Date 생성자 호출)
    AccountBook();

    // 가계부 내역 추가
    void AddTransaction(string detail);

    // 가계부 내역 삭제
    void DelTransaction(string detail);

    // 가계부 표시
    void PrintAccountBook();

    // 내역 출력
    void Menu() ;
};