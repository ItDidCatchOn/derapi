#ifndef _EXCHANGEFUTURECONTRACTBASE_
#define _EXCHANGEFUTURECONTRACTBASE_

#include <cstdint>
#include <cmath>

#include <string>

#include "time_utils.h"

#include "ExchangeInstrument.h"
#include "Currencies.h"

class ExchangeFutureContractBase: public ExchangeInstrument
{
  public:
    typedef uint64_t t_timestamp;
    ExchangeFutureContractBase(const char* instr_name): ExchangeInstrument(instr_name) {}
    virtual ~ExchangeFutureContractBase(){}

    inline const double& GetInterestRate() const{return interest_rate;}
    inline const t_timestamp& GetExpirationTimestamp() const{return expiration_timestamp;}
    inline virtual void SetExpirationTimestamp(const t_timestamp& expiry){expiration_timestamp=expiry;}

    virtual void UpdateRate()=0;


  protected:
    t_timestamp expiration_timestamp;
    double interest_rate;

  private:
    const ExchangeFutureContractBase& operator=(const ExchangeFutureContractBase&){return *this;}
};

#endif
