    #include <string>
    #include <vector>
    #include <algorithm>

    using namespace std;

    class Schedule {
    private:    
        //일정 벡터
        vector<string> schedule;  
    public: 
        //일정 추가
        void AddSchedule(string detail);
        //일정 삭제
        void DelSchedule(string detail);
        //일정vector 반환
        vector<string> GetScheduleVector();
        //index번째 일정 반환
        string GetScheduleString(int index);
        //일정 개수 반환
        int CountSchedule();
        //일정이 비어있는지 확인
        bool EmptySchedule();
    };