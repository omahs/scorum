#pragma once
#include <vector>
#include <functional>
#include <scorum/chain/schema/bet_objects.hpp>

namespace scorum {
namespace chain {

struct data_service_factory_i;

struct dynamic_global_property_service_i;
struct betting_property_service_i;
struct matched_bet_service_i;
struct pending_bet_service_i;
struct game_service_i;
struct account_service_i;

using scorum::protocol::market_type;
using scorum::protocol::wincase_type;

class betting_property_object;

namespace dba {
template <typename> struct db_accessor;
struct db_accessor_factory;
}

struct betting_service_i
{
    // TODO: will be removed after db_accessors introduction
    using pending_bet_crefs_type = std::vector<std::reference_wrapper<const pending_bet_object>>;
    using matched_bet_crefs_type = std::vector<std::reference_wrapper<const matched_bet_object>>;

    virtual bool is_betting_moderator(const account_name_type& account_name) const = 0;

    virtual void cancel_game(const game_id_type& game_id) = 0;
    virtual void cancel_bets(const game_id_type& game_id) = 0;
    virtual void cancel_bets(const game_id_type& game_id, fc::time_point_sec created_from) = 0;
    virtual void cancel_bets(const game_id_type& game_id, const fc::flat_set<market_type>& cancelled_markets) = 0;

    virtual void cancel_pending_bet(pending_bet_id_type id) = 0;
    virtual void cancel_pending_bets(const game_id_type& game_id) = 0;
    virtual void cancel_pending_bets(const game_id_type& game_id, pending_bet_kind kind) = 0;
    virtual void cancel_pending_bets(const pending_bet_crefs_type& pending_bets) = 0;

    virtual void cancel_matched_bets(const game_id_type& game_id) = 0;
    virtual void cancel_matched_bets(const matched_bet_crefs_type& matched_bets) = 0;
};

class betting_service : public betting_service_i
{
public:
    betting_service(data_service_factory_i&, dba::db_accessor_factory&);

    bool is_betting_moderator(const account_name_type& account_name) const override;

    void cancel_game(const game_id_type& game_id) override;
    void cancel_bets(const game_id_type& game_id) override;
    void cancel_bets(const game_id_type& game_id, fc::time_point_sec created_from) override;
    void cancel_bets(const game_id_type& game_id, const fc::flat_set<market_type>& cancelled_markets) override;

    void cancel_pending_bet(pending_bet_id_type id) override;
    void cancel_pending_bets(const game_id_type& game_id) override;
    void cancel_pending_bets(const game_id_type& game_id, pending_bet_kind kind) override;
    void cancel_pending_bets(const pending_bet_crefs_type& pending_bets) override;

    void cancel_matched_bets(const game_id_type& game_id) override;
    void cancel_matched_bets(const matched_bet_crefs_type& matched_bets) override;

private:
    void return_or_restore_bet(const bet_data& bet, game_id_type game_id, fc::time_point_sec threshold);

    dynamic_global_property_service_i& _dgp_property_service;
    betting_property_service_i& _betting_property_service;
    matched_bet_service_i& _matched_bet_svc;
    pending_bet_service_i& _pending_bet_svc;
    game_service_i& _game_svc;
    account_service_i& _account_svc;
    dba::db_accessor<betting_property_object>& _betting_property_dba;
};
}
}
