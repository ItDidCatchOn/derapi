#ifndef _DERIVATIVEEXCHANGE_
#define _DERIVATIVEEXCHANGE_

#include "ExchangeUnderlyingAssetSubset.h"
#include "ExchangeInstrumentSetTree.h"

class DerivativeExchange
{
  public:
    DerivativeExchange(): fCurrencies(), fUnderlyingAssetSubset(NULL), fInstruments(){}
    virtual ~DerivativeExchange(){delete fUnderlyingAssetSubset;};

    inline void AddAllCurrencies(){fUnderlyingAssetSubset->AddAll();}

    inline s_exchange_underlying_asset const& AddCurrency(const char* symbol) {return fUnderlyingAssetSubset->Add(symbol);}

    inline void RemoveAllCurrencies(){fUnderlyingAssetSubset->RemoveAll();}

    inline void RemoveCurrency(const char* symbol){fUnderlyingAssetSubset->Remove(symbol);}

    virtual int UpdateCurrencies()=0;
    virtual int UpdateInstruments()=0;

  protected:
    void BasePostConstructor(){fUnderlyingAssetSubset=new ExchangeUnderlyingAssetSubset(fCurrencies);}

    ExchangeCurrencySet fCurrencies;
    ExchangeUnderlyingAssetSubset* fUnderlyingAssetSubset;
    ExchangeInstrumentSetTree fInstruments;

  private:
    DerivativeExchange(const DerivativeExchange&): fCurrencies(), fUnderlyingAssetSubset(NULL), fInstruments(){}
    const DerivativeExchange& operator=(const DerivativeExchange&){return *this;}
};

#endif
