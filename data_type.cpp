#include "data_type.hpp"

//일정 추가
void Schedule::addSchedule(string detail) {
    schedule.push_back(detail);
}

//일정 삭제
void Schedule::delSchedule(string detail) {
    schedule.erase(remove(schedule.begin(), schedule.end(), detail), schedule.end());
}

//index번째 일정 반환
string Schedule::getSchedule_s(int index) {
    return schedule[index];
}

//일정vector 반환
vector<string> Schedule::getSchedule_v() {
    return schedule;
}

//일정 개수 반환
int Schedule::countSchedule() {
    return schedule.size();
}

//일정이 비어있는지 확인
bool Schedule::emptySchedule() {
    return schedule.empty();
}