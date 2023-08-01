#ifndef _EXCHANGEOPTIONCONTRACTPAIR_
#define _EXCHANGEOPTIONCONTRACTPAIR_

#include "ExchangeOptionContract.h"

class ExchangeOptionContractPair
{
  public:
    ExchangeOptionContractPair(): fCall(NULL), fPut(NULL){}
    ~ExchangeOptionContractPair(){Clear();}

    void Clear(){if(fCall) {delete fCall; fCall=NULL;} if(fPut) {delete fPut; fPut=NULL;}}

    inline void SetCall(ExchangeInstrumentSet const* const instr_set, ExchangeOptionContract* call){if(fCall) delete fCall; fCall=call; fCall->SetInstrumentSet(instr_set);}
    inline void SetPut(ExchangeInstrumentSet const* const instr_set, ExchangeOptionContract* put){if(fPut) delete fPut; fPut=put; fPut->SetInstrumentSet(instr_set);}

    inline void DeleteCall(){if(fCall) {delete fCall; fCall=NULL;}}
    inline void DeletePut(){if(fPut) {delete fPut; fPut=NULL;}}

    inline ExchangeOptionContract const* GetCall() const{return fCall;}
    inline ExchangeOptionContract* GetCall(){return fCall;}
    inline ExchangeOptionContract const* GetPut() const{return fPut;}
    inline ExchangeOptionContract* GetPut(){return fPut;}

  protected:
    ExchangeOptionContract* fCall;
    ExchangeOptionContract* fPut;

  private:
    ExchangeOptionContractPair(const ExchangeOptionContractPair&){}
    const ExchangeOptionContractPair& operator=(const ExchangeOptionContractPair&){return *this;}
};

#endif
