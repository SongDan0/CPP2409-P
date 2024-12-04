#include "Note.hpp"

//생성자
Note::Note() {
    LoadMemo();
    AllocationMemo();
}

// 메모장 할당
void Note::AllocationMemo() {
    note.push_back(vector<string>());
}

// 저장
void Note::SaveMemo() {
    ofstream outFile("../storage/NoteData");    
    if (outFile.is_open()) {
        outFile << EncryptCaesar(password) << endl;
        for (int page = 0; page < note.size(); ++page) {            
            if(!note.at(page).empty()) {
                 for (string str: note.at(page)) {
                    outFile << page << " "
                            << EncryptCaesar(str) << endl;
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

// 불러오기
void Note::LoadMemo() {
    ifstream inFile("../storage/NoteData");                                      
    //파일이 정상적으로 열렸을 경우
    if (inFile.is_open()) {
        int page;                                             
        string str;                        

        inFile >> password;
        password = DecryptCaesar(password);
        int max_page = -1;
        while (inFile >> page >> str) {              
            if(max_page < page) {
                AllocationMemo();
                max_page++;
            }
            inFile.ignore();                                           
            note.at(page).push_back(DecryptCaesar(str));
        }

        inFile.close();                                              
    }
    //파일이 정상적으로 열리지 않았을 경우 
    else {
        cout << "읽어올 데이터 파일이 없습니다." << endl;
    }
}

// 비밀번호 설정
void Note::SetPassword() {
    while(true) {
        string password;
        string check_password;
        
        // 비밀번호 1차 입력
        cout << "비밀번호를 입력해주세요: ";
        getline(cin, password);
        password.erase(remove(password.begin(), password.end(), ' '),
            password.end());
        
        // 비밀번호 2차 입력
        cout << "비밀번호를 다시 입력해주세요: ";
        getline(cin, check_password);
        check_password.erase(remove(check_password.begin(), check_password.end(), ' '),
            check_password.end());

        // 비밀번호 1차, 2차 같을 경우 비밀번호 설정    
        if(password == check_password) {
            this->password = password;
            break;
        }
        // 비밀번호 1차, 2차 다를 경우 비밀번호 재입력    
        else {
            continue;
        }
    }

    SaveMemo();
}

// 비밀번호 입력 받기 및 해제
bool Note::Unlock() {
    while(true) {
        string input;
        string password;
        // 비밀번호가 없을 경우 잠금 해제
        if(this->password == "") {
            return true;
        }
        // 사용자 입력
        cout << "패스워드, 뒤로가기 중 하나를 입력해주세요: ";
        getline(cin, input);
        input.erase(remove(input.begin(), input.end(), ' '), input.end());
        // 패스워드가 맞을 경우 잠금해제
        if(input == "패스워드") {
            cout << "패스워드: ";
            getline(cin, password);
            input.erase(std::remove(input.begin(), input.end(), ' '), input.end());
            if(this->password == password) {
                return true;
            }
            // 패스워드가 틀릴 경우 문구 출력
            else {
                cout << "패스워드가 틀렸습니다." << endl;
                continue;
            }
        }
        else if(input == "뒤로가기") {
            return false;
        }
        else {
            cout << "잘못 입력하였습니다." << endl;
            continue;
        }
    }
}

// 메모장 표시
void Note::PrintNote() {
    system("cls");
    // 페이지 출력
    cout << "page: " << current_page << endl; 
    
    // 메모 출력
    for(string str: note.at(current_page)) {
        cout << str << endl;
    }

    // 아래 공백 출력
    for(int i = note.at(current_page).size(); i < 30; i++ )
        cout << endl;
}

// 메모 추가
void Note::AddMemo(string detail) {
    note[current_page].push_back(detail);
    SaveMemo();
}

// 메모 삭제
void Note::DelMemo(string detail) {
    note[current_page].erase(
        remove(note[current_page].begin(), note[current_page].end(), detail), 
        note[current_page].end());
    SaveMemo();
}

// 다음 메모
void Note::NextMemo() {
    current_page++;
    try {
        PrintNote();
    }
    catch(out_of_range e) {
        AllocationMemo();
    }
    PrintNote();
}

// 이전 메모
void Note::PriviousMemo() {
    if(current_page != 0) {
        current_page--;
        PrintNote();
    }
    else {
        cout << "이전 페이지가 없습니다." << endl;
    }
}

// page번째 메모로 이동
void Note::PageMemo(int page) {
    if(0 <= page && page < note.size()) {
        current_page = page;
        PrintNote();
    }
    else {
        cout << "page가 존재하지 않습니다." << endl;
    }
}

// 메뉴
void Note::Menu() {
    string input;
    bool is_user = Unlock();

    // 초기 메모장 출력
    if(is_user) {
        PrintNote();
    }

    // 메뉴
    while(is_user) {
        // 사용자 입력
        cout << "추가, 삭제, 다음, 이전, 페이지, 비밀번호 설정, 뒤로가기 중 하나를 선택해서 입력해주세요: ";
        getline(cin, input);
        input.erase(remove(input.begin(), input.end(), ' '), input.end());

        // 추가
        if(input == "추가") {
            // 사용자 입력
            string detail;
            cout << "입력해주세요: ";
           getline(cin, detail);
            // 처리
            AddMemo(detail);
            PrintNote();
        }
        // 삭제
        else if(input == "삭제") {
            // 사용자 입력
            string detail;
            cout << "입력해주세요: ";
            getline(cin, detail);
            // 처리
            DelMemo(detail);
            PrintNote();
        }
        // 다음
        else if(input == "다음") {
            NextMemo();
        }
        // 이전
        else if(input == "이전") {
            PriviousMemo();
        }
        // 페이지 지정 이동
        else if(input == "페이지") {
            try {
                int page;
                cout << "페이지를 입력해주세요: ";
                cin >> page;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                if(cin.fail()){
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    throw runtime_error("page를 정수로 입력해주세요");
                }
                PageMemo(page);
            }catch(runtime_error e) {
                cout << e.what() << endl;
            }
        }
        // 패스워드 설정
        else if(input == "비밀번호설정") {
            SetPassword();
            PrintNote();
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

// Caesar Cipher 암호화 (모든 문자 포함)
string EncryptCaesar(string text, int shift) {
    string result = "";

    // 각 문자에 대해 shift를 적용 (알파벳에만 제한되지 않음)
    for (char c : text) {
        result += c + shift;  // 모든 문자를 shift만큼 이동
    }
    return result;
}

// Caesar Cipher 복호화
string DecryptCaesar(string text) {
    return EncryptCaesar(text, -10);  // 복호화는 음수 방향으로 shift
}

