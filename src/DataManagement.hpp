#pragma once
#include <iostream>
#include <fstream>
#include <regex>
#include "DataType.hpp"
#include "Date.hpp"

class DataManagement: public Date {
private:
    Schedule*** calendar;           // 날짜별 Schedule 객체를 담을 3차원 배열의 포인터
    Schedule*** planner;            // 날짜별 Schedule 객체를 담을 3차원 배열의 포인터   
    Transaction*** account_book;     // 날짜별 Transaction 객체를 담을 3차원 배열의 포인터   

    // 생성자(Date 생성자 호출, 연도 단위 날짜 할당, 환경 설정 파일 가져오기)(Singleton)
    DataManagement();
public:
    int year_index = -1;            // 현재 할당된 연도의 인덱스의 최댓값을 저장할 변수

    // 싱글턴 객체 반환
    static DataManagement& GetInstance();

    //calendar배열 반환
    Schedule*** GetArrayCalendar();

    //planner배열 반환
    Schedule*** GetArrayPlanner();
    
    //accountBook배열 반환
    Transaction*** GetArrayAccountBook();
    
    // 달력 범위 1년 증가(다음 연도 할당)
    void AddYear();

    // 환경 설정 파일 저장
    void SaveConfig();

    // 환경 설정 파일 불러오기
    void LoadConfig();

    // 데이터 파일 저장
    void SaveToFile();

    // 데이터 파일 불러오기
    void LoadFromFile();
};

// 시작과 끝의 공백을 제거하는 함수
string Trim(const string& str);

// 문자열의 출력 길이 반환하는 함수
int StringLength(const string& str);
