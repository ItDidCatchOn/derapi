#ifndef _EXCHANGEFUTURECONTRACTLASTSYNTHETIC_
#define _EXCHANGEFUTURECONTRACTLASTSYNTHETIC_

#include <cstdint>

#include <string>

#include "ExchangeFutureContractBase.h"
#include "ExchangeFutureContract.h"
#include "Currencies.h"

class ExchangeFutureContractLastSynthetic: public ExchangeFutureContractBase
{
  public:
    typedef uint64_t t_timestamp;
    ExchangeFutureContractLastSynthetic(const char* instr_name, const ExchangeFutureContract& earlier_contract): ExchangeFutureContractBase(instr_name), earlier(earlier_contract) {}
    ~ExchangeFutureContractLastSynthetic(){}

    inline void UpdateRate()
    {
      interest_rate = earlier.GetInterestRate();
      //printf("Updating rate from contract '%s' to %9.4f%%\n", instrument_name.c_str(), interest_rate);
    }

    const ExchangeFutureContract& earlier;

  protected:

  private:
    const ExchangeFutureContractLastSynthetic& operator=(const ExchangeFutureContractLastSynthetic&){return *this;}
};

#endif
