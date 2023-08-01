#ifndef _EXCHANGEFUTURECONTRACT_
#define _EXCHANGEFUTURECONTRACT_

#include <cstdint>

#include <string>

#include "ExchangeFutureContractBase.h"
#include "ExchangeUnderlyingAssetSubset.h"

class ExchangeFutureContract: public ExchangeFutureContractBase
{
  public:
    typedef uint64_t t_timestamp;
    ExchangeFutureContract(const char* instr_name): ExchangeFutureContractBase(instr_name) {}
    ~ExchangeFutureContract(){}

    inline void UpdateRate()
    {
      //printf("Mark price is %e, quote currency value is %e, base currency value is %e\n", mark_price, quote_currency->value,  base_currency->value);
      interest_rate = (pow(mark_price * quote_currency->value / base_currency->value, YEARMS / (double)(expiration_timestamp - base_currency->request_time)) - 1) * 100;
      //printf("Updating rate from contract '%s' to %9.4f%%\n", instrument_name.c_str(), interest_rate);
    }

    s_exchange_underlying_asset const* base_currency;
    s_exchange_underlying_asset const* quote_currency;
    uint32_t contract_size;
    t_timestamp creation_timestamp;
    bool is_active;
    uint32_t max_leverage;

    double tick_size;
    double maker_commission;
    double taker_commission;
    double block_trade_commission;
    double min_trade_amount;

    double volume;
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

  private:
    const ExchangeFutureContract& operator=(const ExchangeFutureContract&){return *this;}
};

#endif
