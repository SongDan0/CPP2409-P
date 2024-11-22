#include "DataType.hpp"

// 일정 추가
void Schedule::AddSchedule(string detail) {
    schedule.push_back(detail);
}

// 일정 삭제
void Schedule::DelSchedule(string detail) {
    schedule.erase(remove(schedule.begin(), schedule.end(), detail), schedule.end());
}

// 일정vector 반환
vector<string> Schedule::GetScheduleVector() {
    return schedule;
}

// index번째 일정 반환
string Schedule::GetScheduleString(int index) {
    return schedule[index];
}

// 일정 개수 반환
int Schedule::CountSchedule() {
    return schedule.size();
}

// 일정이 비어있는지 확인
bool Schedule::EmptySchedule() {
    return schedule.empty();
}
/*
// 거래 내역 추가
void Transaction::AddTransaction(string detail, int price) {
    transactionDetail.push_back(detail);
    transactionPrice.push_back(price);
}

// 거래 내역 삭제
void Transaction::DelTransaction(string detail, int price) {
    transactionDetail.erase(remove(transactionDetail.begin(), transactionDetail.end(), transactionDetail), transactionDetail.end());
    transactionPrice.erase(remove(transactionPrice.begin(), transactionPrice.end(), transactionPrice), transactionPrice.end());
}

// 거래 상세내용 벡터 반환
vector<string> Transaction::GetDetailVector() {
   return transactionDetail;
}

// 거래 가격 벡터 반환
vector<int> Transaction::GetPriceVector() {
   return transactionPrice;
}

// index번째 거래 상세내용 반환
string Transaction::GetDetailString(int index) {
    return transactionDetail[index];
}

// index번째 거래 가격 반환
int Transaction::GetPriceString(int index) {
    return transactionPrice[index];
}

//일정 개수 반환
int Transaction::CountSchedule() {
    return transactionDetail.size();
}

//일정이 비어있는지 확인
bool Transaction::EmptySchedule() {
    return transactionDetail.empty();
}
*/