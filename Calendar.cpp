#include "Calendar.hpp"

// 생성자(달력 해당 연도만 할당)
Calendar::Calendar() {
    bd = &DataManagement::GetInstance();
    date = bd->GetArrayCalendar();
    current_year = bd->current_year;
    current_month = bd->current_month;
    current_day = bd->current_day;
    plan = new Planner{};
    accountBook = new AccountBook{};
}

// 캘린더 일정 추가
void Calendar::AddSchedule(int year_s, int month_s, int day_s, int year_e, int month_e, int day_e, string detail){
    // 날짜 유효성 체크(유효하지 않으면 함수 종료)
    if(!bd->CheckRange(year_s, month_s, day_s) || !bd->CheckRange(year_e, month_e, day_e)) {
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
    if(bd->max_year_index < year_e - bd->initial_year) {
        for(; bd->year_index < year_e - bd->initial_year; bd->max_year_index++) {
            bd->AddYear();
        }
        // 프로그램 시작 시 일정이 있는 날짜까지만 할당하기 위해 변수값 변경
        bd->max_year_index = year_e - bd->initial_year;
    }

    //일정 추가
    if(year_s == year_e) {
        if(month_s == month_e) {
            //시작와 끝의 연도, 월이 같을 경우
            for(int d = day_s-1; d < day_e; d++) {
                date[year_s - bd->initial_year][month_s-1][d].AddSchedule(detail);
            }
        }
        else {
            //시작과 끝의 연도가 같고 월이 다를 경우
            for(int d = day_s-1; d < bd->kDaysLeafYear[month_s-1]; d++)                     //시작 달
                date[year_s - bd->initial_year][month_s-1][d].AddSchedule(detail);
            for(int m = month_s; m < month_e - 1; m++) {                                //중간 달
                for(int d = 0;  d < bd->kDaysLeafYear[m]; d++)
                    date[year_s - bd->initial_year][m][d].AddSchedule(detail);
            }
            for(int d = 0; d < day_e; d++)                                              //끝 달
                date[year_s - bd->initial_year][month_e-1][d].AddSchedule(detail);
        }
    }
    else {
        //시작과 끝의 연도가 다를 경우
        //시작 연도
        for(int d = day_s-1; d < bd->kDaysLeafYear[month_s-1]; d++)                         //시작 연도의 시작 달
            date[year_s - bd->initial_year][month_s-1][d].AddSchedule(detail);
        for(int m = month_s; m < bd->MAX_MONTHS; m++) {                                     //시작 연도의 나머지 달
            for(int d = 0;  d < bd->kDaysLeafYear[m]; d++)
                date[year_s - bd->initial_year][m][d].AddSchedule(detail);
        }
        //중간 연도
        for(int y = year_s - bd->initial_year + 1; y < year_e - bd->initial_year; y++){         //중간 연도의 전체
            for(int m = 0; m < bd->MAX_MONTHS; m++) {
                for(int d = 0; d < bd->kDaysLeafYear[m]; d++)
                    date[y][m][d].AddSchedule(detail);
            }
        }
        //마지막 연도
            for(int m = 0; m < month_e - 1; m++) {                                      //마지막 연도의 마지막 달 전까지의 달
                for(int d = 0;  d < bd->kDaysLeafYear[m]; d++)
                    date[year_e - bd->initial_year][m][d].AddSchedule(detail);
            }
            for(int d = 0; d < day_e; d++)                                              //마지막 연도의 끝 달
                date[year_e - bd->initial_year][month_e-1][d].AddSchedule(detail);
        }
    bd->SaveToFile();
    bd->SaveConfig();
}

// 캘린더 일정 삭제
void Calendar::DelSchedule(int year_s, int month_s, int day_s, int year_e, int month_e, int day_e, string detail) {
    // 날짜 유효성 체크(유효하지 않으면 함수 종료)
    if(!bd->CheckRange(year_s, month_s, day_s) || !bd->CheckRange(year_e, month_e, day_e)) {
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

    //끝 연도가 할당된 연도보다 범위가 클 경우 할당된 범위로 변경
    if(year_e - bd->initial_year > bd->max_year_index)
        year_e = bd->max_year_index + bd->initial_year;

    //일정 삭제
    if(year_s == year_e) {
        if(month_s == month_e) {
            //시작와 끝의 연도, 월이 같을 경우
            for(int d = day_s-1; d < day_e; d++) {
                date[year_s - bd->initial_year][month_s-1][d].DelSchedule(detail);
            }
        }
        else {
            //시작과 끝의 연도가 같고 월이 다를 경우
            for(int d = day_s-1; d < bd->kDaysLeafYear[month_s-1]; d++)                     //시작 달
                date[year_s - bd->initial_year][month_s-1][d].DelSchedule(detail);
            for(int m = month_s; m < month_e - 1; m++) {                                //중간 달
                for(int d = 0;  d < bd->kDaysLeafYear[m]; d++)
                    date[year_s - bd->initial_year][m][d].DelSchedule(detail);
            }
            for(int d = 0; d < day_e; d++)                                              //끝 달
                date[year_s - bd->initial_year][month_e-1][d].DelSchedule(detail);
        }
    }
    else {
        //시작과 끝의 연도가 다를 경우
        //시작 연도
        for(int d = day_s-1; d < bd->kDaysLeafYear[month_s-1]; d++)                         //시작 연도의 시작 달
                date[year_s - bd->initial_year][month_s-1][d].DelSchedule(detail);
        for(int m = month_s; m < bd->MAX_MONTHS; m++) {                                     //시작 연도의 나머지 달
            for(int d = 0;  d < bd->kDaysLeafYear[m]; d++)
                date[year_s - bd->initial_year][m][d].DelSchedule(detail);
        }
        //중간 연도
        for(int y = year_s - bd->initial_year + 1; y < year_e - bd->initial_year; y++){         //중간 연도의 전체
            for(int m = 0; m < bd->MAX_MONTHS; m++) {
                for(int d = 0; d < bd->kDaysLeafYear[m]; d++)
                    date[y][m][d].DelSchedule(detail);
            }
        }
        //마지막 연도
            for(int m = 0; m < month_e - 1; m++) {                                      //마지막 연도의 마지막 달 전까지의 달
                for(int d = 0;  d < bd->kDaysLeafYear[m]; d++)
                    date[year_e - bd->initial_year][m][d].DelSchedule(detail);
            }
            for(int d = 0; d < day_e; d++)                                              //마지막 연도의 끝 달
                date[year_e - bd->initial_year][month_e-1][d].DelSchedule(detail);
    }
    bd->SaveToFile();
    bd->SaveConfig();
}

// 캘린더 표시
void Calendar::PrintCalendar(int year, int month, int day) {
    system("cls");
    //날짜 유효한지 체크
    if(!bd->CheckRange(year, month, day)) {
        cout << "날짜가 유효하지 않습니다.";
        return;
    }

    //달력 할당 여부 판단하여 할당
    for(; bd->year_index < year - bd->initial_year;) {
        bd->AddYear();
    }
    
    //윤년 판단
    const int* kDaysPoint;
    if(bd->CheckLeapYear(year))
        kDaysPoint = bd->kDaysLeafYear;
    else   
        kDaysPoint = bd->kDays;

    //연도, 월 출력
    if(month < 10)
        cout << " ──────────────────────────────────────────────────────────────" << year << ".0" << month << "───────────────────────────────────────────────────────────────" << endl;
    else   
        cout << " ──────────────────────────────────────────────────────────────" << year << "." << month << "───────────────────────────────────────────────────────────────" << endl;
    
    //요일 출력
    cout << "┌──────────────────┬──────────────────┬──────────────────┬──────────────────┬──────────────────┬──────────────────┬──────────────────┐" << endl;
    cout << "│      ";
    for(int i = 0; i < 7; i++) 
        cout << bd->kWeekdays[i] << "      │      ";
    cout << endl;
    cout << "├──────────────────┼──────────────────┼──────────────────┼──────────────────┼──────────────────┼──────────────────┼──────────────────┤" << endl;
    
    //일 및 일정 출력
    int index = bd->GetDayOfWeek(year, month, day);
        for(; day+7 <= kDaysPoint[month-1]; ) {
            if(day == 1) {
                cout << "│";
                for(int i = 0; i < index; i++)
                    cout << "                  │";
                for(int i = index; i < 7; i++, day++) 
                    cout << "        " << day << "         │";
                cout << endl;
                cout << "├──────────────────┼──────────────────┼──────────────────┼──────────────────┼──────────────────┼──────────────────┼──────────────────┤" << endl;
                PrintSchedule(year, month, 1, 7-index, -1);
                PrintTotle(year, month, 1, 7-index, -1);
                cout << "├──────────────────┼──────────────────┼──────────────────┼──────────────────┼──────────────────┼──────────────────┼──────────────────┤" << endl;
            }
            cout << "│";
            for(int i = 0; i < 7; i++, day++){
                if(day < 10)
                    cout << "        " << day << "         │";
                else
                    cout << "        " << day << "        │";
            }
            cout << endl;
            cout << "├──────────────────┼──────────────────┼──────────────────┼──────────────────┼──────────────────┼──────────────────┼──────────────────┤" << endl;
            PrintSchedule(year, month, day-7, day-1);
            PrintTotle(year, month, day-7, day-1);
            cout << "├──────────────────┼──────────────────┼──────────────────┼──────────────────┼──────────────────┼──────────────────┼──────────────────┤" << endl;
        }
    index = kDaysPoint[month-1] - day + 1;
    cout << "│";
        for(int i = 0; i < index; i++, day++)
            cout << "        " << day << "        │";
        for(int i = index; i < 7; i++)
            cout << "                  │";
        cout << endl;
        cout << "├──────────────────┼──────────────────┼──────────────────┼──────────────────┼──────────────────┼──────────────────┼──────────────────┤" << endl;
        PrintSchedule(year, month, kDaysPoint[month-1]-index+1, kDaysPoint[month-1],  1);
        PrintTotle(year, month, kDaysPoint[month-1]-index+1, kDaysPoint[month-1],  1);
    cout << "└──────────────────┴──────────────────┴──────────────────┴──────────────────┴──────────────────┴──────────────────┴──────────────────┘" << endl;
        
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
            if(date[year - bd->initial_year][month-1][d].CountSchedule() > i) {
                cout << date[year - bd->initial_year][month-1][d].GetScheduleString(i);
                for(int k = date[year - bd->initial_year][month-1][d].GetScheduleString(i).length()*2/3; k < 18; k++) 
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
        cout << "합계: " << accountBook->GetTotal(year, month, d+1) << "원";
        for(int k = accountBook->GetTotal(year, month, d+1).length(); k < 10; k++) 
            cout << " ";
        cout << "│";
    }     
    if(space_position == 1) {
       for(int s = day_e-day_s+1; s < 7; s++)
           cout << "                  │";
    }
    
        cout << endl;   
}   

// 메뉴
void Calendar::Menu() {
    string input;
    system("cls");
    PrintCalendar(current_year, current_month);
    while(true) {
        cout << "날짜 이동, 일정 추가, 일정 삭제, 플래너, 암호화 메모장, 가계부, 종료 중 하나를 선택해서 입력해주세요: ";
        getline(cin, input);
        input.erase(std::remove(input.begin(), input.end(), ' '), input.end());                                   
        if(input == "날짜이동") {
            int year, month;
            cout << "0000 00 형태로 연도와 월을 입력해주세요: ";
            cin >> year >> month;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            PrintCalendar(year, month);
        }
        else if(input == "일정추가") {
            int year_s, month_s, day_s;
            int year_e, month_e, day_e;
            string detail;
            cout << "시작 날짜를 0000 00 00 형태로 연도, 월, 일을 입력해주세요: ";
            cin >> year_s >> month_s >> day_s;
            cout << "종료 날짜를 0000 00 00 형태로 연도, 월, 일을 입력해주세요: ";
            cin >> year_e >> month_e >> day_e;
            cout << "9글자 이내의 한글로 일정을 입력해주세요(띄어쓰기 사용금지): ";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            getline(cin, detail);
            AddSchedule(year_s, month_s, day_s, year_e, month_e, day_e, detail);
            PrintCalendar(year_s, month_s);
        }
        else if(input == "일정삭제") {
            int year_s, month_s, day_s;
            int year_e, month_e, day_e;
            string detail;
            cout << "삭제할 일정의 시작 날짜를 0000 00 00 형태로 연도, 월, 일을 입력해주세요: ";
            cin >> year_s >> month_s >> day_s;
            cout << "삭제할 일정의 종료 날짜를 0000 00 00 형태로 연도, 월, 일을 입력해주세요: ";
            cin >> year_e >> month_e >> day_e;
            cout << "삭제할 일정을 적어주세요: ";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            getline(cin, detail);
            DelSchedule(year_s, month_s, day_s, year_e, month_e, day_e, detail);
            PrintCalendar(year_s, month_s);
        }
        else if(input == "플래너") {
            plan->Menu();
            system("cls");
            PrintCalendar(current_year, current_month);
        }
        else if(input == "암호화메모장") {

        }
        else if(input == "가계부") {
            accountBook->Menu();
            system("cls");
            PrintCalendar(current_year, current_month);
        }
        else if(input == "종료") {
            cout << "종료합니다.";
            break;
        }
        else {
            cout << "잘못된 입력입니다.";
        }
    }
}

