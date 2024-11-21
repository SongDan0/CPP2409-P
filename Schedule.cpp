#include "Schedule.hpp"

//일정 추가
void Schedule::AddSchedule(string detail) {
    schedule.push_back(detail);
}

//일정 삭제
void Schedule::DelSchedule(string detail) {
    schedule.erase(remove(schedule.begin(), schedule.end(), detail), schedule.end());
}

//index번째 일정 반환
string Schedule::GetScheduleString(int index) {
    return schedule[index];
}

//일정vector 반환
vector<string> Schedule::GetScheduleVector() {
    return schedule;
}

//일정 개수 반환
int Schedule::CountSchedule() {
    return schedule.size();
}

//일정이 비어있는지 확인
bool Schedule::EmptySchedule() {
    return schedule.empty();
}