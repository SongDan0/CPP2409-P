#include "Calendar.hpp"
using namespace std;

//생성자(달력 해당 연도만 할당)
Calendar::Calendar() {
    //시작 연도 현재 연도로 설정
    auto now = chrono::system_clock::now();                             
    time_t now_c = chrono::system_clock::to_time_t(now);
    tm* localTime = localtime(&now_c);
    initial_year = 1900 + localTime->tm_year;

    loadConfig();
    cout << maxYearIndex;
    //초기 연도 할당
    int index = maxYearIndex;
    maxYearIndex = -1;
    calendar = new Schedule**[MAX_YEARS];
    do{
        addYear();
    }while(maxYearIndex < index);
    loadFromFile("schedule");
    plan = new Planner{};
}

//달력 범위 1년 증가(다음 연도 할당)
void Calendar::addYear() {
    calendar[++maxYearIndex] = new Schedule*[MAX_MONTHS];               // 다음 연도에 대한 월 배열 생성
    for (int i = 0; i < MAX_MONTHS; i++)
        calendar[maxYearIndex][i] = new Schedule[MAX_DAYS];             // 다음연도의 월에 대한 일 배열 생성
}

bool Calendar::checkLeapYear(int year) {
    if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)
        return true;
    return false;
}

//날짜 유효한지 체크(true: 날짜 유효/ flase: 날짜 유효x)
bool Calendar::check_range(int year, int month, int day) {
    if(initial_year <= year && year <= initial_year + 99) {
        if(1 <= month  && month <= MAX_MONTHS) {
            if (checkLeapYear(year)){ // 윤년 판단
                if(1 <= day && day <= Days_M[month-1])
                    return true;
            }
            else {
                if(1 <= day && day <= Days_m[month-1])
                    return true;
            }
        }
    }
    return false;
}

//캘린더 일정 추가
void Calendar::addSchedule(int year_s, int month_s, int day_s, int year_e, int month_e, int day_e, string detail){
    //날짜 유효성 체크(유효하지 않으면 함수 종료)
    if(!check_range(year_s, month_s, day_s) || !check_range(year_e, month_e, day_e)) {
        cout << "날짜가 유효하지 않습니다.";
        return;
    }

    //달력 할당 여부 판단하여 할당
    if(maxYearIndex < year_s - initial_year || maxYearIndex < year_e - initial_year) {
        for(; maxYearIndex < year_e - initial_year;) {
            addYear();
        }
    }

    //일정 추가
    if(year_s == year_e) {
        if(month_s == month_e) {
            //시작와 끝의 연도, 월이 같을 경우
            for(int d = day_s-1; d < day_e; d++) {
                calendar[year_s - initial_year][month_s-1][d].addSchedule(detail);
            }
        }
        else {
            //시작과 끝의 연도가 같고 월이 다를 경우
            for(int d = day_s-1; d < Days_M[month_s-1]; d++)                            //시작 달
                calendar[year_s - initial_year][month_s-1][d].addSchedule(detail);
            for(int m = month_s; m < month_e - 1; m++) {                                //중간 달
                for(int d = 0;  d < Days_M[m]; d++)
                    calendar[year_s - initial_year][m][d].addSchedule(detail);
            }
            for(int d = 0; d < day_e; d++)                                  //끝 달
                calendar[year_s - initial_year][month_e-1][d].addSchedule(detail);
        }
    }
    else {
        //시작과 끝의 연도가 다를 경우
        //시작 연도
        for(int d = day_s-1; d < Days_M[month_s-1]; d++)                                //시작 연도의 시작 달
            calendar[year_s - initial_year][month_s-1][d].addSchedule(detail);
        for(int m = month_s; m < MAX_MONTHS; m++) {                                     //시작 연도의 나머지 달
            for(int d = 0;  d < Days_M[m]; d++)
                calendar[year_s - initial_year][m][d].addSchedule(detail);
            }
        //중간 연도
        for(int y = year_s - initial_year + 1; y < year_e - initial_year; y++){     //중간 연도의 전체
            for(int m = 0; m < MAX_MONTHS; m++) {
                for(int d = 0; d < Days_M[m]; d++)
                    calendar[y][m][d].addSchedule(detail);
            }
        }
        //마지막 연도
            for(int m = 0; m < month_e - 1; m++) {                                      //마지막 연도의 마지막 달 전까지의 달
                for(int d = 0;  d < Days_M[m]; d++)
                    calendar[year_e - initial_year][m][d].addSchedule(detail);
            }
            for(int d = 0; d < day_e; d++)                                  //마지막 연도의 끝 달
                calendar[year_e - initial_year][month_e-1][d].addSchedule(detail);
        }
    saveToFile("schedule");
}

    //일정 삭제
void Calendar::delSchedule(int year_s, int month_s, int day_s, int year_e, int month_e, int day_e, string detail) {
    //날짜 유효성 체크(유효하지 않으면 함수 종료)
    if(!check_range(year_s, month_s, day_s) || !check_range(year_e, month_e, day_e)) {
        cout << "날짜가 유효하지 않습니다.";
        return;
    }

    //끝 연도가 할당된 연도보다 범위가 클 경우 할당된 범위롤 변경
    if(year_e - initial_year > maxYearIndex)
        year_e = maxYearIndex + initial_year;

        //일정 삭제
    if(year_s == year_e) {
        if(month_s == month_e) {
            //시작와 끝의 연도, 월이 같을 경우
            for(int d = day_s-1; d < day_e; d++) {
                calendar[year_s - initial_year][month_s-1][d].delSchedule(detail);
            }
        }
        else {
            //시작과 끝의 연도가 같고 월이 다를 경우
            for(int d = day_s-1; d < Days_M[month_s-1]; d++)                            //시작 달
                calendar[year_s - initial_year][month_s-1][d].delSchedule(detail);
            for(int m = month_s; m < month_e - 1; m++) {                                //중간 달
                for(int d = 0;  d < Days_M[m]; d++)
                    calendar[year_s - initial_year][m][d].delSchedule(detail);
            }
            for(int d = 0; d < day_e; d++)                                  //끝 달
                calendar[year_s - initial_year][month_e-1][d].delSchedule(detail);
        }
    }
    else {
        //시작과 끝의 연도가 다를 경우
        //시작 연도
        for(int d = day_s-1; d < Days_M[month_s-1]; d++)                                //시작 연도의 시작 달
                calendar[year_s - initial_year][month_s-1][d].delSchedule(detail);
        for(int m = month_s; m < MAX_MONTHS; m++) {                                     //시작 연도의 나머지 달
            for(int d = 0;  d < Days_M[m]; d++)
                calendar[year_s - initial_year][m][d].delSchedule(detail);
        }
        //중간 연도
        for(int y = year_s - initial_year + 1; y < year_e - initial_year; y++){     //중간 연도의 전체
            for(int m = 0; m < MAX_MONTHS; m++) {
                for(int d = 0; d < Days_M[m]; d++)
                    calendar[y][m][d].delSchedule(detail);
            }
        }
        //마지막 연도
            for(int m = 0; m < month_e - 1; m++) {                                      //마지막 연도의 마지막 달 전까지의 달
                for(int d = 0;  d < Days_M[m]; d++)
                    calendar[year_e - initial_year][m][d].delSchedule(detail);
            }
            for(int d = 0; d < day_e; d++)                                  //마지막 연도의 끝 달
                calendar[year_e - initial_year][month_e-1][d].delSchedule(detail);
    }
    saveToFile("schedule");
}

    //해당 날짜의 요일 반환(Zeller의 공식)(0=일요일, 1=월요일, ..., 6=토요일)
int Calendar::getDayOfWeek(int year, int month, int day) {
    // 1월과 2월을 13월, 14월로 처리
    if (month < 3) {
        month += 12;
        year--;
    }
    int K = year % 100; // 연도의 마지막 두 자리
    int J = year / 100; // 연도의 첫 두 자리
    // Zeller의 공식
    int h = (day + (13 * (month + 1)) / 5 + K + K / 4 + J / 4 + 5 * J) % 7;
    h = (h + 6) % 7;  // h가 음수일 경우 양수로 변환

    return h;
}

    //캘린더 표시
void Calendar::printCalendar(int year, int month, int day) {
    //날짜 유효한지 체크
    if(!check_range(year, month, day)) {
        cout << "날짜가 유효하지 않습니다.";
        return;
    }
     //달력 할당 여부 판단하여 할당
    if(maxYearIndex < year - initial_year) {
        for(; maxYearIndex < year - initial_year;) {
            addYear();
        }
    }

    //윤년 판단
    const int* Days;
    if(checkLeapYear(year))
        Days = Days_M;
    else   
        Days = Days_m;

    //연도, 월
    if(month < 10)
        cout << "──────────────────" << year << ".0" << month << "──────────────────" << endl;
    else   
        cout << "──────────────────" << year << "." << month << "──────────────────" << endl;
    //요일
    cout << "┌──────────────────┬──────────────────┬──────────────────┬──────────────────┬──────────────────┬──────────────────┬──────────────────┐" << endl;
    cout << "│      ";
    for(int i = 0; i < 7; i++) 
        cout << weekdays[i] << "      │      ";
    cout << endl;
    cout << "├──────────────────┼──────────────────┼──────────────────┼──────────────────┼──────────────────┼──────────────────┼──────────────────┤" << endl;
    //일
    int index = getDayOfWeek(year, month, day);
        for(; day+7 <= Days[month-1]; ) {
            if(day == 1) {
                cout << "│";
                for(int i = 0; i < index; i++)
                    cout << "                  │";
                for(int i = index; i < 7; i++, day++) 
                    cout << "        " << day << "         │";
                cout << endl;
                cout << "├──────────────────┼──────────────────┼──────────────────┼──────────────────┼──────────────────┼──────────────────┼──────────────────┤" << endl;
                printSchedule(year, month, 1, 7-index, Days, -1);
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
            printSchedule(year, month, day-7, day-1, Days);
            cout << "├──────────────────┼──────────────────┼──────────────────┼──────────────────┼──────────────────┼──────────────────┼──────────────────┤" << endl;
        }
    index = Days[month-1] - day + 1;
    cout << "│";
        for(int i = 0; i < index; i++, day++)
            cout << "        " << day << "        │";
        for(int i = index; i < 7; i++)
            cout << "                  │";
        cout << endl;
        cout << "├──────────────────┼──────────────────┼──────────────────┼──────────────────┼──────────────────┼──────────────────┼──────────────────┤" << endl;
        printSchedule(year, month, Days[month-1]-index+1, Days[month-1], Days,  1);
    cout << "└──────────────────┴──────────────────┴──────────────────┴──────────────────┴──────────────────┴──────────────────┴──────────────────┘" << endl;
        
}

    //일정 출력
void Calendar::printSchedule(int year, int month, int day_s, int day_e, const int* Days, int space_position) {
    for(int i = 0; i < 7; i++) {
        cout << "│";
        if(space_position == -1) {
            for(int s = day_e-day_s+1; s < 7; s++)
                cout << "                  │";
        }
        for(int d = day_s-1; d < day_e; d++) {
            if(calendar[year - initial_year][month-1][d].countSchedule() > i) {
                cout << calendar[year - initial_year][month-1][d].getSchedule_s(i);
                for(int k = calendar[year - initial_year][month-1][d].getSchedule_s(i).length()*2/3; k < 18; k++) 
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

void Calendar::saveToFile(const string& filename) {
ofstream outFile(filename);

    if (outFile.is_open()) {
        saveConfig();  // 설정을 파일에 저장
        // Iterate over all years, months, and days
        for (int y = 0; y <= maxYearIndex; ++y) {
            for (int m = 0; m < MAX_MONTHS; ++m) {
                for (int d = 0; d < Days_m[m]; ++d) {
                    // Only save days with schedules
                    if (!calendar[y][m][d].emptySchedule()) {
                        for (const string& schedule : calendar[y][m][d].getSchedule_v()) {
                            outFile << (initial_year + y) << " "  // Year
                                    << (m + 1) << " "  // Month
                                    << (d + 1) << " "  // Day
                                    << schedule << "\n";  // Schedule details
                        }
                    }
                }
            }
        }
        outFile.close();
        cout << "일정이 파일에 저장되었습니다." << endl;
    } else {
        cout << "파일을 열 수 없습니다." << endl;
    }
}

// 파일에서 일정 데이터를 불러오는 함수
void Calendar::loadFromFile(const string& filename) {
    ifstream inFile(filename);

    if (inFile.is_open()) {
        int year, month, day;
        string detail;

        // Read each line in the file
        while (inFile >> year >> month >> day) {
            // Clear any remaining whitespace before reading detail
            inFile.ignore();
            getline(inFile, detail);

            // Adjust calendar range if needed
            int yearIndex = year - initial_year;
            if (yearIndex > maxYearIndex) {
                while (maxYearIndex < yearIndex) {
                    addYear();
                }
            }

            // Add the schedule to the appropriate day
            calendar[yearIndex][month - 1][day - 1].addSchedule(detail);
        }

        inFile.close();
        cout << "일정이 파일에서 성공적으로 불러와졌습니다." << endl;
    } else {
        cout << "파일을 열 수 없습니다." << endl;
    }
}
void Calendar::saveConfig() {
    ofstream configFile("config.txt");

    if (configFile.is_open()) {
        configFile << initial_year << " " << maxYearIndex;
        configFile.close();
        cout << "설정 저장 완료: initial_year = " << initial_year 
             << ", maxYearIndex = " << maxYearIndex << endl;
    } else {
        cout << "설정 파일을 저장할 수 없습니다." << endl;
    }
}

void Calendar::loadConfig() {
    ifstream configFile("config.txt");

    if (configFile.is_open()) {
        configFile >> initial_year >> maxYearIndex;
        configFile.close();
        cout << "설정 불러오기: initial_year = " << initial_year 
             << ", maxYearIndex = " << maxYearIndex << endl;
    } else {
        cout << "설정 파일이 없으므로 기본값을 사용합니다." << endl;
    }
}
void Calendar::menu() {
    string input;
    while(true) {
        cout << "캘린더, 일정 추가, 일정 삭제, 플래너, 암호화 메모장, 가계부, 종료 중 하나를 선택해서 입력해주세요: ";
        getline(cin, input);
        if(input == "캘린더") {
            int year, month;
            cout << "0000 00 형태로 연도와 월을 입력해주세요: ";
            cin >> year >> month;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            system("cls");
            printCalendar(year, month);
        }
        else if(input == "일정 추가") {
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
            addSchedule(year_s, month_s, day_s, year_e, month_e, day_e, detail);
            system("cls");
            printCalendar(year_s, month_s);
        }
        else if(input == "일정 삭제") {
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
            delSchedule(year_s, month_s, day_s, year_e, month_e, day_e, detail);
            system("cls");
            printCalendar(year_s, month_s);
        }
        else if(input == "플래너") {
            plan->menu();
        }
        else if(input == "암호화 메모장") {

        }
        else if(input == "가계부") {

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

