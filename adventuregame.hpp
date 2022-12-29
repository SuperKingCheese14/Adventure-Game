#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>
#include <eosio/system.hpp>
#include <string>
#include <cstdlib> 
#include "atomicassets.hpp"
#include "atomicdata.hpp"
#include <eosio/transaction.hpp>
#include <ctime>

using namespace std;
using namespace eosio;

#define ATOMIC name("atomicassets")
#define EOSIO name("eosio")

#define CONTRACTIN name("victoria1234")

CONTRACT adventuregame : public contract {

  public:
    
    using contract::contract;
   
     struct transfer_args
    {
        name from;
        name to;
        vector<uint64_t>  asset_ids;
        string memo;
    };
    
    struct eosio_transfer_args
    {
        name from;
        name to;
        asset reward;
        string memo;
    };
    

    ACTION reguser(name username);
    
    [[eosio::on_notify("atomicassets::transfer")]] void stake(
      name  from,
      name  to,
      vector <uint64_t> asset_ids,
      string memo
    );
    
    ACTION mine(name username, vector<uint64_t> asset_ids);
    
    
    
    //ACTION getadventures(name username);
    
    //ACTION executeadventure(name username, uint64_t adventure_id);
    
    //ACTION buypacks (name from, name to, uint64_t pack_id, string memo);
    
    //ACTION buyslots (name username, uint64_t number_of_slots);
    
    //ACTION craft (name username, int32_t template_id);
    
    //ACTION unstake (name username, vector<uint64_t> asset_ids);
    
    //void genadventures (name username, uint64_t numslots);
    
    //void wipeadventures (name username);
    
    //ACTION setadventrs(uint64_t adv_id, uint64_t duration, uint64_t min_adv, uint64_t max_adv, string rarity, uint64_t cooldown);
    
    ACTION editusersl (name wallet, uint64_t num_slots);
    
    ACTION deluser (name username);
    
    ACTION deltools (uint64_t asset_id);
    
    ACTION toolconf ( int32_t template_id, name tool_name, name rarity, uint64_t cooldown, asset craft_cost, asset reward);
    
    ACTION unstake (name username, vector<uint64_t> asset_id, string memo);
    
    
    
  private:
  
    struct [[eosio::table]] assets_s {
      uint64_t asset_id;
      name collection_name;
      name schema_name;
      int32_t template_id;
      name ram_payer;
      vector<asset> backed_tokens;
      vector<uint8_t> immutable_serialized_data;
      vector<uint8_t> mutable_serialized_data;
      uint64_t primary_key() const { return asset_id; }
    };
    
    typedef multi_index<"assets"_n, assets_s> atomics_t;
  
    TABLE users_t
    {
      name                username;
      uint64_t            number_of_slots;
      
      
      auto primary_key() const { return username.value; }
    };
    
    TABLE hero_t
    {
      int32_t  template_id;
      name     tool_name;
      name     rarity;
      uint64_t cooldown;
      asset    craft_cost;
      asset    reward;
      
      auto primary_key() const { return template_id; }
    };
    
    TABLE tools_t
    {
      uint64_t    asset_id;
      name        owner;
      int32_t     template_id;
      uint64_t    last_claim;
      
      auto primary_key() const { return asset_id; }
    };
       
    typedef eosio::multi_index<name("users"), users_t> users_s;
    typedef eosio::multi_index<name("toolsconfig"), hero_t> hero_s;
    typedef eosio::multi_index<name("tools"), tools_t> tools_s;
    
    users_s users = users_s(get_self(), get_self().value);
    hero_s toolsconfig = hero_s(get_self(), get_self().value);
    tools_s tools = tools_s(get_self(), get_self().value);
    
    void in_contract_transfer (name to, vector<uint64_t> asset_ids, string memo);
    
    void sendreward (name to);
    
    
    void mintnft(
        name authorized_minter,
        name collection_name,
        name schema_name,
        int32_t template_id,
        name new_owner,
        ATTRIBUTE_MAP immutable_data,
        ATTRIBUTE_MAP mutable_data,
        vector<asset> tokens_to_back);
        
    void addstake ( name from, vector<uint64_t> asset_ids, name ram_payer);    
        
    name COLLECTION_NAME = "gen1pokemon1"_n;  
    name SCHEMA_NAME = "pokemon"_n;      
};
    
    
    
    
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      


