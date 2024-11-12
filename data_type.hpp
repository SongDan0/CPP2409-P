    #include<string>
    #include<vector>
    #include <algorithm>

    using namespace std;

    class Schedule {
    private:    
        //일정 벡터
        vector<string> schedule;  
    public: 
        //일정 추가
        void addSchedule(string detail);
        //일정 삭제
        void delSchedule(string detail);
        //일정vector 반환
        vector<string> getSchedule_v();
        //index번째 일정 반환
        string getSchedule_s(int index);
        //일정 개수 반환
        int countSchedule();
        //일정이 비어있는지 확인
        bool emptySchedule();
    };
