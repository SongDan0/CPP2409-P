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
    vector<string> GetScheduleVector();

    // index번째 일정 반환
    string GetScheduleString(int index);

    // 일정 개수 반환
    int CountSchedule();

    // 일정이 비어있는지 확인
    bool EmptySchedule();
};

class Transaction {
private:    
    // 거래 상세내용 벡터
    vector<string> transactionDetail;
    // 거래 가격 벡터
    vector<int> transactionPrice;  
public: 
    // 거래 내역 추가
    void AddTransaction(string detail, int price);

    // 거래 내역 삭제
    void DelTransaction(string detail, int price);

    // 거래 상세내용 벡터 반환
    vector<string> GetDetailVector();

    // 거래 가격 벡터 반환
    vector<int> GetPriceVector();

    // index번째 거래 상세내용 반환
    string GetDetailString(int index);

    // index번째 거래 가격 반환
    int GetPriceString(int index);

    //일정 개수 반환
    int CountSchedule();

    //일정이 비어있는지 확인
    bool EmptySchedule();
};