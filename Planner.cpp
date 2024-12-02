#include "Planner.hpp"

// 생성자(Date 클래스 생성자 호출)
Planner::Planner() {
    dm = &DataManagement::GetInstance();
    date = dm->GetArrayPlanner();
    current_year = dm->current_year;
    current_month = dm->current_month;
    current_day = dm->current_day;
}

// 플래너 일정 추가
void Planner::AddSchedule(string detail){
    // 프로그램 시작 시 일정이 있는 날짜까지만 할당하기 위해 변수값 변경
    if(dm->max_year_index < current_year - dm->initial_year)
        dm->max_year_index = current_year - dm->initial_year;
    
    // 일정 추가
    date[current_year-dm->initial_year][current_month-1][current_day-1].AddSchedule(detail);
    
    // 변경 사항 저장
    dm->SaveToFile();
    dm->SaveConfig();
}

//플래너 일정 삭제
void Planner::DelSchedule(string detail) {
    // 일정 삭제
    date[current_year-dm->initial_year][current_month-1][current_day-1].DelSchedule(detail);
    
    // 변경 사항 저장
    dm->SaveToFile();
    dm->SaveConfig();
}

//플래너 표시
void Planner::PrintPlanner() {
    system("cls");
    // 날짜 유효한지 체크
    if(!dm->CheckRange(current_year, current_month, current_day)) {
        cout << "날짜가 유효하지 않습니다.";
        return;
    }

    // 현재 표시할 날짜가 할당이 되어 있는지 체크하고 할당
    for(; dm->year_index < current_year-dm->initial_year;) {
        dm->AddYear();
    }
    
    // 연도, 월, 일, 요일 출력
    if(current_month < 10) {
        if(current_day < 10) {
            cout << " ──────────" << current_year 
            << ".0" << current_month 
            << ".0" << current_day 
            << "(" << Date::WEEKDAYS[dm->GetDayOfWeek(current_year, current_month, current_day)] 
            << ")────────────" << endl;
        }
        else {
            cout << " ──────────" << current_year 
            << ".0" << current_month 
            << "." << current_day 
            << "(" << Date::WEEKDAYS[dm->GetDayOfWeek(current_year, current_month, current_day)] 
            << ")────────────" << endl;
        }
    }
    else {
        if(current_day < 10) {
            cout << " ──────────" << current_year 
            << "." << current_month 
            << ".0" << current_day 
            << "(" << Date::WEEKDAYS[dm->GetDayOfWeek(current_year, current_month, current_day)] 
            << ")────────────" << endl;
        }
        else {
            cout << " ──────────" << current_year 
            << "." << current_month 
            << "." << current_day 
            << "(" << Date::WEEKDAYS[dm->GetDayOfWeek(current_year, current_month, current_day)] 
            << ")────────────" << endl;
        }
    }

    // 일정 출력
    PrintSchedule();
}

// 일정 출력
void Planner::PrintSchedule() {
    // 위 경계선 출력
    cout << "┌────────────────────────────────────────┐" << endl;
    // 일정 출력 및 오른쪽 공백 출력
    for(int i = 0; i < date[current_year-dm->initial_year][current_month-1][current_day-1].CountSchedule(); i++) {
        cout << "│" << date[current_year-dm->initial_year][current_month-1][current_day-1].GetSchedule(i);
        for(int j = StringLength(date[current_year-dm->initial_year][current_month-1][current_day-1].GetSchedule(i)); j < 40; j++)
            cout << " ";
        cout << "│" << endl;
    }
    
    // 아래쪽 공백 출력
    for(int i = date[current_year-dm->initial_year][current_month-1][current_day-1].CountSchedule(); i < 24; i++)
        cout  << "│                                        │" <<endl;
    
    // 아래 경계선 출력
    cout << "└────────────────────────────────────────┘" << endl;
}   

// 메뉴(사용자 입력 받아 작업 실행)
void Planner::Menu() {
    string input;
    PrintPlanner();

    while(true) {
        try {
            // 사용자 입력
            cout << "날짜 이동, 일정 추가, 일정 삭제, 뒤로가기 중 하나를 선택해서 입력해주세요: ";
            getline(cin, input);
            input.erase(std::remove(input.begin(), input.end(), ' '), input.end());

            // 날짜 이동
            if(input == "날짜이동") {
                // 사용자 입력
                cout << "0000 00 00형태로 연도, 월, 일을 입력해주세요: ";
                cin >> current_year >> current_month >> current_day;
                // 처리
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                if(cin.fail()){
                    cin.clear();
                    throw runtime_error("날짜를 잘못입력하였습니다.");
                }
                PrintPlanner();
            }
            // 일정 추가
            else if(input == "일정추가") {
                // 사용자 입력
                string detail;
                cout << "40글자 이내로 일정을 입력해주세요(한글: 2칸, 그 외: 1칸): ";
                getline(cin, detail);
                // 처리
                detail = Trim(detail);
                AddSchedule(detail);
                PrintPlanner();
            }
            // 일정 삭제
            else if(input == "일정삭제") {
                // 사용자 입력
                string detail;
                cout << "40글자 이내로 일정을 입력해주세요(한글: 2칸, 그 외: 1칸): ";
                getline(cin, detail);
                // 처리
                detail = Trim(detail);
                DelSchedule(detail);
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
        catch(runtime_error e) {
            cout << e.what() << endl;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}