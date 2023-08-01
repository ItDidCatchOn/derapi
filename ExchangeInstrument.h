#ifndef _EXCHANGEINSTRUMENT_
#define _EXCHANGEINSTRUMENT_

#include <string>

class ExchangeInstrumentSet;

class ExchangeInstrument
{
  public:
    typedef uint64_t t_timestamp;
    ExchangeInstrument(const char* instr_name): instrument_name(instr_name), fInstrumentSet(NULL){}
    ~ExchangeInstrument(){}

    std::string instrument_name;
    
  protected:
    void SetInstrumentSet(ExchangeInstrumentSet const* const instr_set){fInstrumentSet=instr_set;}
    ExchangeInstrumentSet const* fInstrumentSet;
  private:

    friend class ExchangeInstrumentSet;
    friend class ExchangeOptionContractPair;
};

#endif
