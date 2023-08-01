# derapi
Basic C++ implementation of the Deribit API

Implementation of the endpoint for Deribit. The DeribitExchange class allows to load data for the option and future contract instruments. For the sake of option pricing calculations, a future contract is associated to each option contract. If there is no future contract with a matching expiry, a synthetic one is created using an interpolated value.
