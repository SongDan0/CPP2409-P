#include "DataType.hpp"

// 일정 추가
void Schedule::AddSchedule(string detail) {
    schedule.push_back(detail);
}

// 일정 삭제
void Schedule::DelSchedule(string detail) {
    schedule.erase(
        remove(schedule.begin(), schedule.end(), detail), 
        schedule.end());
}

// 일정vector 반환
vector<string> Schedule::GetSchedule() {
    return schedule;
}

// index번째 일정 반환
string Schedule::GetSchedule(int index) {
    return schedule[index];
}

// 일정 개수 반환
int Schedule::CountSchedule() {
    return schedule.size();
}

// 일정이 비어있는지 확인
bool Schedule::IsEmpty() {
    return schedule.empty();
}

// 거래내역 추가
void Transaction::AddTransaction(string detail, int price) {
    transaction_detail.push_back(detail);
    transaction_price.push_back(to_string(price));
}

// 거래내역 삭제
void Transaction::DelTransaction(string detail, string price) {
    transaction_detail.erase(
        remove(transaction_detail.begin(), transaction_detail.end(), detail), 
        transaction_detail.end());
    transaction_price.erase(
        remove(transaction_price.begin(), transaction_price.end(), price), 
        transaction_price.end());
}

// 거래내용 vector 반환
vector<string> Transaction::GetDetail() {
    return transaction_detail;
}

// 거래가격 vector 반환
vector<string> Transaction::GetPrice() {
    return transaction_price;
}

// index번째 거래내용 반환
string Transaction::GetDetail(int index) {
    return transaction_detail[index];
}

// index번째 거래가격 반환
string Transaction::GetPrice(int index) {
    return transaction_price[index];
}

// 거래내역 개수 반환
int Transaction::CountTransaction() {
    return transaction_detail.size();
}

// 거래내역이 비어있는지 확인
bool Transaction::IsEmpty() {
    return transaction_detail.empty();
}