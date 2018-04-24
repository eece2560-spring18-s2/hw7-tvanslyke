#ifndef MEMBER_H_
#define MEMBER_H_

#include <vector>
#include <string>
#include <stdint.h>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <deque>
#include <cassert>
#include "group.h"

namespace algdb {

class Member;
class Database;

#define COLOR_WHITE 0
#define COLOR_GRAY  1
#define COLOR_BLACK 2

class MemberConnection {
 public:
  Group *group;
  Member *dst;
  
  double GetWeight() const {
    return group->members.size() + 1;
  }
};

class Member {
  bool BFS_Visit(uint64_t id, Member* start_pos, std::deque<Member*>& pending) 
  {
    for(auto& p: connecting_members)
    {
      auto mem = p.second.dst;
      if(mem != start_pos and not mem->parent)
      {
        mem->parent = this;
        pending.push_back(mem);
      }
    }
    return false;
  }

  bool IDDFS_Visit(uint64_t id, Member* start_pos, std::size_t depth) 
  {
    if(depth == 0)
      return (this->member_id == id);
    for(auto& conn: connecting_members)
    {
      auto child = conn.second.dst;
      if(child == start_pos or child->parent)
        continue;
      child->parent = this;
      if(child->IDDFS_Visit(id, start_pos, depth - 1))
        return true;
      child->parent = nullptr;
    }
    return false;
  }
 public:
  uint64_t member_id;
  float lat;
  float lon;
  std::string member_name;

  std::vector<uint64_t> group_ids;
  std::vector<Group *> groups;
  std::unordered_map<uint64_t, MemberConnection> connecting_members;

  void PathToMemberBFS(uint64_t dst_member_id);
  void PathToMemberIDDFS(uint64_t dst_member_id);
  void PrintPath(Member *dst);
  void DumpConnections();
  
  static void ConnectGroupMembers(Group& group);

  // For graph algorithms
  int color = COLOR_WHITE;
  double key; // For BST DFT, this is the depth. For MST, this is the key.
  Member *parent;
};

}

#endif
