#ifndef _EXCHANGEINSTRUMENTSET_
#define _EXCHANGEINSTRUMENTSET_

#include <map>

#include "ExchangeFutureContractBase.h"
#include "ExchangeOptionContractPairSet.h"

class ExchangeInstrumentSet
{
  public:
    ExchangeInstrumentSet(): fFutureContract(NULL), fOptionContractPairSet(NULL){}
    ~ExchangeInstrumentSet(){Clear();}

    void Clear()
    {
      if(fFutureContract) {
	delete fFutureContract;
	fFutureContract=NULL;
      }

      if(fOptionContractPairSet) {
	delete fOptionContractPairSet;
	fOptionContractPairSet=NULL;
      }

    }

    inline void SetFutureContract(ExchangeFutureContractBase* future_contract){if(fFutureContract) delete fFutureContract; fFutureContract=future_contract; fFutureContract->SetInstrumentSet(this);}

    inline ExchangeOptionContractPairSet& AddOptionContractPairSet(){if(!fOptionContractPairSet) fOptionContractPairSet=new ExchangeOptionContractPairSet; return *fOptionContractPairSet;}

    inline void DeleteFutureContract(){if(fFutureContract) {delete fFutureContract; fFutureContract=NULL;}}
    inline void DeleteOptionContractPairSet(){if(fOptionContractPairSet) {delete fOptionContractPairSet; fOptionContractPairSet=NULL;}}

    inline ExchangeFutureContractBase* GetFutureContract(){return fFutureContract;}
    inline ExchangeOptionContractPairSet* GetOptionContractPairSet(){return fOptionContractPairSet;}

  protected:
    ExchangeFutureContractBase*  fFutureContract;
    ExchangeOptionContractPairSet* fOptionContractPairSet;

  private:
    ExchangeInstrumentSet(const ExchangeInstrumentSet&){}
    const ExchangeInstrumentSet& operator=(const ExchangeInstrumentSet&){return *this;}
};

#endif
