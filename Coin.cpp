#include "Coin.h"

CoinConfig coinConfig;

static const CoinConfig btcConfig = {
  "Bitcoin", "BTC",
  0x00,       // p2pkhVersion
  0x05,       // p2shVersion
  "bc",       // bech32Hrp
  4,          // bech32PrefixLen ("bc1q")
  0x80,       // wifVersion
  '1',        // p2pkhChar
  '3',        // p2shChar
  'b',        // bech32Char
  '5',        // wifUncompChar
  "KL"        // wifCompChars
};

static const CoinConfig ltcConfig = {
  "Litecoin", "LTC",
  0x30,       // p2pkhVersion
  0x32,       // p2shVersion
  "ltc",      // bech32Hrp
  5,          // bech32PrefixLen ("ltc1q")
  0xB0,       // wifVersion
  'L',        // p2pkhChar
  'M',        // p2shChar
  'l',        // bech32Char
  '6',        // wifUncompChar
  "T"         // wifCompChars
};

void SetCoin(CoinType type) {
  if (type == COIN_BTC)
    coinConfig = btcConfig;
  else
    coinConfig = ltcConfig;
}

CoinType DetectCoinFromPrefix(const char *prefix) {
  if (!prefix || !prefix[0])
    return COIN_LTC; // default

  char c = prefix[0];

  // BTC
  if (c == '1' || c == '3')
    return COIN_BTC;
  if ((c == 'b' || c == 'B') && prefix[1] && (prefix[1] == 'c' || prefix[1] == 'C'))
    return COIN_BTC;

  // LTC
  if (c == 'L' || c == 'M')
    return COIN_LTC;
  if (c == 'l' && prefix[1] && prefix[1] == 't')
    return COIN_LTC;

  return COIN_LTC;
}
