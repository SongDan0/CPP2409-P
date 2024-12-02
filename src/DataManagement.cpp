#include "DataManagement.hpp"

// 생성자(현재 시간 저장, 연도 단위 날짜 할달, 환경 설정 파일 가져오기)
DataManagement::DataManagement(): Date() {
    // 환경 설정 파일 가져오기
    initial_year = current_year;
    LoadConfig();            
    
    // 초기 연도 할당  
    if(current_year - initial_year > max_year_index)
        max_year_index = current_year - initial_year;  

    calendar = new Schedule**[MAX_YEARS];
    planner = new Schedule**[MAX_YEARS];
    account_book = new Transaction**[MAX_YEARS];
    while(year_index < max_year_index){
        AddYear();
    }  

    // 일정 불러오기
    LoadFromFile();
}

// 싱글턴 객체 반환
 DataManagement& DataManagement::GetInstance() {
    static DataManagement instance;     // 메서드 내부에서 static 인스턴스 생성
    return instance;
}

// calendar배열 반환
Schedule*** DataManagement::GetArrayCalendar() {
    return calendar;
}

// planner배열 반환
Schedule*** DataManagement::GetArrayPlanner() {
    return planner;
}
    
// account_book배열 반환
Transaction*** DataManagement::GetArrayAccountBook() {
    return account_book;
}

// 달력 범위 1년 증가(다음 연도 할당)
void DataManagement::AddYear() {
    // 캘린더
    calendar[++year_index] = new Schedule*[MAX_MONTHS];         // 다음 연도에 대한 월 배열 생성
    for (int i = 0; i < MAX_MONTHS; i++)                        // 다음 연도의 월에 대한 일 배열 생성
        calendar[year_index][i] = new Schedule[MAX_DAYS];  
    
    // 플래너
    planner[year_index] = new Schedule*[MAX_MONTHS];            // 다음 연도에 대한 월 배열 생성
    for (int i = 0; i < MAX_MONTHS; i++)                        // 다음 연도의 월에 대한 일 배열 생성
        planner[year_index][i] = new Schedule[MAX_DAYS];  

    // 가계부
    account_book[year_index] = new Transaction*[MAX_MONTHS];     // 다음 연도에 대한 월 배열 생성
    for (int i = 0; i < MAX_MONTHS; i++)                         // 다음 연도의 월에 대한 일 배열 생성
        account_book[year_index][i] = new Transaction[MAX_DAYS];  
}

//환경 설정 파일 저장
void DataManagement::SaveConfig() {
    ofstream configFile("config");                              //config파일을 쓰기 모드로 열기                            

    if (configFile.is_open()) {                                 //파일이 정상적으로 열렸을 경우
        configFile << initial_year << " " << max_year_index;    //intitial_year maxYearIndex 저장
        configFile.close();                                     //파일 닫기
    } else {
        cerr << "설정 파일을 저장할 수 없습니다." << endl;
    }
}

//환경 설정 파일 불러오기
void DataManagement::LoadConfig() {
    ifstream configFile("config");                              //config파일을 읽기 모드로 열기

    if (configFile.is_open()) {                                 //파일이 정상적으로 열렸을 경우
        configFile >> initial_year >> max_year_index;           //intitial_year, maxYearIndex 읽어오기
        configFile.close();                                     //파일 닫기
    } else {
        cout << "설정 파일이 없으므로 기본값을 사용합니다." << endl;
    }
}

// 데이터 파일 저장
void DataManagement::SaveToFile() {
    string fileName[3] = {"../storage/CalendarData",
        "../storage/PlannerData", 
        "../storage/AccountBookData"};
    // CalendarData, PlannerData 저장
    for(int i = 0; i < 2; i++) {
        ofstream outFile(fileName[i]);
        Schedule*** data;
        if(i == 0) 
            data = calendar;
        else
            data = planner;
        //파일이 정상적으로 열렸을 경우
        if (outFile.is_open()) {
            for (int y = 0; y <= max_year_index; ++y) {                                               
                for (int m = 0; m < MAX_MONTHS; ++m) {
                    for (int d = 0; d < MAX_DAYS; ++d) {
                        // 해당 날짜의 일정이 비어있지 않다면 저장
                        if (!data[y][m][d].IsEmpty()) {
                            // year month day schedule 형태로 저장
                            for (const string& schedule : data[y][m][d].GetSchedule()) {          
                                outFile << (initial_year + y) << " "    // Year
                                        << (m + 1) << " "               // Month
                                        << (d + 1) << " "               // Day
                                        << schedule << "\n";            // Schedule details
                            }
                        }
                    }
                }
            }
            outFile.close();   
        }
        //파일이 정상적으로 열리지 않았을 경우
        else {                                                    
            cerr << "파일을 열 수 없습니다." << endl;
        }
    }
    // AccountBookData 저장
    ofstream outFile(fileName[2]);    
    if (outFile.is_open()) {
        for (int y = 0; y <= max_year_index; ++y) {                                               
            for (int m = 0; m < MAX_MONTHS; ++m) {
                for (int d = 0; d < MAX_DAYS; ++d) {
                    // 해당 날짜의 일정이 비어있지 않다면 저장
                    if (!account_book[y][m][d].IsEmpty()) {
                        // year month day schedule 형태로 저장
                        for (int i = 0; i < account_book[y][m][d].CountTransaction(); i++) {          
                            outFile << (initial_year + y) << " "    // Year
                                    << (m + 1) << " "               // Month
                                    << (d + 1) << " "               // Day
                                    << account_book[y][m][d].GetPrice(i) << " "      // Transaction Price
                                    << account_book[y][m][d].GetDetail(i) << "\n";   // Transaction Detail
                        }
                    }
                }
            }
        }
        outFile.close();                                     
    }
    //파일이 정상적으로 열리지 않았을 경우
    else {                                
        cerr << "파일을 열 수 없습니다." << endl;
    }
}

// 파일에서 데이터를 불러오기
void DataManagement::LoadFromFile() {
    string fileName[3] = {"../storage/CalendarData",
        "../storage/PlannerData",
        "../storage/AccountBookData"};
    // CalendarData, PlannerData 불러오기
    for(int i = 0; i < 2; i++) {
        ifstream inFile(fileName[i]);                                         
        Schedule*** data;
        if(i == 0) 
            data = calendar;
        else
            data = planner;
        //파일이 정상적으로 열렸을 경우
        if (inFile.is_open()) {
            int year, month, day;                                           // 날짜 정보를 저장할 변수              
            string detail;                                                  // 일정을 저장할 변수

            // 파일에서 한 줄씩 읽으면서 연도, 월, 일, 세부 내용을 처리
            while (inFile >> year >> month >> day) {                        // 연도, 월, 일 읽어오기 
                inFile.ignore();                                            // 한 칸 건너띄어 공백 무기
                getline(inFile, detail);                                    // 일정 읽어오기
                data[year-initial_year][month - 1][day - 1].AddSchedule(detail); // 읽어온 일정 추가
            }

            inFile.close();                                               
        }
        //파일이 정상적으로 열리지 않았을 경우 
        else {
            cout << "읽어올 데이터 파일이 없습니다." << endl;
        }
    }

    // AccountBookData 불러오기
    ifstream inFile(fileName[2]);                                      
    //파일이 정상적으로 열렸을 경우
    if (inFile.is_open()) {
        int year, month, day, price;                                    // 날짜 정보를 저장할 변수              
        string detail;                                                  // 일정을 저장할 변수

        // 파일에서 한 줄씩 읽으면서 연도, 월, 일, 세부 내용을 처리
        while (inFile >> year >> month >> day >> price) {               // 연도, 월, 일 읽어오기 
            inFile.ignore();                                            // 한 칸 건너띄어 공백 무기
            getline(inFile, detail);                                    // 일정 읽어오기
            account_book[year-initial_year][month - 1][day - 1].AddTransaction(detail, price); // 읽어온 일정 추가
        }

        inFile.close();                                              
    }
    //파일이 정상적으로 열리지 않았을 경우 
    else {
        cout << "읽어올 데이터 파일이 없습니다." << endl;
    }

}

// 시작과 끝의 공백을 제거하는 함수
string Trim(const string& str) {
    // 정규 표현식을 사용하여 시작과 끝의 공백 제거
    regex ws_re("^\\s+|\\s+$");
    return regex_replace(str, ws_re, "");
}

// 문자열의 출력 길이 반환하는 함수
int StringLength(const string& str) {
    int count_ascii = 0;
    int count_korean = 0;
    for(char c: str) {
        if(0 <= c && c <= 127)
            count_ascii++;
        else
            count_korean++;
    }
    return count_ascii + count_korean*2/3;
}
