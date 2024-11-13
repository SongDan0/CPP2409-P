#include "Planner.hpp"

// 생성자(Date 클래스 생성자 호출)
Planner::Planner(): Date("config_p", "schedule_p") {}

// 플래너 일정 추가
void Planner::addSchedule(string detail){
    // 프로그램 시작 시 일정이 있는 날짜까지만 할당하기 위해 변수값 변경
    if(maxYearIndex < currentYear - initial_year)
        maxYearIndex = currentYear-initial_year;
    
    // 일정 추가
    date[currentYear-initial_year][currentMonth][currentDay].addSchedule(detail);
    
    // 변경 사항 저장
    saveToFile("schedule_p");
    saveConfig("config_p");
}

//플래너 일정 삭제
void Planner::delSchedule(string detail) {
    // 일정 삭제
    date[currentYear-initial_year][currentMonth][currentDay].delSchedule(detail);
    
    // 변경 사항 저장
    saveToFile("schedule_p");
    saveConfig("config_p");
}

//플래너 표시
void Planner::printPlanner() {
    // 날짜 유효한지 체크
    if(!check_range(currentYear, currentMonth, currentDay)) {
        cout << "날짜가 유효하지 않습니다.";
        return;
    }

    // 현재 표시할 날짜가 할당이 되어 있는지 체크하고 할당
    for(; yearIndex < currentYear - initial_year;) {
        addYear();
    }
    
    // 연도, 월, 일, 요일 출력
    if(currentMonth < 10) {
        if(currentDay < 10)
            cout << " ──────────" << currentYear << ".0" << currentMonth << ".0" << currentDay << "(" << weekdays[getDayOfWeek(currentYear, currentMonth, currentDay)] << ")────────────" << endl;
        else
            cout << " ──────────" << currentYear << ".0" << currentMonth << "." << currentDay << "(" << weekdays[getDayOfWeek(currentYear, currentMonth, currentDay)] << ")────────────" << endl;
    }
    else {
        if(currentDay < 10)
            cout << " ──────────" << currentYear << "." << currentMonth << ".0" << currentDay << "(" << weekdays[getDayOfWeek(currentYear, currentMonth, currentDay)] << ")────────────" << endl;
        else
            cout << " ──────────" << currentYear << "." << currentMonth << "." << currentDay << "(" << weekdays[getDayOfWeek(currentYear, currentMonth, currentDay)] << ")────────────" << endl;
    }

    // 일정 출력
    printSchedule();
}

// 일정 출력
void Planner::printSchedule() {
    // 위 경계선 출력
    cout << "┌────────────────────────────────────────┐" << endl;
    
    // 일정 출력 및 오른쪽 공백 출력
    for(int i = 0; i < date[currentYear-initial_year][currentMonth][currentDay].countSchedule(); i++) {
        cout << "│" << date[currentYear-initial_year][currentMonth][currentDay].getSchedule_s(i);
        for(int j = date[currentYear-initial_year][currentMonth][currentDay].getSchedule_s(i).length()*2/3; j < 40; j++)
            cout << " ";
        cout << "│" << endl;
    }
    
    // 아래쪽 공백 출력
    for(int i = date[currentYear-initial_year][currentMonth][currentDay].countSchedule(); i < 24; i++)
        cout  << "│                                        │" <<endl;
    
    // 아래 경계선 출력
    cout << "└────────────────────────────────────────┘" << endl;
}   

// 메뉴(사용자 입력 받아 작업 실행)
void Planner::menu() {
    string input;
            system("cls");
            printPlanner();
    while(true) {
        // 날짜 이동, 일정 추가, 일정 삭제, 캘린더 중 하나 사용자 입력 받기
        cout << "날짜 이동, 일정 추가, 일정 삭제, 뒤로가기 중 하나를 선택해서 입력해주세요(뒤로 가기는 캘린더로 이동): ";
        getline(cin, input);
        input.erase(std::remove(input.begin(), input.end(), ' '), input.end());            
        // 날짜 이동
        if(input == "날짜이동") {
            cout << "0000 00 00형태로 연도, 월, 일을 입력해주세요: ";
            cin >> currentYear >> currentMonth >> currentDay;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            system("cls");
            printPlanner();
        }
        // 일정 추가
        else if(input == "일정추가") {
            string detail;
            cout << "20글자 이내의 한글로 일정을 입력해주세요(띄어쓰기 사용금지): ";
            getline(cin, detail);
            addSchedule(detail);
            system("cls");
            printPlanner();
        }
        // 일정 삭제
        else if(input == "일정삭제") {
            string detail;
            cout << "9글자 이내의 한글로 일정을 입력해주세요(띄어쓰기 사용금지): ";
            getline(cin, detail);
            delSchedule(detail);
            system("cls");
            printPlanner();
        }
        // 뒤로 가기
        else if(input == "뒤로가기") {
            
            break;
        }
        // 잘못된 입력
        else {
            cout << "잘못된 입력입니다.";
        }
    }
}

