#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>

using namespace std;

class Schedule {
private:    
    // 일정 벡터
    vector<string> schedule;  
};


class DataManagement{
private:
    Schedule*** calendar = nullptr;           // 날짜별 Schedule 객체를 담을 3차원 배열의 포인터
    Schedule*** planner = nullptr;            // 날짜별 Schedule 객체를 담을 3차원 배열의 포인터 
    int initial_year = 0;           // 0번 인덱스의 연도(시작 연도)
    int max_year_index = -1;        // 프로그램 시작 시 할당할 연도의 인덱스의 최댓값을 저장하는 변수  
    static const int MAX_YEARS;                     // 최대 연도수
    static const int MAX_MONTHS;                    // 최대 개월수
    static const int MAX_DAYS;                      // 최대 일수
    static const int kDays[12];                     // 각 월의 최대 일수
    static const int kDaysLeafYear[12];             // 각 월의 최대 일수
    static const string kWeekdays[7];               // 요일 배열
        int current_year;                        // 현재 연도 변수
    int current_month;                       // 현재 월 변수
    int current_day;                         // 현재 일 변수
    //Transaction*** accountBook;     // 날짜별 Schedule 객체를 담을 3차원 배열의 포인터   

    // 생성자(Date 생성자 호출, 연도 단위 날짜 할당, 환경 설정 파일 가져오기)(Singleton)
    DataManagement();
public:
    int year_index = -1;            // 현재 할당된 연도의 인덱스의 최댓값을 저장할 변수

    // 싱글턴 객체 반환
    static DataManagement& GetInstance();

    //calendar배열 반환
    Schedule*** GetArrayCalendar();

    //planner배열 반환
    Schedule*** GetArrayPlanner();
    
    //accountBook배열 반환
    //Transaction*** GetArrayAccountBook();
    
    // 달력 범위 1년 증가(다음 연도 할당)
    void AddYear();
};

// 생성자(현재 시간 저장, 연도 단위 날짜 할달, 환경 설정 파일 가져오기)
DataManagement::DataManagement(){
    // 환경 설정 파일 가져오기
                      
    
    // 초기 연도 할당  
        // 현재 시간 가져오기
    time_t t = time(nullptr);            // 현재 시간을 time_t 타입으로 얻기
    tm* now = localtime(&t);             // 현재 시간을 tm 구조체 포인터로 변환

    if(current_year - initial_year > max_year_index)
        max_year_index = current_year - initial_year;

    // 현재 년, 월, 일을 변수에 저장
    current_year = now->tm_year + 1900;  // tm_year는 1900년부터 시작하므로 1900을 더해 실제 연도 계산
    current_month = now->tm_mon + 1;     // tm_mon은 0부터 시작하므로 1을 더해 실제 월 계산
    current_day = now->tm_mday;          // 오늘의 일(day)
    if(current_year - initial_year > max_year_index)
        initial_year = current_year;
    
    calendar = new Schedule**[MAX_YEARS];
    planner = new Schedule**[MAX_YEARS];
    //accountBook = new Transaction**[MAX_YEARS];
    do {
        AddYear();
        cout << year_index << " " << max_year_index << endl;
    }while(year_index < max_year_index);


}

// 싱글턴 객체 반환
 DataManagement& DataManagement::GetInstance() {
    static DataManagement instance;  // 메서드 내부에서 static 인스턴스 생성
    return instance;
}

//calendar배열 반환
Schedule*** DataManagement::GetArrayCalendar() {
    return calendar;
}

//planner배열 반환
Schedule*** DataManagement::GetArrayPlanner() {
    return planner;
}
    
//accountBook배열 반환
//Transaction*** GetArrayAccountBook() {
//    return accountBook;
//}

// 달력 범위 1년 증가(다음 연도 할당)
// 달력 범위 1년 증가(다음 연도 할당)
void DataManagement::AddYear() {
    // 캘린더
    calendar[++year_index] = new Schedule*[MAX_MONTHS];         // 다음 연도에 대한 월 배열 생성
    for (int i = 0; i < MAX_MONTHS; i++)                        // 다음 연도의 월에 대한 일 배열 생성
        calendar[year_index][i] = new Schedule[MAX_DAYS];  
    
    // 플래너
    planner[year_index] = new Schedule*[MAX_MONTHS];            // 다음 연도에 대한 월 배열 생성
    //for (int i = 0; i < MAX_MONTHS; i++)                        // 다음 연도의 월에 대한 일 배열 생성
        planner[year_index][0] = new Schedule[MAX_DAYS];  

    // 가계부
    //accountBook[year_index] = new Transaction*[MAX_MONTHS];      // 다음 연도에 대한 월 배열 생성
    //for (int i = 0; i < MAX_MONTHS; i++)                        // 다음 연도의 월에 대한 일 배열 생성
    //    accountBook[year_index][i] = new Transaction[MAX_DAYS];  
}

const int DataManagement::MAX_YEARS = 100;                                                 
const int DataManagement::MAX_MONTHS = 12;                                                           
const int DataManagement::MAX_DAYS = 31;                                            
const int DataManagement::kDays[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}; 
const int DataManagement::kDaysLeafYear[12] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
const string DataManagement::kWeekdays[7] = {"일요일", "월요일", "화요일", "수요일", "목요일", "금요일", "토요일"};

int main() {
    DataManagement::GetInstance();
}
