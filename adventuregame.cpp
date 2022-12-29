#include "adventuregame.hpp"
#include "eosio.token.hpp"
#include "eosio.system.hpp"
#include <cctype>

using namespace eosio;
static inline time_point_sec current_time_point_sec() { return time_point_sec(current_time_point()); };

ACTION adventuregame::reguser(name username)
{
  require_auth(username);
  auto itr = users.find(username.value);
  check(itr == users.end(), "User already registered");
  itr = users.emplace(username, [&](auto& row){
    row.username = username;
    row.number_of_slots = 3;
  });
}

void adventuregame::stake(name from, name to, vector<uint64_t> asset_ids, string memo)
{
  require_auth(from);
  if (to != get_self() || from == get_self()) return;
  auto itr = users.find(from.value);
  check(itr != users.end(), "User is not registered");
  check(memo == "deposit", "Incorrect memo!");
  
  if (memo == "deposit") {
    
    addstake(from, asset_ids, from);
  }
}

void adventuregame::addstake(name from, vector<uint64_t> asset_ids, name ram_payer )
{
  auto self = get_self();
  
  for (size_t i = 0; i <asset_ids.size(); i++)
  {
    auto itr = tools.find(asset_ids[i]);
    check(itr == tools.end(), "One of assets is already staked");
    name collectionz = atomics_t("atomicassets"_n, _self.value).require_find(asset_ids[i], "error incorrect collection")->collection_name;
    check(collectionz == COLLECTION_NAME, "error bro");
    name schemaz = atomics_t("atomicassets"_n, _self.value).require_find(asset_ids[i])->schema_name;
    check(schemaz == SCHEMA_NAME, "Wrong schema");
    int32_t template_id = atomics_t("atomicassets"_n, _self.value).require_find(asset_ids[i])->template_id;
    itr = tools.emplace(self, [&](auto &row){
      row.asset_id = asset_ids[i];
      row.owner = from;
      row.template_id = template_id;
      row.last_claim = 0;
    });
  }
}

ACTION adventuregame::unstake (name username, vector<uint64_t> asset_id, string memo)
{
  auto self = get_self();
  require_auth(username);
  
  auto ac_itr = users.find(username.value);
  check(ac_itr != users.end(), "Account doesn't exist");
  
  auto tool_itr = tools.find(asset_id[0]);
  check(tool_itr != tools.end(), "Tool isn't staked");
  
  action(
      permission_level(get_self(), "active"_n),
      "atomicassets"_n, "transfer"_n,
      transfer_args{
          self,
          username,
          asset_id,
          memo}
      
      ).send();
      
  tools.erase(tool_itr);
}

ACTION adventuregame::mine (name username, vector<uint64_t> asset_ids)
{
  auto self = get_self();
  require_auth(username);
  
  auto ac_itr = users.find(username.value);
  check(ac_itr != users.end(), "User not registered");
  auto slots = ac_itr->number_of_slots;
  check(asset_ids.size() <= slots, "too many tools, not enough slots");
  
    for (size_t i = 0; i <asset_ids.size(); i++) 
    {
      uint64_t timestamp = eosio::current_time_point().sec_since_epoch();
      auto tools_itr = tools.find(asset_ids[i]);
      check(tools_itr != tools.end(), "One or more tools are not staked");
      
      auto tools_conf_itr = toolsconfig.find(tools_itr->template_id);
      check(tools_itr->last_claim + tools_conf_itr->cooldown <= timestamp, "mining not ready yet");
      
      tools.modify(tools_itr, self, [&](auto &row) {
        row.last_claim = timestamp;
      });
      
      sendreward(username);
    }
}

void adventuregame::sendreward( name to)
{

  auto self = get_self();
  require_auth(self);
  string memo("reward");
  asset quantity = asset(2000000000, symbol_type(S(8, WAX)) );

  
  action(
    permission_level(get_self(), "active"_n),
    "eosio.token"_n, "transfer"_n,
    transfer_args{
        self,
        to,
        quantity,
        memo}
      
    ).send();
}
    
  
  

//ACTION adventuregame::setadventrs (uint64_t adv_id, uint64_t duration, uint64_t min_adv, uint64_t max_adv, string rarity, uint64_t cooldown)
//{
   //auto self = get_self();
   //require_auth(self);
   
   //auto itr = adventures.find(adv_id);
   //check(itr == adventures.end(), "Adventure already exists in table");
   //itr = adventures.emplace(self, [&](auto &row){
     //.adventure_id = adv_id;
     //row.duration_sec = duration;
     //row.min_adventurers = min_adv;
     //row.max_adventurers = max_adv;
     //row.req_rarity = rarity;
     //row.cooldown = cooldown;
   //});
//}

ACTION adventuregame::editusersl (name wallet, uint64_t num_slots)
{
  auto self = get_self();
  require_auth(self);
  
  auto itr = users.find(wallet.value);
  users.modify(itr, self, [&](auto &row) {
    row.number_of_slots = num_slots;
  });
}

ACTION adventuregame::deluser (name username)
{
  auto self = get_self();
  require_auth(self);
  
  auto itr = users.find(username.value);
  users.erase(itr);
}

ACTION adventuregame::deltools (uint64_t asset_id)
{
  require_auth(get_self());
  
  auto itr = tools.find(asset_id);
  tools.erase(itr);
}

ACTION adventuregame::toolconf( int32_t template_id, name tool_name, name rarity, uint64_t cooldown, asset craft_cost, asset reward)
{
  auto self = get_self();
  require_auth(self);
  
  auto itr = toolsconfig.find(template_id);
  check(itr == toolsconfig.end(), "tool already exists");
  
  itr = toolsconfig.emplace(self, [&](auto &row){
    row.template_id = template_id;
    row.tool_name = tool_name;
    row.rarity = rarity;
    row.cooldown = cooldown;
    row.craft_cost = craft_cost;
    row.reward = reward;
  });
}

  
  




   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
