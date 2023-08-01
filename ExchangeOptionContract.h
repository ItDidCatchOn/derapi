#ifndef _EXCHANGEOPTIONCONTRACT_
#define _EXCHANGEOPTIONCONTRACT_

#include <cstdint>

#include <string>

#include "ExchangeUnderlyingAssetSubset.h"
#include "ExchangeInstrument.h"
#include "ExchangeFutureContractBase.h"

class ExchangeOptionContract: public ExchangeInstrument
{
  public:
    typedef double t_strike_price;
    const uint64_t t_infinitestamp=UINT64_MAX;
    ExchangeOptionContract(const char* instr_name): ExchangeInstrument(instr_name) {}
    ~ExchangeOptionContract(){}

    inline const t_timestamp& GetExpirationTimestamp() const{return expiration_timestamp;}
    inline void SetExpirationTimestamp(const t_timestamp& expiry){expiration_timestamp=expiry;}

    s_exchange_underlying_asset const* base_currency;
    s_exchange_underlying_asset const* quote_currency;
    uint32_t contract_size;
    bool call_option;
    double strike;
    t_timestamp creation_timestamp;
    bool is_active;

    double tick_size;
    double maker_commission;
    double taker_commission;
    double block_trade_commission;
    double min_trade_amount;

    double volume;
    double underlying_price;
    std::string underlying_index;
    double percent_price_change_24h;
    double open_interest_quantity;
    double mid_price;
    double mark_price;
    double last_price;
    double low_price_24h;
    double high_price_24h;
    double estimated_delivery_price;
    double bid_price;
    double ask_price;

  protected:
    t_timestamp expiration_timestamp;

  private:
    const ExchangeOptionContract& operator=(const ExchangeOptionContract&){return *this;}
};

#endif
