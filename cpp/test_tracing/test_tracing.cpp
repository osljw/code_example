#include <iostream>
#include <fstream>
//// Procedure: dump
//inline void ExecutorObserver::dump(std::ostream& os) const {
//
//  os << '[';
//
//  for(size_t w=0; w<_timeline.executions.size(); w++) {
//
//    if(w != 0 && _timeline.executions[w].size() > 0 && 
//                 _timeline.executions[w-1].size() > 0) {
//      os << ',';
//    }
//
//    for(size_t i=0; i<_timeline.executions[w].size(); i++) {
//
//      os << '{'
//         << "\"cat\":\"ExecutorObserver\","
//         << "\"name\":\"" << _timeline.executions[w][i].task_view.name() << "\","
//         << "\"ph\":\"X\","
//         << "\"pid\":1,"
//         << "\"tid\":" << w << ','
//         << "\"ts\":" << std::chrono::duration_cast<std::chrono::microseconds>(
//                           _timeline.executions[w][i].beg - _timeline.origin
//                         ).count() << ','
//         << "\"dur\":" << std::chrono::duration_cast<std::chrono::microseconds>(
//                           _timeline.executions[w][i].end - _timeline.executions[w][i].beg
//                         ).count();
//
//      if(i != _timeline.executions[w].size() - 1) {
//        os << "},";
//      }
//      else {
//        os << '}';
//      }
//    }
//  }
//  os << "]\n";
//}

int main() {

  std::ofstream os("timestamps.json");

  os << '[';

  for(size_t w=0; w<3; w++) {

    if(w != 0) {
      os << ',';
    }

    for(size_t i=0; i<10; i++) {

      os << '{'
         << "\"cat\":\"ExecutorObserver\","
         << "\"name\":\"" << "name:" << i << "\","
         << "\"ph\":\"X\","
         << "\"pid\":1,"
         << "\"tid\":" << w << ','
         << "\"ts\":" << w*3 + i << ','
         << "\"dur\":" << i;

      if(i != 10 - 1) {
        os << "},";
      }
      else {
        os << '}';
      }
    }
  }
  os << "]\n";

}
