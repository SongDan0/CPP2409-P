#include "Calendar.hpp"

// 생성자(달력 해당 연도만 할당)
Calendar::Calendar() {
    dm = &DataManagement::GetInstance();
    date = dm->GetArrayCalendar();
    current_year = dm->current_year;
    current_month = dm->current_month;
    current_day = dm->current_day;
    plan = new Planner{};
    account_book = new AccountBook{};
}

// 캘린더 일정 추가
void Calendar::AddSchedule(int year_s, int month_s, int day_s, int year_e, int month_e, int day_e, string detail){
    // 날짜 유효성 체크(유효하지 않으면 함수 종료)
    if(!dm->CheckRange(year_s, month_s, day_s) || !dm->CheckRange(year_e, month_e, day_e)) {
        cout << "날짜가 유효하지 않습니다.";
        return;
    }
    // 시작 날짜 < 종료 날짜가 유효한지 체크
    if (year_s > year_e || 
    (year_s == year_e && month_s > month_e) || 
    (year_s == year_e && month_s == month_e && day_s > day_e)) {
        cout << "날짜가 유효하지 않습니다.";
        return;
    }   

    // 프로그램 시작 시 일정이 있는 날짜까지만 할당하기 위해 변수값 변경 및 달력 할당 여부 판단하여 할당
    if(dm->max_year_index < year_e-dm->initial_year) {
        for(; dm->year_index < year_e-dm->initial_year;) {
            dm->AddYear();
        }
        // 프로그램 시작 시 일정이 있는 날짜까지만 할당하기 위해 변수값 변경
        dm->max_year_index = year_e-dm->initial_year;
    }

    // 일정 추가
    if(year_s == year_e) {
        if(month_s == month_e) {
            // 시작와 끝의 연도, 월이 같을 경우
            for(int d = day_s-1; d < day_e; d++) {
                date[year_s - dm->initial_year][month_s-1][d].AddSchedule(detail);
            }
        }
        else {
            // 시작과 끝의 연도가 같고 월이 다를 경우
            for(int d = day_s-1; d < dm->DAYS_LEAF_YEAR[month_s-1]; d++)                // 시작 달
                date[year_s - dm->initial_year][month_s-1][d].AddSchedule(detail);
            for(int m = month_s; m < month_e - 1; m++) {                                // 중간 달
                for(int d = 0;  d < dm->DAYS_LEAF_YEAR[m]; d++)
                    date[year_s - dm->initial_year][m][d].AddSchedule(detail);
            }
            for(int d = 0; d < day_e; d++)                                              // 끝 달
                date[year_s - dm->initial_year][month_e-1][d].AddSchedule(detail);
        }
    }
    else {
        // 시작과 끝의 연도가 다를 경우
        // 시작 연도
        for(int d = day_s-1; d < dm->DAYS_LEAF_YEAR[month_s-1]; d++)                    // 시작 연도의 시작 달
            date[year_s - dm->initial_year][month_s-1][d].AddSchedule(detail);
        for(int m = month_s; m < dm->MAX_MONTHS; m++) {                                 // 시작 연도의 나머지 달
            for(int d = 0;  d < dm->DAYS_LEAF_YEAR[m]; d++)
                date[year_s - dm->initial_year][m][d].AddSchedule(detail);
        }
        // 중간 연도
        for(int y = year_s - dm->initial_year + 1; y < year_e - dm->initial_year; y++){ // 중간 연도의 전체
            for(int m = 0; m < dm->MAX_MONTHS; m++) {
                for(int d = 0; d < dm->DAYS_LEAF_YEAR[m]; d++)
                    date[y][m][d].AddSchedule(detail);
            }
        }
        // 마지막 연도
            for(int m = 0; m < month_e - 1; m++) {                                      // 마지막 연도의 마지막 달 전까지의 달
                for(int d = 0;  d < dm->DAYS_LEAF_YEAR[m]; d++)
                    date[year_e - dm->initial_year][m][d].AddSchedule(detail);
            }
            for(int d = 0; d < day_e; d++)                                              // 마지막 연도의 끝 달
                date[year_e - dm->initial_year][month_e-1][d].AddSchedule(detail);
        }

    dm->SaveToFile();
    dm->SaveConfig();
}

// 캘린더 일정 삭제
void Calendar::DelSchedule(int year_s, int month_s, int day_s, int year_e, int month_e, int day_e, string detail) {
    // 날짜 유효성 체크(유효하지 않으면 함수 종료)
    if(!dm->CheckRange(year_s, month_s, day_s) || !dm->CheckRange(year_e, month_e, day_e)) {
        cout << "날짜가 유효하지 않습니다.";
        return;
    }
    // 시작 날짜 < 종료 날짜가 유효한지 체크
    if (year_s > year_e || 
    (year_s == year_e && month_s > month_e) || 
    (year_s == year_e && month_s == month_e && day_s > day_e)) {
        cout << "날짜가 유효하지 않습니다.";
        return;
    }

    // 끝 연도가 할당된 연도보다 범위가 클 경우 할당된 범위로 변경
    if(year_e - dm->initial_year > dm->max_year_index)
        year_e = dm->max_year_index + dm->initial_year;

    // 일정 삭제
    if(year_s == year_e) {
        if(month_s == month_e) {
            // 시작와 끝의 연도, 월이 같을 경우
            for(int d = day_s-1; d < day_e; d++) {
                date[year_s - dm->initial_year][month_s-1][d].DelSchedule(detail);
            }
        }
        else {
            // 시작과 끝의 연도가 같고 월이 다를 경우
            for(int d = day_s-1; d < dm->DAYS_LEAF_YEAR[month_s-1]; d++)                // 시작 달
                date[year_s - dm->initial_year][month_s-1][d].DelSchedule(detail);
            for(int m = month_s; m < month_e - 1; m++) {                                // 중간 달
                for(int d = 0;  d < dm->DAYS_LEAF_YEAR[m]; d++)
                    date[year_s - dm->initial_year][m][d].DelSchedule(detail);
            }
            for(int d = 0; d < day_e; d++)                                              // 끝 달
                date[year_s - dm->initial_year][month_e-1][d].DelSchedule(detail);
        }
    }
    else {
        // 시작과 끝의 연도가 다를 경우
        // 시작 연도
        for(int d = day_s-1; d < dm->DAYS_LEAF_YEAR[month_s-1]; d++)                    // 시작 연도의 시작 달
                date[year_s - dm->initial_year][month_s-1][d].DelSchedule(detail);
        for(int m = month_s; m < dm->MAX_MONTHS; m++) {                                 // 시작 연도의 나머지 달
            for(int d = 0;  d < dm->DAYS_LEAF_YEAR[m]; d++)
                date[year_s - dm->initial_year][m][d].DelSchedule(detail);
        }
        // 중간 연도
        for(int y = year_s - dm->initial_year + 1; y < year_e - dm->initial_year; y++){ // 중간 연도의 전체
            for(int m = 0; m < dm->MAX_MONTHS; m++) {
                for(int d = 0; d < dm->DAYS_LEAF_YEAR[m]; d++)
                    date[y][m][d].DelSchedule(detail);
            }
        }
        // 마지막 연도
            for(int m = 0; m < month_e - 1; m++) {                                      // 마지막 연도의 마지막 달 전까지의 달
                for(int d = 0;  d < dm->DAYS_LEAF_YEAR[m]; d++)
                    date[year_e - dm->initial_year][m][d].DelSchedule(detail);
            }
            for(int d = 0; d < day_e; d++)                                              // 마지막 연도의 끝 달
                date[year_e - dm->initial_year][month_e-1][d].DelSchedule(detail);
    }

    dm->SaveToFile();
    dm->SaveConfig();
}

// 캘린더 표시
void Calendar::PrintCalendar(int year, int month, int day) {
    system("cls");
    // 날짜 유효한지 체크
    if(!dm->CheckRange(year, month, day)) {
        cout << "날짜가 유효하지 않습니다.";
        return;
    }

    // 달력 할당 여부 판단하여 할당
    for(; dm->year_index < year - dm->initial_year;) {
        dm->AddYear();
    }
    
    // 윤년 판단
    const int* days_point;
    if(dm->CheckLeapYear(year))
        days_point = dm->DAYS_LEAF_YEAR;
    else   
        days_point = dm->DAYS;

    // 연도, 월 출력
    if(month < 10)
        cout << " ──────────────────────────────────────────────────────────────" 
        << year << ".0" << month 
        << "───────────────────────────────────────────────────────────────" << endl;
    else   
        cout << " ──────────────────────────────────────────────────────────────" 
        << year << "." << month 
        << "───────────────────────────────────────────────────────────────" << endl;
    
    // 요일 출력
    cout << "┌";                                        // 상단선 출력
    for(int i = 0; i < 6; i++)
        cout << "──────────────────┬";
    cout << "──────────────────┐" << endl;
    cout << "│      ";                                  // 요일 출력
    for(int i = 0; i < 7; i++) 
        cout << dm->WEEKDAYS[i] << "      │      ";
    cout << endl;
    PrintLine();                                        // 분리선 출력
    
    // 일 및 일정, 합계 출력
    // 첫 주
    int index = dm->GetDayOfWeek(year, month, day);
        for(; day+7 <= days_point[month-1]; ) {                 // 일 출력
            if(day == 1) {
                cout << "│";
                for(int i = 0; i < index; i++)
                    cout << "                  │";
                for(int i = index; i < 7; i++, day++) 
                    cout << "        " << day << "         │";
                cout << endl;
                PrintLine();                                    // 분리선 출력
                PrintSchedule(year, month, 1, 7-index, -1);     // 일정 출력
                PrintTotle(year, month, 1, 7-index, -1);        // 합계 출력
                PrintLine();                                    // 분리선 출력
            }
            cout << "│";
            // 중간 주
            for(int i = 0; i < 7; i++, day++){                  // 일 출력
                if(day < 10)
                    cout << "        " << day << "         │";
                else
                    cout << "        " << day << "        │";
            }
            cout << endl;
            PrintLine();                                        // 분리선 츌력
            PrintSchedule(year, month, day-7, day-1);           // 일정 출력
            PrintTotle(year, month, day-7, day-1);              // 합계 출력
            PrintLine();                                        // 분리선 츌력
        }
    // 마지막 주
    index = days_point[month-1] - day + 1;
    cout << "│";                                                // 일 출력
        for(int i = 0; i < index; i++, day++)
            cout << "        " << day << "        │";
        for(int i = index; i < 7; i++)
            cout << "                  │";
        cout << endl;
        PrintLine();                                            // 분리선 츌력
        PrintSchedule(year, month, days_point[month-1]-index+1, days_point[month-1],  1);   // 일정 출력
        PrintTotle(year, month, days_point[month-1]-index+1, days_point[month-1],  1);      // 합계 출력
        cout << "└";                                            // 상단선 출력
        for(int i = 0; i < 6; i++)
            cout << "──────────────────┴";
        cout << "──────────────────┘" << endl;
        
        
}

// 일정 출력
void Calendar::PrintSchedule(int year, int month, int day_s, int day_e, int space_position) {
    for(int i = 0; i < 7; i++) {
        cout << "│";
        if(space_position == -1) {
            for(int s = day_e-day_s+1; s < 7; s++)
                cout << "                  │";
        }
        for(int d = day_s-1; d < day_e; d++) {
            if(date[year - dm->initial_year][month-1][d].CountSchedule() > i) {
                cout << date[year - dm->initial_year][month-1][d].GetSchedule(i);
                for(int k = StringLength(date[year - dm->initial_year][month-1][d].GetSchedule(i)); k < 18; k++) 
                    cout << " ";
                cout << "│";
            }
            else
                cout << "                  │";
                
        }
        if(space_position == 1) {
            for(int s = day_e-day_s+1; s < 7; s++)
                cout << "                  │";
        }
        cout << endl;
    }
        
}   

// 가계부 일의 합계 출력
void Calendar::PrintTotle(int year, int month, int day_s, int day_e, int space_position) {
    cout << "│";
    if(space_position == -1) {
        for(int s = day_e-day_s+1; s < 7; s++)
            cout << "                  │";
    }
    for(int d = day_s-1; d < day_e; d++) {
        cout << "합계: " << account_book->GetTotal(year, month, d+1) << "원";
        for(int k = account_book->GetTotal(year, month, d+1).length(); k < 10; k++) 
            cout << " ";
        cout << "│";
    }     
    if(space_position == 1) {
       for(int s = day_e-day_s+1; s < 7; s++)
           cout << "                  │";
    }
    
        cout << endl;   
}   

// 분리선 출력
    void Calendar::PrintLine() {
        cout << "├";
        for(int i = 0; i < 6; i++)
            cout << "──────────────────┼";
        cout << "──────────────────┤" << endl;
    }

// 메뉴
void Calendar::Menu() {
    string input;
    PrintCalendar(current_year, current_month);

    while(true) {
        try {
            // 사용자 입력
            cout << "날짜 이동, 일정 추가, 일정 삭제, 플래너, 암호화 메모장, 가계부, 종료 중 하나를 선택해서 입력해주세요: ";
            getline(cin, input);
            input.erase(std::remove(input.begin(), input.end(), ' '), input.end());

            // 날짜 이동                
            if(input == "날짜이동") {
                // 사용자 입력
                int year, month;
                cout << "0000 00 형태로 연도와 월을 입력해주세요: ";
                cin >> year >> month;
                // 처리
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                if(cin.fail()){
                    cin.clear();
                    throw runtime_error("날짜를 잘못입력하였습니다.");
                }
                PrintCalendar(year, month);
            }
            else if(input == "일정추가") {
                // 사용자 입력
                int year_s, month_s, day_s;
                int year_e, month_e, day_e;
                string detail;
                cout << "시작 날짜를 0000 00 00 형태로 연도, 월, 일을 입력해주세요: ";
                cin >> year_s >> month_s >> day_s;
                // 처리
                if(cin.fail()){
                    cin.clear();
                    throw runtime_error("날짜를 잘못입력하였습니다.");
                }
                // 사용자 입력
                cout << "종료 날짜를 0000 00 00 형태로 연도, 월, 일을 입력해주세요: ";
                cin >> year_e >> month_e >> day_e;
                cout << "18글자 이내의 일정을 입력해주세요(한글: 2칸, 그 외: 1칸): ";
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                getline(cin, detail);
                // 처리
                if(cin.fail()){
                    cin.clear();
                    throw runtime_error("날짜를 잘못입력하였습니다.");
                }
                detail = Trim(detail);
                AddSchedule(year_s, month_s, day_s, year_e, month_e, day_e, detail);
                PrintCalendar(year_s, month_s);
            }
            else if(input == "일정삭제") {
                // 사용자 입력
                int year_s, month_s, day_s;
                int year_e, month_e, day_e;
                string detail;
                cout << "삭제할 일정의 시작 날짜를 0000 00 00 형태로 연도, 월, 일을 입력해주세요: ";
                cin >> year_s >> month_s >> day_s;
                // 처리
                if(cin.fail()){
                    cin.clear();
                    throw runtime_error("날짜를 잘못입력하였습니다.");
                }
                // 사용자 입력
                cout << "삭제할 일정의 종료 날짜를 0000 00 00 형태로 연도, 월, 일을 입력해주세요: ";
                cin >> year_e >> month_e >> day_e;
                cout << "18글자 이내의 일정을 입력해주세요(한글: 2칸, 그 외: 1칸): ";
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                getline(cin, detail);
                // 처리
                if(cin.fail()){
                    cin.clear();
                    throw runtime_error("날짜를 잘못입력하였습니다.");
                }
                detail = Trim(detail);
                DelSchedule(year_s, month_s, day_s, year_e, month_e, day_e, detail);
                PrintCalendar(year_s, month_s);
            }
            else if(input == "플래너") {
                plan->Menu();
                PrintCalendar(current_year, current_month);
            }
            else if(input == "암호화메모장") {

            }   
            else if(input == "가계부") {
                account_book->Menu();
                PrintCalendar(current_year, current_month);
            }
            else if(input == "종료") {
                cout << "종료합니다.";
                break;
            }
            else {
                cout << "잘못된 입력입니다." << endl;
            }
        }
        catch(runtime_error e) {
            cout << e.what() << endl;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }   
    }
}

