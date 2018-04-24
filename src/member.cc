#include "member.h"

#include <queue>

#include "database.h"

namespace algdb {

void Member::DumpConnections() {
  std::cout << "(" << member_id << ")";
  for (auto &m : connecting_members) {
    auto conn = m.second;
    std::cout << "->" << conn.dst->member_id << 
      "(" << conn.group->group_id << ")";
  }
  std::cout << "\n";
}

void Member::PathToMemberBFS(uint64_t dst_member_id) {
  std::deque<Member*> pending{this};
  while(pending.size() > 0)
  {
    auto member = pending.front();
    pending.pop_front();
    if(member->BFS_Visit(dst_member_id, this, pending))
      break;
  }
}

void Member::PathToMemberIDDFS(uint64_t dst_member_id) {
  for(std::size_t i = 0; not IDDFS_Visit(dst_member_id, this, i); ++i)
  {}
}
  
void Member::PrintPath(Member* dst) {
  Member *curr = dst;
  while(curr && curr != this) {
    std::cout << curr->member_name << " <- ";
    curr = curr->parent;
  }
  std::cout << this->member_name << "\n";
}

void Member::ConnectGroupMembers(Group& group)
{
  auto& members = group.members;
  for(auto i = begin(members); i < end(members); ++i)
  {
    for(auto j = std::next(i); j < end(members); ++j)
    {
      auto mem1 = *i;
      auto mem2 = *j;
      bool already_exists = not mem1->connecting_members.emplace(
        mem2->member_id, MemberConnection{&group, mem2}
      ).second;
      // if this member connection already exists, don't try to insert in 
      // the other connection vector
      if(already_exists)
        continue;
      mem2->connecting_members.emplace(
        mem1->member_id, MemberConnection{&group, mem1}
      );
    }
  }
}

}
