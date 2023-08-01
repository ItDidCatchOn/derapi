#ifndef _DERIBITEXCHANGE_
#define _DERIBITEXCHANGE_

#include <cstring>
#include <cmath>

#include <unordered_set>
#include <unordered_map>

#include "strutils.h"

#include "DerivativeExchange.h"
#include "ExchangeFutureContract.h"
#include "ExchangeFutureContractSynthetic.h"
#include "DeribitEndpoint.h"

class DeribitExchange: public DerivativeExchange
{
  public:
    DeribitExchange(const dertype& dtype, const char* configfile): fEP(new DeribitEndpoint(dtype, configfile)){fCurrencies.SetDefaultCurrency("USD"); BasePostConstructor();}
    virtual ~DeribitExchange(){delete fEP;}

    int UpdateCurrencies();
    int UpdateInstruments();

  protected:
    int LoadNewFutureContractData(ExchangeInstrumentSetExpiryMap* em, const char* instr_name, json_object* obj, ExchangeFutureContract* fc);
    int LoadNewOptionContractData(ExchangeInstrumentSetExpiryMap* em, const char* instr_name, json_object* obj, ExchangeOptionContract* oc);
    DeribitEndpoint* fEP;

  private:
    DeribitExchange(const DeribitExchange&): DerivativeExchange(), fEP(NULL){}
    const DeribitExchange& operator=(const DeribitExchange&){return *this;}
};

#endif
