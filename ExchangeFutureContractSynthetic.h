#ifndef _EXCHANGEFUTURECONTRACTSYNTHETIC_
#define _EXCHANGEFUTURECONTRACTSYNTHETIC_

#include <cstdint>

#include <string>

#include "ExchangeFutureContractBase.h"
#include "ExchangeFutureContract.h"
#include "Currencies.h"

class ExchangeFutureContractSynthetic: public ExchangeFutureContractBase
{
  public:
    typedef uint64_t t_timestamp;
    ExchangeFutureContractSynthetic(const char* instr_name, const ExchangeFutureContract& earlier_contract, const ExchangeFutureContract& later_contract): ExchangeFutureContractBase(instr_name), earlier(earlier_contract), later(later_contract) {}
    ~ExchangeFutureContractSynthetic(){}

    inline void SetExpirationTimestamp(const t_timestamp& expiry)
    {
      ExchangeFutureContractBase::SetExpirationTimestamp(expiry);
      interpolation_factor = (expiration_timestamp - earlier.GetExpirationTimestamp()) / (double)(later.GetExpirationTimestamp() - earlier.GetExpirationTimestamp());
    }

    inline void UpdateRate()
    {
      const double earlier_value = earlier.mark_price * earlier.quote_currency->value;
      interest_rate = (pow(earlier_value * pow(later.mark_price * later.quote_currency->value / earlier_value, interpolation_factor) / earlier.base_currency->value, YEARMS / (double)(expiration_timestamp - earlier.base_currency->request_time)) - 1) * 100;
      //printf("Updating rate from contract '%s' to %9.4f%%\n", instrument_name.c_str(), interest_rate);
    }

    const ExchangeFutureContract& earlier;
    const ExchangeFutureContract& later;
    double interpolation_factor;

  protected:

  private:
    const ExchangeFutureContractSynthetic& operator=(const ExchangeFutureContractSynthetic&){return *this;}
};

#endif
