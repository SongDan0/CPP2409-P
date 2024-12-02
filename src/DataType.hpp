#pragma once
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

class Schedule {
private:    
    // 일정 벡터
    vector<string> schedule;  
public: 
    // 일정 추가
    void AddSchedule(string detail);

    // 일정 삭제
    void DelSchedule(string detail);

    // 일정vector 반환
    vector<string> GetSchedule();

    // index번째 일정 반환
    string GetSchedule(int index);

    // 일정 개수 개수 반환
    int CountSchedule();

    // 일정이 비어있는지 확인
    bool IsEmpty();
};

class Transaction {
private:    
    // 거래내용 벡터
    vector<string> transaction_detail;
    // 거래가격 벡터
    vector<string> transaction_price;  
public: 
    // 거래내역 추가
    void AddTransaction(string detail, int price);

    // 거래내역 삭제
    void DelTransaction(string detail, string price);

    // 거래내용 vector 반환
    vector<string> GetDetail();

    // 거래가격 vector 반환
    vector<string> GetPrice();

    // index번째 거래내용 반환
    string GetDetail(int index);

    // index번째 거래가격 반환
    string GetPrice(int index);

    // 거래내역 개수 반환
    int CountTransaction();

    // 거래내역이 비어있는지 확인
    bool IsEmpty();
};