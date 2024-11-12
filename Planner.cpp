#include "Planner.hpp"

//생성자(달력 해당 연도만 할당)
Planner::Planner() {
    //시작 연도 현재 연도로 설정
    auto now = chrono::system_clock::now();                             
    time_t now_c = chrono::system_clock::to_time_t(now);
    tm* localTime = localtime(&now_c);
    initial_year = 1900 + localTime->tm_year;

    loadConfig();
    
    //초기 연도 할당
    int index = maxYearIndex;
    maxYearIndex = -1;
    planner = new Schedule**[MAX_YEARS];
    do{
        addYear();
    }while(maxYearIndex < index);
    loadFromFile("planer");
}

//달력 범위 1년 증가(다음 연도 할당)
void Planner::addYear() {
    planner[++maxYearIndex] = new Schedule*[MAX_MONTHS];               // 다음 연도에 대한 월 배열 생성
    for (int i = 0; i < MAX_MONTHS; i++)
        planner[maxYearIndex][i] = new Schedule[MAX_DAYS];             // 다음연도의 월에 대한 일 배열 생성
}

bool Planner::checkLeapYear(int year) {
    if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)
        return true;
    return false;
}

//날짜 유효한지 체크(true: 날짜 유효/ flase: 날짜 유효x)
bool Planner::check_range(int year, int month, int day) {
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
void Planner::addSchedule(string detail){
    planner[currentYear-initial_year][currentMonth][currentDay].addSchedule(detail);
    saveToFile("planer");
}

//일정 삭제
void Planner::delSchedule(string detail) {
    planner[currentYear-initial_year][currentMonth][currentDay].delSchedule(detail);
    saveToFile("planer");
}

//해당 날짜의 요일 반환(Zeller의 공식)(0=일요일, 1=월요일, ..., 6=토요일)
int Planner::getDayOfWeek(int year, int month, int day) {
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

//플래너 표시
void Planner::printPlanner() {
    //날짜 유효한지 체크
    if(!check_range(currentYear, currentMonth, currentDay)) {
        cout << "날짜가 유효하지 않습니다.";
        return;
    }

    if(maxYearIndex < currentYear - initial_year) {
        for(; maxYearIndex < currentYear - initial_year;) {
            addYear();
        }
    }

    //연도, 월
    if(currentMonth < 10) {
        if(currentDay < 10)
            cout << "──────────────────" << currentYear << ".0" << currentMonth << ".0" << currentDay << "(" << weekdays[getDayOfWeek(currentYear, currentMonth, currentDay)] << ")──────────────────" << endl;
        else
            cout << "──────────────────" << currentYear << ".0" << currentMonth << "." << currentDay << "(" << weekdays[getDayOfWeek(currentYear, currentMonth, currentDay)] << ")──────────────────" << endl;
    }
    else {
        if(currentDay < 10)
            cout << "──────────────────" << currentYear << "." << currentMonth << ".0" << currentDay << "(" << weekdays[getDayOfWeek(currentYear, currentMonth, currentDay)] << ")──────────────────" << endl;
        else
            cout << "──────────────────" << currentYear << "." << currentMonth << "." << currentDay << "(" << weekdays[getDayOfWeek(currentYear, currentMonth, currentDay)] << ")──────────────────" << endl;
    }
    printSchedule();
}

    //일정 출력
void Planner::printSchedule() {
    cout << "┌────────────────────────────────────────┐" << endl;
    for(int i = 0; i < planner[currentYear-initial_year][currentMonth][currentDay].countSchedule(); i++) {
        cout << "│" << planner[currentYear-initial_year][currentMonth][currentDay].getSchedule_s(i);
        for(int j = planner[currentYear-initial_year][currentMonth][currentDay].getSchedule_s(i).length()*2/3; j < 40; j++)
            cout << " ";
        cout << "│" << endl;
    }
    for(int i = planner[currentYear-initial_year][currentMonth][currentDay].countSchedule(); i < 24; i++)
        cout  << "│                                        │" <<endl;
    cout << "└────────────────────────────────────────┘" << endl;
}   

void Planner::saveToFile(const string& filename) {
ofstream outFile(filename);

    if (outFile.is_open()) {
        saveConfig();  // 설정을 파일에 저장
        // Iterate over all years, months, and days
        for (int y = 0; y <= maxYearIndex; ++y) {
            for (int m = 0; m < MAX_MONTHS; ++m) {
                for (int d = 0; d < Days_m[m]; ++d) {
                    // Only save days with schedules
                    if (!planner[y][m][d].emptySchedule()) {
                        for (const string& schedule : planner[y][m][d].getSchedule_v()) {
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
void Planner::loadFromFile(const string& filename) {
    ifstream inFile(filename);

    if (inFile.is_open()) {
        int year, month, day;
        string detail;

        // Read each line in the file
        while (inFile >> year >> month >> day) {
            // Clear any remaining whitespace before reading detail
            inFile.ignore();
            getline(inFile, detail);

            // Adjust planner range if needed
            int yearIndex = year - initial_year;
            if (yearIndex > maxYearIndex) {
                while (maxYearIndex < yearIndex) {
                    addYear();
                }
            }

            // Add the schedule to the appropriate day
            planner[yearIndex][month - 1][day - 1].addSchedule(detail);
        }

        inFile.close();
        cout << "일정이 파일에서 성공적으로 불러와졌습니다." << endl;
    } else {
        cout << "파일을 열 수 없습니다." << endl;
    }
}
void Planner::saveConfig() {
    ofstream configFile("config1.txt");

    if (configFile.is_open()) {
        configFile << initial_year << " " << maxYearIndex;
        configFile.close();
        cout << "설정 저장 완료: initial_year = " << initial_year 
             << ", maxYearIndex = " << maxYearIndex << endl;
    } else {
        cout << "설정 파일을 저장할 수 없습니다." << endl;
    }
}

void Planner::loadConfig() {
    ifstream configFile("config1.txt");

    if (configFile.is_open()) {
        configFile >> initial_year >> maxYearIndex;
        configFile.close();
        cout << "설정 불러오기: initial_year = " << initial_year 
             << ", maxYearIndex = " << maxYearIndex << endl;
    } else {
        cout << "설정 파일이 없으므로 기본값을 사용합니다." << endl;
    }
}
void Planner::menu() {
    string input;
    cout << "0000 00 00형태로 연도, 월, 일을 입력해주세요: ";
            cin >> currentYear >> currentMonth >> currentDay;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            system("cls");
            printPlanner();
    while(true) {
        cout << "플래너, 일정 추가, 일정 삭제, 뒤로 가기 중 하나를 선택해서 입력해주세요: ";
        getline(cin, input);
        if(input == "플래너") {
            cout << "0000 00 00형태로 연도, 월, 일을 입력해주세요: ";
            cin >> currentYear >> currentMonth >> currentDay;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            system("cls");
            printPlanner();
        }
        else if(input == "일정 추가") {
            string detail;
            cout << "9글자 이내의 한글로 일정을 입력해주세요(띄어쓰기 사용금지): ";
            getline(cin, detail);
            addSchedule(detail);
            system("cls");
            printPlanner();
        }
        else if(input == "일정 삭제") {
            string detail;
            cout << "9글자 이내의 한글로 일정을 입력해주세요(띄어쓰기 사용금지): ";
            getline(cin, detail);
            delSchedule(detail);
            system("cls");
            printPlanner();
        }
        else if(input == "뒤로 가기") {
            break;
        }
        else {
            cout << "잘못된 입력입니다.";
        }
    }
}

