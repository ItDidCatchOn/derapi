CNAMES := json_print.c
CXXNAMES := DeribitEndpoint.cxx Currencies.cxx DeribitExchange.cxx ExchangeInstrumentSetExpiryMap.cxx

LCOBJ := $(CNAMES:.c=.o)
LCODEP := $(LCOBJ:.o=.d)

LCXXOBJ := $(CXXNAMES:.cxx=.o)
LCXXODEP := $(LCXXOBJ:.o=.d)

LIBNAME:= option_strategy
LIB	:= lib$(LIBNAME).so

all:	$(LIB) $(EXES)

$(LIB): $(LCOBJ) $(LCXXOBJ)
	$(CXX) $(CXXFLAGS) -fPIC -shared -o $@ $^ $(LDFLAGS) -lm -lpthread -lssl -lcrypto -lcurl -ljson-c

$(LCODEP): %.d: %.c %.h
	@echo "Generating dependency file $@"
	@set -e; rm -f $@
	@$(CC) -M $(CFLAGS) $< > $@.tmp
	@sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.tmp > $@
	@rm -f $@.tmp

$(LCXXODEP) $(EDEPS): %.d: %.cxx %.h
	@echo "Generating dependency file $@"
	@set -e; rm -f $@
	$(CXX) -M $(CXXFLAGS) $< > $@.tmp
	@sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.tmp > $@
	@rm -f $@.tmp

include $(LCODEP) $(LCXXODEP)

$(LCOBJ): %.o: %.c %.h
	$(CC) $(CFLAGS) -fPIC -c -o $@ $<

$(LCXXOBJ): %.o: %.cxx %.h
	$(CXX) $(CXXFLAGS) -fPIC -c -o $@ $<

clean:
	rm -rf $(LCOBJ) $(LCXXOBJ) $(LCODEP) $(LCXXODEP)
	rm -rf build

clear: clean
	rm -rf $(LIB)
