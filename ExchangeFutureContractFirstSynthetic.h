#ifndef _EXCHANGEFUTURECONTRACTFIRSTSYNTHETIC_
#define _EXCHANGEFUTURECONTRACTSFIRSTYNTHETIC_

#include <cstdint>

#include <string>

#include "ExchangeFutureContractBase.h"
#include "ExchangeUnderlyingAssetSubset.h"

class ExchangeFutureContractFirstSynthetic: public ExchangeFutureContractBase
{
  public:
    typedef uint64_t t_timestamp;
    ExchangeFutureContractFirstSynthetic(const char* instr_name, const ExchangeFutureContract& later_contract): ExchangeFutureContractBase(instr_name), later(later_contract) {}
    ~ExchangeFutureContractFirstSynthetic(){}

    inline void UpdateRate()
    {
      interest_rate = later.GetInterestRate();
      //printf("Updating rate from contract '%s' to %9.4f%%\n", instrument_name.c_str(), interest_rate);
    }

    const ExchangeFutureContract& later;

  protected:

  private:
    const ExchangeFutureContractFirstSynthetic& operator=(const ExchangeFutureContractFirstSynthetic&){return *this;}
};

#endif
