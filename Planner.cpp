#include "Planner.hpp"

// 생성자(Date 클래스 생성자 호출)
Planner::Planner() {
    bd = &DataManagement::GetInstance();
    date = bd->GetArrayPlanner();
    current_year = bd->current_year;
    current_month = bd->current_month;
    current_day = bd->current_day;
}

// 플래너 일정 추가
void Planner::AddSchedule(string detail){
    // 프로그램 시작 시 일정이 있는 날짜까지만 할당하기 위해 변수값 변경
    if(bd->max_year_index < current_year - bd->initial_year)
        bd->max_year_index = current_year-bd->initial_year;
    
    // 일정 추가
    date[current_year-bd->initial_year][current_month][current_day].AddSchedule(detail);
    
    // 변경 사항 저장
    bd->SaveToFile();
    bd->SaveConfig();
}

//플래너 일정 삭제
void Planner::DelSchedule(string detail) {
    // 일정 삭제
    date[current_year-bd->initial_year][current_month][current_day].DelSchedule(detail);
    
    // 변경 사항 저장
    bd->SaveToFile();
    bd->SaveConfig();
}

//플래너 표시
void Planner::PrintPlanner() {
    // 날짜 유효한지 체크
    if(!bd->CheckRange(current_year, current_month, current_day)) {
        cout << "날짜가 유효하지 않습니다.";
        return;
    }

    // 현재 표시할 날짜가 할당이 되어 있는지 체크하고 할당
    for(; bd->year_index < current_year - bd->initial_year;) {
        bd->AddYear();
    }
    
    // 연도, 월, 일, 요일 출력
    if(current_month < 10) {
        if(current_day < 10)
            cout << " ──────────" << current_year << ".0" << current_month << ".0" << current_day << "(" << Date::kWeekdays[bd->GetDayOfWeek(current_year, current_month, current_day)] << ")────────────" << endl;
        else
            cout << " ──────────" << current_year << ".0" << current_month << "." << current_day << "(" << Date::kWeekdays[bd->GetDayOfWeek(current_year, current_month, current_day)] << ")────────────" << endl;
    }
    else {
        if(current_day < 10)
            cout << " ──────────" << current_year << "." << current_month << ".0" << current_day << "(" << Date::kWeekdays[bd->GetDayOfWeek(current_year, current_month, current_day)] << ")────────────" << endl;
        else
            cout << " ──────────" << current_year << "." << current_month << "." << current_day << "(" << Date::kWeekdays[bd->GetDayOfWeek(current_year, current_month, current_day)] << ")────────────" << endl;
    }

    // 일정 출력
    PrintSchedule();
}

// 일정 출력
void Planner::PrintSchedule() {
    // 위 경계선 출력
    cout << "┌────────────────────────────────────────┐" << endl;
    
    // 일정 출력 및 오른쪽 공백 출력
    for(int i = 0; i < date[current_year-bd->initial_year][current_month][current_day].CountSchedule(); i++) {
        cout << "│" << date[current_year-bd->initial_year][current_month][current_day].GetScheduleString(i);
        for(int j = date[current_year-bd->initial_year][current_month][current_day].GetScheduleString(i).length()*2/3; j < 40; j++)
            cout << " ";
        cout << "│" << endl;
    }
    
    // 아래쪽 공백 출력
    for(int i = date[current_year-bd->initial_year][current_month][current_day].CountSchedule(); i < 24; i++)
        cout  << "│                                        │" <<endl;
    
    // 아래 경계선 출력
    cout << "└────────────────────────────────────────┘" << endl;
}   

// 메뉴(사용자 입력 받아 작업 실행)
void Planner::Menu() {
    string input;
            system("cls");
            PrintPlanner();
    while(true) {
        // 날짜 이동, 일정 추가, 일정 삭제, 캘린더 중 하나 사용자 입력 받기
        cout << "날짜 이동, 일정 추가, 일정 삭제, 뒤로가기 중 하나를 선택해서 입력해주세요(뒤로 가기는 캘린더로 이동): ";
        getline(cin, input);
        input.erase(std::remove(input.begin(), input.end(), ' '), input.end());            
        // 날짜 이동
        if(input == "날짜이동") {
            cout << "0000 00 00형태로 연도, 월, 일을 입력해주세요: ";
            cin >> current_year >> current_month >> current_day;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            system("cls");
            PrintPlanner();
        }
        // 일정 추가
        else if(input == "일정추가") {
            string detail;
            cout << "20글자 이내의 한글로 일정을 입력해주세요(띄어쓰기 사용금지): ";
            getline(cin, detail);
            AddSchedule(detail);
            system("cls");
            PrintPlanner();
        }
        // 일정 삭제
        else if(input == "일정삭제") {
            string detail;
            cout << "9글자 이내의 한글로 일정을 입력해주세요(띄어쓰기 사용금지): ";
            getline(cin, detail);
            DelSchedule(detail);
            system("cls");
            PrintPlanner();
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

