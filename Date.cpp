#include "Date.hpp"

//static const 변수 초기화
const int Date::MAX_YEARS = 100;                                                 
const int Date::MAX_MONTHS = 12;                                                           
const int Date::MAX_DAYS = 31;                                            
const int Date::Days_m[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}; 
const int Date::Days_M[12] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
const string Date::weekdays[7] = {"일요일", "월요일", "화요일", "수요일", "목요일", "금요일", "토요일"};

// 생성자(현재 시간 저장, 연도 단위 날짜 할달, 환경 설정 파일 가져오기)
Date::Date(string config, string filename) {
    // 현재 시간 가져오기
    time_t t = time(nullptr);           // 현재 시간을 time_t 타입으로 얻기
    tm* now = localtime(&t);            // 현재 시간을 tm 구조체 포인터로 변환

    // 현재 년, 월, 일을 변수에 저장
    currentYear = now->tm_year + 1900;  // tm_year는 1900년부터 시작하므로 1900을 더해 실제 연도 계산
    initial_year = currentYear;         //
    currentMonth = now->tm_mon + 1;     // tm_mon은 0부터 시작하므로 1을 더해 실제 월 계산
    currentDay = now->tm_mday;          // 오늘의 일(day)

    
    // 환경 설정 파일 가져오기
    loadConfig(config);                       

    
    // 초기 연도 할당
    if(currentYear - initial_year > maxYearIndex)
        maxYearIndex = currentYear;
    
    date = new Schedule**[MAX_YEARS];
    do {
        addYear();
    }while(yearIndex < maxYearIndex);

    
    
    //일정 불러오기
    loadFromFile(filename);
}

// 달력 범위 1년 증가(다음 연도 할당)
void Date::addYear() {
    date[++yearIndex] = new Schedule*[MAX_MONTHS];          // 다음 연도에 대한 월 배열 생성
    for (int i = 0; i < MAX_MONTHS; i++)                    // 다음 연도의 월에 대한 일 배열 생성
        date[yearIndex][i] = new Schedule[MAX_DAYS];  
}

// 윤년 체크(true: 윤년/ flase: 윤년x)
bool Date::checkLeapYear(int year) {
    if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)
        return true;
    return false;
}

// 날짜 유효한지 체크(true: 날짜 유효/ flase: 날짜 유효x)
bool Date::check_range(int year, int month, int day) {
    if(initial_year <= year && year <= initial_year + 99) { // 연도 체크
        if(1 <= month  && month <= MAX_MONTHS) {            // 월 체크
            if (checkLeapYear(year)){                       // 윤년 판단(일 체크)
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

// 해당 날짜의 요일 반환(Zeller의 공식)(0=일요일, 1=월요일, ..., 6=토요일)
int Date::getDayOfWeek(int year, int month, int day) {
    // 1월과 2월을 13월, 14월로 처리
    if (month < 3) {
        month += 12;
        year--;
    }
    int K = year % 100;                                                         // 연도의 마지막 두 자리
    int J = year / 100;                                                         // 연도의 첫 두 자리
    
    // Zeller의 공식
    int h = (day + (13 * (month + 1)) / 5 + K + K / 4 + J / 4 + 5 * J) % 7;
    h = (h + 6) % 7;                                                            // h가 음수일 경우 양수로 변환

    return h;                                                                   // 요일 인덱스값 반환
}

//환경 설정 파일 저장
void Date::saveConfig(string config) {
    ofstream configFile(config);                                    //config에 저장된 문자열을 이름으로 가진 파일을 쓰기 모드로 열기                            

    if (configFile.is_open()) {                                     //파일이 정상적으로 열렸을 경우
        configFile << initial_year << " " << maxYearIndex;          //intitial_year maxYearIndex 형식으로 저장
        configFile.close();                                         //파일 닫기
    } else {
        cout << "설정 파일을 저장할 수 없습니다." << endl;
    }
}

//환경 설정 파일 불러오기
void Date::loadConfig(string config) {
    ifstream configFile(config);                                    //config에 저장된 문자열을 이름으로 가진 파일을 읽기 모드로 열기

    if (configFile.is_open()) {                                     //파일이 정상적으로 열렸을 경우
        configFile >> initial_year >> maxYearIndex;                 //intitial_year, maxYearIndex 읽어오기
        configFile.close();                                         //파일 닫기
    } else {
        cout << "설정 파일이 없으므로 기본값을 사용합니다." << endl;
    }
}

// 데이터 파일 저장
void Date::saveToFile(const string& filename) {
    ofstream outFile(filename);                                     // filename에 저장된 문자열을 이름으로 가진 파일을 파일 쓰기 모드로 열기
    //파일이 정상적으로 열렸을 경우
    if (outFile.is_open()) {                                                                    
        // 각 날짜 순회
        for (int y = 0; y <= maxYearIndex; ++y) {                                               
            for (int m = 0; m < MAX_MONTHS; ++m) {
                for (int d = 0; d < MAX_DAYS; ++d) {
                    // 해당 날짜의 일정이 비어있지 않다면 저장
                    if (!date[y][m][d].emptySchedule()) {
                        // year month day schedule 형태로 저장
                        for (const string& schedule : date[y][m][d].getSchedule_v()) {          
                            outFile << (initial_year + y) << " "    // Year
                                    << (m + 1) << " "               // Month
                                    << (d + 1) << " "               // Day
                                    << schedule << "\n";            // Schedule details
                        }
                    }
                }
            }
        }
        outFile.close();                                            // 파일 닫기
    } 
    //파일이 정상적으로 열리지 않았을 경우
    else {                                                    
        cout << "파일을 열 수 없습니다." << endl;
    }
}

// 파일에서 데이터를 불러오기
void Date::loadFromFile(const string& filename) {
    ifstream inFile(filename);                                          // filename에 저장된 문자열을 이름으로 가진 파일을 파일 읽기 모드로 열기

    //파일이 정상적으로 열렸을 경우
    if (inFile.is_open()) {
        int year, month, day;                                           // 날짜 정보를 저장할 변수              
        string detail;                                                  // 일정을 저장할 변수

        // 파일에서 한 줄씩 읽으면서 연도, 월, 일, 세부 내용을 처리
        while (inFile >> year >> month >> day) {                        // 연도, 월, 일 읽어오기 
            inFile.ignore();                                            // 한 칸 건너띄어 공백 무기
            getline(inFile, detail);                                    // 일정 읽어오기
            date[year-initial_year][month - 1][day - 1].addSchedule(detail); // 읽어온 일정 추가
        }

        inFile.close();                                                 //파일 닫기
    }
    //파일이 정상적으로 열리지 않았을 경우 
    else {
        cout << "파일을 열 수 없습니다." << endl;
    }
}