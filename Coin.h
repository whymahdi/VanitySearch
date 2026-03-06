#ifndef COINH
#define COINH

#include <cstdint>
#include <cstring>

enum CoinType {
  COIN_BTC = 0,
  COIN_LTC = 1
};

struct CoinConfig {
  const char *name;
  const char *ticker;
  uint8_t p2pkhVersion;
  uint8_t p2shVersion;
  const char *bech32Hrp;
  int bech32PrefixLen;
  uint8_t wifVersion;
  char p2pkhChar;
  char p2shChar;
  char bech32Char;
  char wifUncompChar;
  const char *wifCompChars;
};

extern CoinConfig coinConfig;

void SetCoin(CoinType type);
CoinType DetectCoinFromPrefix(const char *prefix);

#endif
