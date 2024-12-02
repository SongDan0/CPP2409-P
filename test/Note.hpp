#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <limits>
using namespace std;

class Note {
public:
    // 생성자
    Note();
private:
    // 메모장 벡터
    vector<vector<string>>note;
    
    // 현재 페이지
    int current_page = 0;

    // 비밀번호
    string password = ""; 

    // 메모장 표시
    void PrintNote();

    // 메모장 할당
    void AllocationMemo();

    // 저장
    void SaveMemo();

    // 불러오기
    void LoadMemo();

    // 메모 추가
    void AddMemo(string detail);

    // 메모 삭제
    void DelMemo(string detail);

    // 다음 메모
    void NextMemo();

    // 이전 메모
    void PriviousMemo();

    // page번째 메모로 이동
    void PageMemo(int page);

    // 비밀번호 설정
    void SetPassword(string password);

    // 메뉴
    void Menu();
};

// Caesar Cipher 암호화 (모든 문자 포함)
string EncryptCaesar(string text, int shift = 10);

// Caesar Cipher 복호화
string DecryptCaesar(string text);
