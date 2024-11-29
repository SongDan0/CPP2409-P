#include "AccountBook.hpp"

// 생성자(Date 생성자 호출)
AccountBook::AccountBook() {
    bd = &DataManagement::GetInstance();
    date = bd->GetArrayAccountBook();
    current_year = bd->current_year;
    current_month = bd->current_month;
    current_day = bd->current_day;
}

// 가계부 내역 추가
void AccountBook::AddTransaction(string detail, int price) {
    // 프로그램 시작 시 일정이 있는 날짜까지만 할당하기 위해 변수값 변경
    if(bd->max_year_index < current_year - bd->initial_year)
        bd->max_year_index = current_year-bd->initial_year;
    
    // 일정 추가
    date[current_year-bd->initial_year][current_month-1][current_day-1].AddTransaction(detail, price);
    
    // 변경 사항 저장
    bd->SaveToFile();
    bd->SaveConfig();
}

// 가계부 내역 삭제
void AccountBook::DelTransaction(string detail, int price) {
    // 일정 삭제
    date[current_year-bd->initial_year][current_month-1][current_day-1].DelTransaction(detail, to_string(price));
    
    // 변경 사항 저장
    bd->SaveToFile();
    bd->SaveConfig();
};

// 합계 반환
    string AccountBook::GetTotal(int year, int month, int day) {
        int sum = 0;
        for(string str: date[year-bd->initial_year][month-1][day-1].GetPriceVector())
            sum += stoi(str);
        return to_string(sum);
    }

// 가계부 표시
void AccountBook::PrintAccountBook() {
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
            cout << " ──────────" << current_year 
            << ".0" << current_month 
            << ".0" << current_day 
            << "(" << Date::kWeekdays[bd->GetDayOfWeek(current_year, current_month, current_day)] 
            << ")────────────" << endl;
        else
            cout << " ──────────" << current_year 
            << ".0" << current_month 
            << "." << current_day 
            << "(" << Date::kWeekdays[bd->GetDayOfWeek(current_year, current_month, current_day)] 
            << ")────────────" << endl;
    }
    else {
        if(current_day < 10)
            cout << " ──────────" << current_year 
            << "." << current_month 
            << ".0" << current_day 
            << "(" << Date::kWeekdays[bd->GetDayOfWeek(current_year, current_month, current_day)] 
            << ")────────────" << endl;
        else
            cout << " ──────────" << current_year 
            << "." << current_month 
            << "." << current_day 
            << "(" << Date::kWeekdays[bd->GetDayOfWeek(current_year, current_month, current_day)] 
            << ")────────────" << endl;
    }

    // 내역 출력
    PrintTransaction();
};

// 내역 출력
void AccountBook::PrintTransaction() {
     // 위 경계선 출력
    cout << "┌────────────────────────────────────────┐" << endl;
    
    // 일정 출력 및 오른쪽 공백 출력
    for(int i = 0; i < date[current_year-bd->initial_year][current_month-1][current_day-1].CountTransaction(); i++) {
        cout << "│" << date[current_year-bd->initial_year][current_month-1][current_day-1].GetDetailString(i) << " " 
        << date[current_year-bd->initial_year][current_month-1][current_day-1].GetPriceString(i) << "원";
        int j = StringLength(date[current_year-bd->initial_year][current_month-1][current_day-1].GetDetailString(i));
        j += date[current_year-bd->initial_year][current_month-1][current_day-1].GetPriceString(i).length();
        for(; j < 37; j++)
            cout << " ";
        cout << "│" << endl;
    }
    
    // 아래쪽 공백 출력
    for(int i = date[current_year-bd->initial_year][current_month-1][current_day-1].CountTransaction(); i < 24; i++)
        cout  << "│                                        │" <<endl;
    
    // 합계 출력
    cout << "│합계: " << GetTotal(current_year, current_month, current_day) << "원";
    for(int i = GetTotal(current_year, current_month, current_day).length(); i < 32; i++)
        cout << " ";
    cout << "│" << endl;

    // 아래 경계선 출력
    cout << "└────────────────────────────────────────┘" << endl;
};

// 메뉴
void AccountBook::Menu() {
     string input;
            system("cls");
            PrintAccountBook();
    while(true) {
        // 날짜 이동, 일정 추가, 일정 삭제, 캘린더 중 하나 사용자 입력 받기
        cout << "날짜 이동, 거래 내역 추가, 거래 내역 삭제, 뒤로가기 중 하나를 선택해서 입력해주세요(뒤로 가기는 캘린더로 이동): ";
        getline(cin, input);
        input.erase(std::remove(input.begin(), input.end(), ' '), input.end());            
        // 날짜 이동
        if(input == "날짜이동") {
            cout << "0000 00 00형태로 연도, 월, 일을 입력해주세요: ";
            cin >> current_year >> current_month >> current_day;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            system("cls");
            PrintAccountBook();
        }
        // 일정 추가
        else if(input == "거래내역추가") {
            string detail;
            int price;
            cout << "30글자 이내로 거래 내역을 입력해주세요(한글: 2칸, 그 외: 1칸): ";
            getline(cin, detail);
            cout << "9글자 이내의 거래 가격을 입력해주세요: ";
            cin >> price;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            detail = Trim(detail);
            AddTransaction(detail, price);
            system("cls");
            PrintAccountBook();
        }
        // 일정 삭제
        else if(input == "거래내역삭제") {
            string detail;
            int price;
            cout << "30글자 이내로 거래 내역을 입력해주세요(한글: 2칸, 그 외: 1칸): ";
            getline(cin, detail);
            cout << "9글자 이내의 거래 가격을 입력해주세요: ";
            cin >> price;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            detail = Trim(detail);
            DelTransaction(detail,price);
            system("cls");
            PrintAccountBook();
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
};